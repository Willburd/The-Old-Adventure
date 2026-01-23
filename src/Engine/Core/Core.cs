using Silk.NET.Maths;
using Silk.NET.Windowing;
using System.Diagnostics;
using System.Numerics;

namespace Engine
{
    public partial class Core
    {
        public static Core singleton;
        protected const string AdventureTitle = "The Old Adventure";
        protected readonly string adventure_id;
        public static string AdventureID
        {
            get
            {
                if (singleton == null) return "UnitTest";
                return singleton.adventure_id;
            }
        }

        public static float DisplayAspectRatio = 1.333333f;
        public static uint DisplayHeight { get; set; } = 800;
        public static uint DisplayWidth
        {
            get
            {
                return GetAspectWidth(DisplayHeight);
            }
        }
        public static uint GetAspectWidth(uint height)
        {
            return (uint)(height * DisplayAspectRatio);
        }

        /// <summary>
        /// Used to convert string characters to font atlas UV positions
        /// </summary>
        static Dictionary<char, Vector2> decode = [];

        /// <summary>
        /// Character sequence that matches the font atlas
        /// </summary>
        const string seq = " ☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{¦}~⌂ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒáíóúñÑªº¿⌐¬½¼¡«»░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀αßΓπΣσµτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■□";

#pragma warning disable CS8618 // I don't care if you're upset the static constructor doesn't set it. We're doing it on instantilize.
        static Core()
        {
            // Prepare window
            WindowOptions options = WindowOptions.Default with
            {
                Size = new Vector2D<int>((int)DisplayWidth, (int)DisplayHeight),
                Title = AdventureTitle
            };

            // Create window
            WindowContext = Window.Create(options);
            WindowContext.Load += HandleWindowLoad;
            WindowContext.Update += HandleWindowUpdate;
            WindowContext.Render += HandleWindowRender;
            WindowContext.Closing += HandleWindowClosing;
            WindowContext.Resize += HandleWindowResize;
            
            // Setup text render data
            int tex_col_count = 16;
            for (int index = 0; index < seq.Length; index++)
            {
                decode.Add(seq[index], new((float)index % tex_col_count / tex_col_count, 1f - (MathF.Floor(index / tex_col_count) / tex_col_count) - (1f / tex_col_count)));
            }
        }
#pragma warning restore CS8618

        // Entry point
        public Core(string id = "UnitTest")
        {

            adventure_id = id;

            Debug.Assert(singleton == null, "Multiple cores created - " + adventure_id);
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

            // Setup batching
            ThreadPool.GetMaxThreads(out int total_available_threads, out int max_asyncthread_count);
            BatchSize = max_asyncthread_count - 1; // incase we're running on something with more limited threads
            // We never want to overrun our task pool, otherwise we'll hit the dreaded 0.5 second reschedual in a gametick.
        }

        // Threading batch control
        public static int BatchSize { get; private set; }

        /// <summary>
        /// Batch tasks to a certain count. Called during loops to await each batch. Prevents overloading threadpool.
        /// </summary>
        public static void AwaitCurrentBatch(List<Task> thread_batch)
        {
            if (thread_batch.Count == 0) return;
            Task.WaitAll(thread_batch);
            thread_batch.Clear();
        }

        private static bool shutting_down = false;
        public static void RequestShutdown()
        {
            shutting_down = true;
        }
    }
}