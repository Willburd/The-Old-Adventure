using System.Numerics;
using Silk.NET.Input;

namespace Engine
{
    public partial class Core
    {
        /// <summary>
        /// Override during OnInit() if your adventure requires an extended InputHandler()
        /// </summary>
        protected InputHandler input_handler = new InputHandler();

        private static void HandleKeyDown(IKeyboard keyboard, Key key, int keyCode)
        {
            singleton?.input_handler.InvokeKeyPressed(keyboard, key, keyCode);
        }

        private static void HandleKeyUp(IKeyboard keyboard, Key key, int keyCode)
        {
            singleton?.input_handler.InvokeKeyReleased(keyboard, key, keyCode);
        }

        private static void HandleMouseWheel(IMouse mouse, ScrollWheel scrollWheel)
        {
            
        }

    }
}