﻿using System;
using System.Threading.Tasks;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Exceptions;
 using Xamarin.Forms;
 using IDevice = Plugin.BLE.Abstractions.Contracts.IDevice;

namespace WagonLights
{
    public partial class MainPage : TabbedPage
    {
        public MainPage()
        {
            InitializeComponent();
        }
    }
}