using System.Threading.Tasks;
using Acr.UserDialogs;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class Programs : ContentPage
	{
		public Programs ()
		{
			InitializeComponent ();
		    AlertButton.Clicked += async (x, y) =>
		    {
		        using (UserDialogs.Instance.Loading("Connecting..."))
		        {
		            await Task.Delay(5000);
		        }
		    };
		}
	}
}