using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using NoteKeeper.Models;

namespace NoteKeeper.Services
{
    public class MockNoteDataStore : INoteDataStore
    {
        private static readonly List<String> mockCourses;
        private static readonly List<Note> mockNotes;
        private static int nextNoteId;

        static MockNoteDataStore()
        {
            mockCourses = new List<string>
            {
                "",
                "Intro to Xamarin.Forms",
                "Android Apps",
                "Managing Android App Data"
            };

            mockNotes = new List<Note>
            {
                new Note {Id = "0", Heading = "UI Code", Text = "Xamarin.Forms UI code shared", Course = "Intro to Xamarin.Forms"} };
        }

        public async Task<bool> AddNoteAsync(Note courseNote)
        {
            mockNotes.Add(courseNote);

            return await Task.FromResult(true);
        }

        public async Task<IList<string>> GetCoursesAsync()
        {
            return await Task.FromResult(mockCourses);
        }

        public async Task<Note> GetNoteAsync(string id)
        {
            return await Task.FromResult(mockNotes.FirstOrDefault(s => s.Id == id));

        }

        public async Task<IList<Note>> GetNotesAsync()
        {
            return await Task.FromResult(mockNotes);
        }

        public async Task<bool> UpdateNoteAsync(Note courseNote)
        {
            var oldNote = mockNotes.Where((Note arg) => arg.Id == courseNote.Id).FirstOrDefault();
            mockNotes.Remove(oldNote);
            mockNotes.Add(courseNote);

            return await Task.FromResult(true);
        }

        public async Task<bool> DeleteItemAsync(string id)
        {
            var oldNote = mockNotes.Where((Note arg) => arg.Id == id).FirstOrDefault();
            mockNotes.Remove(oldNote);

            return await Task.FromResult(true);
        }
    }
}
