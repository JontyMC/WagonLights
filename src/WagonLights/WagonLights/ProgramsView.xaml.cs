using System;
using WagonLights.ViewModels;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class ProgramsView : ContentPage
	{
		public ProgramsView()
		{
			InitializeComponent ();
		    var vm = new ProgramsViewModel();
		    BindingContext = vm;
		    Programs.ItemTapped += (x, y) => vm.ChangeProgram();
		}
	}
}