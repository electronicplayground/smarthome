# examples/server_simple.py
from aiohttp import web
import asyncio
import serial
# import paho.mqtt.client as mqtt

from mqtt_client import MqttClientMixin

from configuration.config_keys import ConfigKeys
from configuration.configuration import Configuration

config = Configuration.Instance()

async def handle(request):
    name = request.match_info.get('name', "Anonymous")
    text = "Hello, " + name
    return web.Response(text=text)


class Server(MqttClientMixin):
    def __init__(self):
        self._app = web.Application()
        print(config[ConfigKeys.WS_ENDPOINT])
        self._app.add_routes(
            [
                web.get(config[ConfigKeys.WS_ENDPOINT], self.wshandle),
            ]
        )
        # self._app.add_routes([web.get('/', handle),
        #                       web.get('/{name}', handle)])
        self._app.on_startup.append(self.start_background_tasks)
        self._app.on_cleanup.append(self.cleanup_background_tasks)

        self._ws_clients = []
        self._serial_read_queue = asyncio.Queue(maxsize=100)
        self._serial_write_queue = asyncio.Queue(maxsize=100)

        self._serial = serial.Serial()

    # The callback for when a PUBLISH message is received from the server.
    def on_message(self, client, userdata, msg):
        print(msg.topic+" "+str(msg.payload))
        try:
            print("Try write to queue")
            self._serial_write_queue.put_nowait(str(msg.payload))
        except Exception as e:
            print(e)
        print("added to queue")

    async def mqtt_run(self, app):
        if not self.mqtt_enabled:
            return
        while True:
            await asyncio.sleep(0.1)
            self.loop()

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
                self.publish(config[ConfigKeys.MQTT_PUBLISH_TOPIC], msg)
                for ws_client in self._ws_clients:
                    if not ws_client.closed:
                        await ws_client.send_str(str(msg))
                       
    async def serial_write_queue(self, app):
            while True:
                await asyncio.sleep(0.1)
                if not self._serial_write_queue.empty():
                    
                    msg = await self._serial_write_queue.get()
                    print(f"Writing to serial {msg}")
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

    def open_serial(self):
        if not config[ConfigKeys.SERIAL_PORT]:
            print("Serial port not configured.")
            return
        if not self._serial.is_open:
            print("Opening serial")
            self._serial.port = config[ConfigKeys.SERIAL_PORT]
            self._serial.baudrate = 9600
            self._serial.timeout = 1
            self._serial.open()


    async def start_background_tasks(self, app):
        self.open_serial()
       
        app['serial_driver'] = asyncio.create_task(self.read_serial_async(app))
        app['serial_read_queue'] = asyncio.create_task(self.serial_read_queue(app))
        app['serial_write_queue'] = asyncio.create_task(self.serial_write_queue(app))
        app['mqtt_run'] = asyncio.create_task(self.mqtt_run(app))

    async def cleanup_background_tasks(self, app):
        # await asyncio.sleep(0.1)
        self._serial.close()
        self.disconnect()
        app['serial_driver'].cancel()
        await app['serial_driver']
        app['serial_read_queue'].cancel()
        await app['serial_read_queue']
        app['serial_write_queue'].cancel()
        await app['serial_write_queue']
        app['mqtt_run'].cancel()
        await app['mqtt_run']

    def run(self):
        self.initalize_mqtt()
        web.run_app(self._app, host='0.0.0.0', port=8001)


if __name__ == '__main__':
    server = Server()
    server.run()