using System;
using System.Collections.Concurrent;
using System.ComponentModel;
using System.Threading;
using System.Threading.Tasks;
using Acr.UserDialogs;
using Plugin.BLE;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;
using Polly;

namespace WagonLights.Wagon
{
    public class WagonBluetooth : INotifyPropertyChanged
    {
        static readonly Guid WagonId = new Guid("00000000-0000-0000-0000-c7414cbfb53a");

        readonly BlockingCollection<WagonCommand> commands = new BlockingCollection<WagonCommand>();

        readonly IAdapter adapter;
        IDevice device;
        ICharacteristic tempCharacteristic;
        ICharacteristic programCharacteristic;
        ICharacteristic brightnessCharacteristic;
        ICharacteristic paletteCharacteristic;

        readonly Policy retryForever = Policy
            .Handle<Exception>()
            .WaitAndRetryForeverAsync(x => TimeSpan.FromSeconds(1), (x, y) =>
            {
                var t = x;
            });

        public WagonBluetooth()
        {
            adapter = CrossBluetoothLE.Current.Adapter;
            adapter.DeviceDisconnected += (x, y) => Connected = false;
            adapter.DeviceConnectionLost += (x, y) => Connected = false;
        }

        public async Task Start()
        {
            while (true)
            {
                var command = commands.Take();
                if (!Connected || device == null || device.State != DeviceState.Connected)
                {
                    await Connect();
                }

                try
                {
                    switch (command.Type)
                    {
                        case WagonCommandType.Program:
                            await programCharacteristic.WriteAsync(command.Command);
                            break;
                        case WagonCommandType.Brightness:
                            await brightnessCharacteristic.WriteAsync(command.Command);
                            break;
                        case WagonCommandType.Palette:
                            await paletteCharacteristic.WriteAsync(command.Command);
                            break;
                    }
                }
                catch (Exception)
                {
                }
                finally
                {
                    await Task.Delay(10);
                }
            }
        }

        async Task Connect()
        {
            using (UserDialogs.Instance.Loading("Connecting..."))
            {
                await retryForever.ExecuteAsync(async () =>
                {
                    device = await adapter.ConnectToKnownDeviceAsync(WagonId);
                    var services = await device.GetServicesAsync();
                    var characs = await services[3].GetCharacteristicsAsync();
                    tempCharacteristic = characs[0];
                    programCharacteristic = characs[1];
                    brightnessCharacteristic = characs[2];
                    paletteCharacteristic = characs[3];
                    tempCharacteristic.ValueUpdated += (x, y) =>
                    {
                        Temperature = y.Characteristic.StringValue + "°C";
                    };
                    await tempCharacteristic.StartUpdatesAsync();
                });
                Connected = true;
            }
        }

        public void SetProgram(int value)
        {
            commands.Add(new WagonCommand
            {
                Type = WagonCommandType.Program,
                Command = BitConverter.GetBytes(value)
            });
        }

        public void SetPalette(int value)
        {
            commands.Add(new WagonCommand
            {
                Type = WagonCommandType.Palette,
                Command = BitConverter.GetBytes(value)
            });
        }

        int lastSetBrightness = 0;

        public void SetBrightness(int value)
        {
            var current = Interlocked.Increment(ref lastSetBrightness);
            Task.Delay(200).ContinueWith(x =>
            {
                if (current == lastSetBrightness)
                {
                    commands.Add(new WagonCommand
                    {
                        Type = WagonCommandType.Brightness,
                        Command = BitConverter.GetBytes(value)
                    });
                }
                x.Dispose();
            });
        }

        bool connected;
        public bool Connected
        {
            get => connected;
            set
            {
                if (connected == value) return;
                connected = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Connected"));
            }
        }
        
        string temperature;
        public string Temperature
        {
            get => temperature;
            set
            {
                if (temperature == value) return;
                temperature = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Temperature"));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}