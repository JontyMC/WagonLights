using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace WagonLights
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class WagonStatus : Grid
	{
		public WagonStatus()
		{
			InitializeComponent();
		}
	}
}