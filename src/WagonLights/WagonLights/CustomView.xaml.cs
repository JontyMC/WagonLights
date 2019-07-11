using WagonLights.ViewModels;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class CustomView : ContentPage
	{
	    public CustomView()
	    {
	        InitializeComponent();
	        var vm = new CustomViewModel();
	        BindingContext = vm;
	        Add.Clicked += (x, y) => vm.Add();
	        Confirm.Clicked += (x, y) => vm.Confirm();
	        Reset.Clicked += (x, y) => vm.Reset();
        }
    }
}