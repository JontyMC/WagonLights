using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;

namespace WagonLights.ViewModels
{
    public class CustomViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<ProgramViewModel> DefaultColors { get; set; } = new ObservableCollection<ProgramViewModel>(new[]
        {
            new ProgramViewModel { Id = 0, Name = "White"},
            new ProgramViewModel { Id = 1, Name = "Red"},
            new ProgramViewModel { Id = 2, Name = "Green"},
            new ProgramViewModel { Id = 3, Name = "Blue"},
            new ProgramViewModel { Id = 4, Name = "Yellow"},
            new ProgramViewModel { Id = 5, Name = "Orange"},
            new ProgramViewModel { Id = 6, Name = "Pink"},
            new ProgramViewModel { Id = 7, Name = "Purple"},
            new ProgramViewModel { Id = 8, Name = "Off"},
        });

        public ObservableCollection<ProgramViewModel> SelectedColors { get; set; } =
            new ObservableCollection<ProgramViewModel>();

        ProgramViewModel selectedDefaultColor;
        public ProgramViewModel SelectedDefaultColor
        {
            get => selectedDefaultColor;
            set
            {
                if (selectedDefaultColor == value) return;
                selectedDefaultColor = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SelectedDefaultColor"));
            }
        }
        public void Reset()
        {
            SelectedColors.Clear();
        }

        public void Add()
        {
            if (SelectedColors.Count < 10)
            {
                SelectedColors.Add(SelectedDefaultColor);
            }
        }

        public void Confirm()
        {
            App.Wagon.SetCustomPalette(SelectedColors.Select(x => x.Id).ToArray());
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
