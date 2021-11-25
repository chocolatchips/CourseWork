using System.ComponentModel;
using Xamarin.Forms;
using NoteKeeper.ViewModels;
using System;
using NoteKeeper.Models;
using System.Collections.Generic;
using NoteKeeper.Services;

namespace NoteKeeper.Views
{
    public partial class ItemDetailPage : ContentPage
    {
        ItemDetailViewModel viewModel;
        public Note Note { get; set; }

        public ItemDetailPage(ItemDetailViewModel viewModel)
        {
            InitializeComponent();
            this.viewModel = viewModel;
            BindingContext = this.viewModel;
        }

        public ItemDetailPage()
        {
            InitializeComponent();
            viewModel = new ItemDetailViewModel();
            BindingContext = viewModel;
        }

        public async void Cancel_Clicked(object sender, EventArgs eventArgs)
        {
            await Navigation.PopModalAsync();
        }

        public async void Save_Clicked(object sender, EventArgs eventArgs)
        {
            
            bool answer = await DisplayAlert("Alert", "Would you like to save current note?", "Yes", "No");
            if (answer)
            {
                viewModel.SaveNote();
                await Navigation.PopModalAsync();
            }
        }

    }
}
