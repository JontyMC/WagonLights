using System.Threading.Tasks;
using WagonLights.Wagon;
using Xamarin.Forms;

namespace WagonLights
{
	public partial class App : Application
	{
        public static WagonBluetooth Wagon { get; set; } = new WagonBluetooth();

		public App ()
		{
			InitializeComponent();

			MainPage = new MainPage();
		}

		protected override void OnStart ()
		{
			// Handle when your app starts
		    Task.Run(() => Wagon.Start());
		}

		protected override void OnSleep ()
		{
			// Handle when your app sleeps
		}

		protected override void OnResume ()
		{
			// Handle when your app resumes
		}
	}
}
