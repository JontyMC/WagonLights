using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Plugin.BLE;
using WagonLights.ViewModels;

namespace WagonLights
{
    public class DiscoverViewModel
    {
        public ObservableCollection<DeviceViewModel> Devices { get; set; } = new ObservableCollection<DeviceViewModel>();

        public async Task Discover()
        {
            try
            {
                var adapter = CrossBluetoothLE.Current.Adapter;
                adapter.ScanTimeout = 5000;
                adapter.DeviceDiscovered += (x, y) => Devices.Add(new DeviceViewModel(y.Device));
                await adapter.StartScanningForDevicesAsync();
            }
            catch (Exception ex)
            {
                //todo error
            }
        }
    }
}
