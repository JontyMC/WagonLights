using System;
using System.Threading.Tasks;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Exceptions;
using IDevice = Plugin.BLE.Abstractions.Contracts.IDevice;

namespace WagonLights
{
    public partial class MainPage
    {
        readonly ConnectViewModel viewModel;
        IDevice device;

        public MainPage()
        {
            InitializeComponent();
            viewModel = new ConnectViewModel();
            BindingContext = viewModel;

            //Discover.Clicked += async (x, y) =>
            //{
            //    try
            //    {


            //        viewModel.DiscoveringDevices = true;
            //        CrossBleAdapter.Current.Scan().Subscribe(z =>
            //        {
            //            viewModel.DiscoveringDevices = false;
            //            viewModel.DeviceName = z.Device.Uuid.ToString();
            //        });

            //    }
            //    catch (Exception ex)
            //    {
            //        viewModel.Error = ex.Message;
            //    }
            //}; 

            //Discover.Clicked += async (x, y) => await DiscoverDevicesAction();
        }

        async Task DiscoverDevicesAction()
        {
            try
            {
                viewModel.Error = "";

                viewModel.DiscoveringDevices = true;
                var ble = CrossBluetoothLE.Current;
                var adapter = CrossBluetoothLE.Current.Adapter;
                adapter.ScanTimeout = 5000;
                adapter.DeviceDiscovered += async (x, y) =>
                {
                    viewModel.DiscoveringDevices = false;
                    //device = y.Device;
                    //viewModel.DeviceName = device.Name;
                    //viewModel.Connecting = true;
                    //try
                    //{
                    //    await adapter.ConnectToDeviceAsync(device);
                    //    viewModel.Connecting = false;
                    //}
                    //catch (DeviceConnectionException ex)
                    //{
                    //    // ... could not connect to device
                    //    viewModel.Error = ex.Message;
                    //}
                };
                await adapter.StartScanningForDevicesAsync();
                viewModel.Error = "Timed out";
                viewModel.DiscoveringDevices = false;
            }
            catch (Exception ex)
            {
                viewModel.Error = ex.Message;
            }
        }

        async Task Connect()
        {
            try
            {
                viewModel.Error = "";

                viewModel.DiscoveringDevices = true;
                var ble = CrossBluetoothLE.Current;
                var adapter = CrossBluetoothLE.Current.Adapter;
                adapter.ScanTimeout = 5000;
                adapter.DeviceDiscovered += async (x, y) =>
                {
                    viewModel.DiscoveringDevices = false;
                    //device = y.Device;
                    //viewModel.DeviceName = device.Name;
                    //viewModel.Connecting = true;
                    //try
                    //{
                    //    await adapter.ConnectToDeviceAsync(device);
                    //    viewModel.Connecting = false;
                    //}
                    //catch (DeviceConnectionException ex)
                    //{
                    //    // ... could not connect to device
                    //    viewModel.Error = ex.Message;
                    //}
                };
                await adapter.StartScanningForDevicesAsync();
                viewModel.Error = "Timed out";
                viewModel.DiscoveringDevices = false;
            }
            catch (Exception ex)
            {
                viewModel.Error = ex.Message;
            }
        }
    }
}
