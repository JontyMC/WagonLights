using System.Collections.ObjectModel;
using System.ComponentModel;

namespace WagonLights.ViewModels
{
    public class PalettesViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<ProgramViewModel> Palettes { get; set; } = new ObservableCollection<ProgramViewModel>(new[]
        {
            new ProgramViewModel { Id = 0, Name = "LGBT"},
            new ProgramViewModel { Id = 1, Name = "RGB"},
            new ProgramViewModel { Id = 2, Name = "England"},
            new ProgramViewModel { Id = 2, Name = "Sweden"}
        });

        ProgramViewModel selectedPalette;
        public ProgramViewModel SelectedPalette
        {
            get => selectedPalette;
            set
            {
                if (selectedPalette == value) return;
                selectedPalette = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SelectedPalette"));
            }
        }

        public void ChangePalette()
        {
            App.Wagon.SetPalette(SelectedPalette.Id);
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
