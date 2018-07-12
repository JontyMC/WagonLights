using WagonLights.ViewModels;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class PalettesView : ContentPage
	{
	    public PalettesView()
	    {
	        InitializeComponent();
	        var vm = new PalettesViewModel();
	        BindingContext = vm;
	        Palettes.ItemTapped += (x, y) => vm.ChangePalette();
        }
    }
}