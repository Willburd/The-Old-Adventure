using System.Numerics;
using Silk.NET.Input;

namespace Engine
{
    public partial class Core
    {
        public static Vector2 RawMousePos { get; private set; }
        public static Vector2 RawMouseDelta { get; private set; }
        private static Vector2 raw_old_mouse_pos = Vector2.Zero;

        /// <summary>
        /// Handles raw mouse input from the window's action signal. See MouseUpdate() for mouse input that is game tick updated.
        /// </summary>
        private static void HandleMouseRawUpdate(IMouse mouse, Vector2 position)
        {
            RawMousePos = position;
            RawMouseDelta = RawMousePos - raw_old_mouse_pos;
            raw_old_mouse_pos = RawMousePos;
        }
    }
}