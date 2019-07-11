using System;
using WagonLights.ViewModels;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class SettingsView : ContentPage
	{
		public SettingsView()
		{
			InitializeComponent ();
		    var vm = new ProgramsViewModel();
		    BindingContext = vm;
		    Brightness.ValueChanged += (x, y) => App.Wagon.SetBrightness(Convert.ToInt32(y.NewValue));
		    SpeedConfirm.Clicked += (x, y) => App.Wagon.SetSpeed(Convert.ToInt32(Speed.Text));
		    WidthConfirm.Clicked += (x, y) => App.Wagon.SetWidth(Convert.ToInt32(Width.Text));
		    Reset.Clicked += (x, y) => App.Wagon.Reset();
        }
	}
}