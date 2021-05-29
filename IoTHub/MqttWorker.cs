using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Options;

namespace IoTHub
{
    public class MqttWorker : BackgroundService
    {
        private readonly ILogger<MqttWorker> _logger;
        private IConfiguration _configuration;


        public MqttWorker(ILogger<MqttWorker> logger, IConfiguration configuration)
        {
            _logger = logger;
            _configuration = configuration;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            Console.WriteLine("Start");
            var factory = new MqttFactory();
            var mqttClient = factory.CreateMqttClient();
             var mqttConfig = GetMqttConfig();

            var options = new MqttClientOptionsBuilder()
                .WithTcpServer(mqttConfig.Broker, mqttConfig.Port)
                .Build();
            var result = await mqttClient.ConnectAsync(options, CancellationToken.None); // Since 3.0.5 with CancellationToken

            Console.WriteLine(result.ResultCode);
           
            await mqttClient.SubscribeAsync(new MqttTopicFilterBuilder().WithTopic($"{mqttConfig.Topic}/#").Build());
            mqttClient.UseConnectedHandler(async e =>
            {
                // Doesn't seem to work
                Console.WriteLine("### CONNECTED WITH SERVER ###");

                
                Console.WriteLine("### SUBSCRIBED ###");
            });

            mqttClient.UseApplicationMessageReceivedHandler(e =>
            {
                Console.WriteLine("### RECEIVED APPLICATION MESSAGE ###");
                Console.WriteLine($"+ Topic = {e.ApplicationMessage.Topic}");
                Console.WriteLine($"+ Payload = {Encoding.UTF8.GetString(e.ApplicationMessage.Payload)}");
                Console.WriteLine($"+ QoS = {e.ApplicationMessage.QualityOfServiceLevel}");
                Console.WriteLine($"+ Retain = {e.ApplicationMessage.Retain}");
                Console.WriteLine();

                // Task.Run(() => mqttClient.PublishAsync("iot_hub", "Received message"));
            });
            
            // var message = new MqttApplicationMessageBuilder()
            //     .WithTopic("iot_hub")
            //     .WithPayload("Hello World")
            //     .WithExactlyOnceQoS()
            //     .WithRetainFlag()
            //     .Build();

            // await mqttClient.PublishAsync(message, CancellationToken.None); // Since 3.0.5 with CancellationToken
            // while (!stoppingToken.IsCancellationRequested)
            // {
            //     _logger.LogInformation("Worker running at: {time}", DateTimeOffset.Now);
            //     await Task.Delay(1000, stoppingToken);

            //     Console.WriteLine(result.ReasonString);
            // }
        }

        private MqttConfig GetMqttConfig() {
            MqttConfig config = new MqttConfig();
            config.Broker = _configuration[ConfigurationConstants.MQTT_BROKER];
            config.Topic = _configuration[ConfigurationConstants.MQTT_TOPIC];

            int port = 1883;
            int.TryParse(_configuration[ConfigurationConstants.MQTT_PORT], out port);
            config.Port = port;

            return config;
        }
    }
}
