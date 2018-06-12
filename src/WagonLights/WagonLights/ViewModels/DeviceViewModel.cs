using System;
using System.ComponentModel;
using Plugin.BLE.Abstractions.Contracts;

namespace WagonLights.ViewModels
{
    public class DeviceViewModel : INotifyPropertyChanged
    {
        IDevice device;

        public DeviceViewModel(IDevice device)
        {
            this.device = device;
            Name = device.Name ?? "Unknown";
        }

        string name;

        public string Name
        {
            get => name;
            set
            {
                if (name == value) return;
                name = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Name"));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}