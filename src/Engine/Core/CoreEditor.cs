
namespace Engine
{
    public partial class Core
    {
        public static bool EditorMode { get; set; } = false; // TODO - UI for entering editor mode
        public static bool EditorAllowsUpdates { get; set; } = false; // TODO - Ui for pausing and unpausing update tick in editor

        // TODO - Editor support functions, such as reloading all assets/actors/room
    }
}