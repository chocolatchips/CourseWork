using System;
using Xamarin.Forms;

using NoteKeeper.Models;
using NoteKeeper.ViewModels;

namespace NoteKeeper.Views
{
    public partial class ItemsPage : ContentPage
    {
        ItemsViewModel _viewModel;

        public ItemsPage()
        {
            InitializeComponent();
            
            BindingContext = _viewModel = new ItemsViewModel();
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();
            //if (_viewModel.Notes.Count == 0)
                _viewModel.LoadItemsCommand.Execute(null);
        }


        async void OnItemSelected(object sender, SelectedItemChangedEventArgs args)
        {
            var item = args.SelectedItem as Item;
            if (item == null)
                return;

            await Navigation.PushAsync(new ItemDetailPage(new ItemDetailViewModel(item)));

            ItemsListView.SelectedItem = null;
        }


        async void AddItem_Clicked(object sender, EventArgs e)
        {
            await Navigation.PushModalAsync(new NavigationPage(new ItemDetailPage()));
        }

        async void DeleteItem_Clicked(object sender, EventArgs e)
        {
            
        }

    }
}
