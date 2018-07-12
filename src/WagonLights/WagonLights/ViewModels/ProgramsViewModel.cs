using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Threading.Tasks;

namespace WagonLights.ViewModels
{
    public class ProgramsViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<ProgramViewModel> Programs { get; set; } = new ObservableCollection<ProgramViewModel>(new []
        {
            new ProgramViewModel { Id = 0, Name = "Show"},
            new ProgramViewModel { Id = 1, Name = "Sound Reactive"},
            new ProgramViewModel { Id = 2, Name = "Solid Color"},
            new ProgramViewModel { Id = 3, Name = "Fade"},
            new ProgramViewModel { Id = 4, Name = "Rainbow"},
            new ProgramViewModel { Id = 5, Name = "Rainbow Dist"},
            new ProgramViewModel { Id = 6, Name = "Color Wipe"},
            new ProgramViewModel { Id = 7, Name = "Chase"},
            new ProgramViewModel { Id = 8, Name = "Off"}
        });

        ProgramViewModel selectedProgram;
        public ProgramViewModel SelectedProgram
        {
            get => selectedProgram;
            set
            {
                if (selectedProgram == value) return;
                selectedProgram = value;

                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SelectedProgram"));
            }
        }

        public void ChangeProgram()
        {
            App.Wagon.SetProgram(SelectedProgram.Id);
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
