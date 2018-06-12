using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class Discover : ContentPage
	{
		public Discover()
		{
			InitializeComponent ();
            var vm = new DiscoverViewModel();
            BindingContext = vm;
		    Task.Run(() => vm.Discover());
		}
	}
}