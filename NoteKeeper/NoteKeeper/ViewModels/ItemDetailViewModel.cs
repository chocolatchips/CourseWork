using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using NoteKeeper.Models;
using Xamarin.Forms;

namespace NoteKeeper.ViewModels
{
    public class ItemDetailViewModel : BaseViewModel
    {
        public Note Note { get; set; }
        public IList<String> CourseList { get; set; }

        public String NoteHeading
        {
            get { return Note.Heading; }
            set
            {
                Note.Heading = value;
                OnPropertyChanged();
            }
        }

        public String NoteText
        {
            get { return Note.Text; }
            set
            {
                Note.Text = value;
                OnPropertyChanged();
            }
        }

        public String NoteTitle
        {
            get { return Note.Course; }
            set
            {
                Note.Course = value;
                OnPropertyChanged();
            }
        }

        public String NoteId
        {
            get { return Note.Id; }
            set
            {
                Note.Id = value;
                OnPropertyChanged();
            }
        }

        public String NoteCourse
        {
            get { return Note.Course; }
            set
            {
                Note.Course = value;
                OnPropertyChanged();
            }
        }


        public ItemDetailViewModel(Item item = null)
        {
            Title = item?.Text;
            InitializeCourseList();
            Note = new Note { Heading = "Test note", Text = "Text for note", Course= CourseList[0]};
        }

        async void InitializeCourseList()
        {
            CourseList = await NoteDataStore.GetCoursesAsync();
        }

        public async void SaveNote()
        {
            await NoteDataStore.AddNoteAsync(Note);
        }

    }
}
