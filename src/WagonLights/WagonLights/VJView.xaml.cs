using WagonLights.ViewModels;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class VJView : ContentPage
	{
	    public VJView()
	    {
	        InitializeComponent();
	        var vm = new VJViewModel();
	        BindingContext = vm;
	        Items.ItemTapped += (x, y) => vm.SetVJ();
        }
    }
}