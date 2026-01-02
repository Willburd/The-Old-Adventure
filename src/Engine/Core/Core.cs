using Silk.NET.Maths;
using Silk.NET.Windowing;
using System.Diagnostics;

namespace Engine
{
    public partial class Core 
    {
        public static Core? singleton;
        protected const string AdventureTitle = "The Old Adventure";
        protected readonly string adventure_id;
        public static string? AdventureID
        {
            get
            {
                if(singleton == null) return "UnitTest";
                return singleton?.adventure_id;
            }
        }

#pragma warning disable CS8618 // I don't care if you're upset the static constructor doesn't set it. We're doing it on instantilize.
        static Core()
        {
            // Prepare window
            WindowOptions options = WindowOptions.Default with
            {
                Size = new Vector2D<int>(800, 600),
                Title = AdventureTitle
            };

            // Create window
            WindowContext = Window.Create(options);
            WindowContext.Load += HandleWindowLoad;
            WindowContext.Update += HandleWindowUpdate;
            WindowContext.Render += HandleWindowRender;
            WindowContext.Closing += HandleWindowClosing;
        }
#pragma warning restore CS8618

        // Entry point
        public Core(string id = "UnitTest")
        {
            
            adventure_id = id;

            Debug.Assert(singleton == null,"Multiple cores created - " + adventure_id);
            singleton = this;
            Console.WriteLine("CORE INIT - " + AdventureTitle);

            // Start setup with gamespecific preinit.
            OnPreInit();

            // Load actory factory, override in OnPreInit() for your own adventure!
            EntityFactory.InitLibrary();
            
            // Start the window. Everything from here is handled by HandleWindowUpdate() and HandleWindowRender()
            WindowContext.Run();
            // HandleWindowLoad() called from Run()

            // Finished game, end it off.
            WindowContext.Dispose();
        }
    }
}