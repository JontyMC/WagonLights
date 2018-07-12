using System.ComponentModel;

namespace WagonLights.ViewModels
{
    public class ProgramViewModel : INotifyPropertyChanged
    {
        int id;

        public int Id
        {
            get => id;
            set
            {
                if (id == value) return;
                id = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Id"));
            }
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