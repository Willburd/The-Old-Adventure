
namespace Engine
{
    public partial class Core 
    {
        public static bool EditorMode { get; set; } = true; // TODO - UI for entering editor mode
        public static bool EditorAllowsUpdates { get; set; } = true; // TODO - Ui for pausing and unpausing update tick in editor

        // TODO - Editor support functions, such as reloading all assets/actors/room
    }
}