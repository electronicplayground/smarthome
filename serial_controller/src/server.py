# examples/server_simple.py
from aiohttp import web
import asyncio
import serial
import paho.mqtt.client as mqtt


async def handle(request):
    name = request.match_info.get('name', "Anonymous")
    text = "Hello, " + name
    return web.Response(text=text)

import paho.mqtt.client as mqtt


class Server:
    def __init__(self):
        self._app = web.Application()
        self._app.add_routes([web.get('/', handle),
                         web.get('/echo', self.wshandle),
                         web.get('/{name}', handle)])
        self._app.on_startup.append(self.start_background_tasks)
        self._app.on_cleanup.append(self.cleanup_background_tasks)

        self._ws_clients = []
        self._serial_read_queue = asyncio.Queue(maxsize=100)
        self._serial_write_queue = asyncio.Queue(maxsize=100)

        self._serial = serial.Serial()

        
        self._client = mqtt.Client()
        self._client.on_connect = self.on_connect
        self._client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("/test")

    # The callback for when a PUBLISH message is received from the server.
    def on_message(self, client, userdata, msg):
        print(msg.topic+" "+str(msg.payload))

    async def mqtt_run(self, app):
        self._client.connect("localhost", 1883, 60)
        while True:
            await asyncio.sleep(0.1)
            self._client.loop()

            
    async def read_serial_async(self, app):  
        while True:
            await asyncio.sleep(0.1)
            if self._serial.is_open:
                #  reads a line from serial and returns byte array
                # self._serial.write(b'hello\n')
                line = self._serial.readline()
                if not line:
                    continue
                line = line.decode('utf-8')
                await self._serial_read_queue.put(line)

    async def serial_read_queue(self, app):
        while True:
            await asyncio.sleep(0.1)
            if not self._serial_read_queue.empty():
                msg = await self._serial_read_queue.get()
                self._client.publish("/serial", payload=str(msg), qos=0, retain=False)
                for ws_client in self._ws_clients:
                    if not ws_client.closed:
                        await ws_client.send_str(str(msg))
                       

    async def serial_write_queue(self, app):
            while True:
                await asyncio.sleep(0.1)
                if not self._serial_write_queue.empty():
                    msg = await self._serial_write_queue.get()
                    if self._serial.is_open:
                        self._serial.write(str.encode(f"{str(msg)}\n"))
                    
    async def wshandle(self, request):
        ws = web.WebSocketResponse()
        await ws.prepare(request)
        self._ws_clients.append(ws)
        async for msg in ws:
            if msg.type == web.WSMsgType.text:
                await self._serial_write_queue.put(msg.data)
            elif msg.type == web.WSMsgType.binary:
                await asyncio.sleep(0.1)
                # await ws.send_bytes(msg.data)
            elif msg.type == web.WSMsgType.close:
                break

        return ws

    async def start_background_tasks(self, app):
        if not self._serial.is_open:
            print("Opening serial")
            self._serial.port = "COM3"
            self._serial.baudrate = 9600
            self._serial.timeout = 1
            self._serial.open()
        app['serial_driver'] = asyncio.create_task(self.read_serial_async(app))
        app['serial_read_queue'] = asyncio.create_task(self.serial_read_queue(app))
        app['serial_write_queue'] = asyncio.create_task(self.serial_write_queue(app))
        app['mqtt_run'] = asyncio.create_task(self.mqtt_run(app))

    async def cleanup_background_tasks(self, app):
        # await asyncio.sleep(0.1)
        self._serial.close()
        self._client.disconnect()
        app['serial_driver'].cancel()
        await app['serial_driver']
        app['serial_read_queue'].cancel()
        await app['serial_read_queue']
        app['serial_write_queue'].cancel()
        await app['serial_write_queue']
        app['mqtt_run'].cancel()
        await app['mqtt_run']

    def run(self):
        web.run_app(self._app, host='0.0.0.0', port=8001)


if __name__ == '__main__':
    server = Server()
    server.run()