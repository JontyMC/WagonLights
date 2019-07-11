using System.Collections.ObjectModel;
using System.ComponentModel;

namespace WagonLights.ViewModels
{
    public class VJViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<ProgramViewModel> Items { get; set; } = new ObservableCollection<ProgramViewModel>(new[]
        {
            new ProgramViewModel { Id = 0, Name = "Strobe"},
            new ProgramViewModel { Id = 1, Name = "Pulse"},
            new ProgramViewModel { Id = 2, Name = "White"},
            new ProgramViewModel { Id = 3, Name = "Red"},
            new ProgramViewModel { Id = 4, Name = "Green"},
            new ProgramViewModel { Id = 5, Name = "Blue"},
            new ProgramViewModel { Id = 6, Name = "Yellow"},
            new ProgramViewModel { Id = 7, Name = "Orange"},
            new ProgramViewModel { Id = 8, Name = "Pink"},
            new ProgramViewModel { Id = 9, Name = "Purple"}
        });

        ProgramViewModel selectedItem;
        public ProgramViewModel SelectedItem
        {
            get => selectedItem;
            set
            {
                if (selectedItem == value) return;
                selectedItem = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SelectedItem"));
            }
        }

        public void SetVJ()
        {
            App.Wagon.SetVJ(SelectedItem.Id);
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
