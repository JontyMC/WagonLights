using System.ComponentModel;

namespace WagonLights
{
    public class ConnectViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        bool connecting;
        public bool Connecting
        {
            get => connecting;
            set
            {
                if (connecting == value) return;
                connecting = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Connecting"));
            }
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

        string deviceName;
        public string DeviceName
        {
            get => deviceName;
            set
            {
                if (deviceName == value) return;
                deviceName = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("DeviceName"));
            }
        }

        string error;
        public string Error
        {
            get => error;
            set
            {
                if (error == value) return;
                error = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Error"));
            }
        }
    }
}
