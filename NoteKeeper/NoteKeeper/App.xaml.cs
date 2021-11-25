using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using NoteKeeper.Services;
using NoteKeeper.Views;

namespace NoteKeeper
{
    public partial class App : Application
    {

        public App()
        {
            InitializeComponent();

            DependencyService.Register<MockDataStore>();
            //MainPage sets the start page of the app at launch
            MainPage = new NavigationPage(new ItemsPage());
        }

        protected override void OnStart()
        {

        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }
    }
}
