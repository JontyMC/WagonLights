using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using Plugin.BLE;
using Xamarin.Forms;

namespace WagonLights.ViewModels
{
    public class DiscoverViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<DeviceViewModel> Devices { get; set; } = new ObservableCollection<DeviceViewModel>();

        public ICommand Refresh { get; }
        public ICommand DeviceTapped { get; }

        public DiscoverViewModel()
        {
            Refresh = new Command(async () => await Discover());
        }

        bool discoveringDevices;
        public bool DiscoveringDevices
        {
            get => discoveringDevices;
            set
            {
                if (discoveringDevices == value) return;
                discoveringDevices = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("DiscoveringDevices"));
            }
        }

        public async Task Discover()
        {
            DiscoveringDevices = true;
            try
            {
                var adapter = CrossBluetoothLE.Current.Adapter;
                adapter.ScanTimeout = 3000;
                adapter.DeviceDiscovered += (x, y) =>
                {
                    if (Devices.All(z => z.Device.Id != y.Device.Id))
                    {
                        Devices.Add(new DeviceViewModel(y.Device));
                    }         
                };
                adapter.ScanTimeoutElapsed += (x, y) => DiscoveringDevices = false;
                await adapter.StartScanningForDevicesAsync();
            }
            catch (Exception ex)
            {
                //todo error
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
