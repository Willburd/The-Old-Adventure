using System.Numerics;
using EntComponents;
using System.Drawing;

namespace Engine
{
    public class Room : Entity
    {
        public static List<Room> loaded_rooms = [];

        public bool UnloadOnEnd {get; private set;} = true;
        public bool RemoveActorsOnEnd {get; private set;} = true;

        public Environments.Environment Environment { get; set; } = new( Tools.ColorToVector(Color.CornflowerBlue), 250f, new Vector4(1f,1f,1f,1f));

        public List<Actor> ActorList {get; private set;} = [];

        public Room() : base(Transform.Identity, "Engine::Room")
        {
            Console.WriteLine("=====================================================");
            Console.WriteLine("=======> Room Loading : " + GetType());
            if(Core.EditorMode) 
            {
                // Meant for debugging scenes
                new EditorCamera(new Transform( new Vector3(0f,0f,0f)), true, this);
            }
            else
            {
                // standard camera
                new Camera(new Transform( new Vector3(0f,0f,0f)), true, this);
            }
            loaded_rooms.Add(this);
            // All scenes implicitly have these
            new WorldRender(this);
            new Collider(this); 
            // Setup room
            LoadAssets();
            LoadActors();
            LoadExits();
            MinimumRenderDistance = float.PositiveInfinity; // Do not hide room geometry
            Console.WriteLine("-------> Room Loaded : " + GetType());
            Console.WriteLine("-----------------------------------------------------");
        }

        public readonly List<Type> exit_list = [];

        /// <summary>
        /// Loads assets for the room itself, such as textures, materials, and even setting the room's render model.
        /// </summary>
        public virtual void LoadAssets() { }
        
        /// <summary>
        /// Called setup, used to spawn actors based on the current game's story information, time of day, or other logic.
        /// </summary>
        public virtual void LoadActors() { }
        
        /// <summary>
        /// Called setup, used to assign exit_list types to spawn when that particular exit is used. Can also be used to spawn exit actors.
        /// </summary>
        public virtual void LoadExits() { }
        
        /// <summary>
        /// Called during the update loop, when the room is Enabled
        /// </summary>
        public virtual void OnRoomUpdate() { }
        
        /// <summary>
        /// Called during the update loop, but only when the room is not Enabled
        /// </summary>
        public virtual void OnRoomDisabledUpdate() { }
        

        protected override void OnCleanup()
        {
            Console.WriteLine("-----------------------------------------------------");
            Console.WriteLine("-------> Room unloading : " + GetType());
            // Lets handle special logic
            SendGlobalSignal(Core.Signals.global_room_unloaded);
            // Lets clean up our scene, and anything tied to us.
            if(UnloadOnEnd || RemoveActorsOnEnd)
            {
                List<Actor> cleanup_list = [.. ActorList]; 
                foreach(Actor act in cleanup_list)
                {
                    act.Destroy();
                }
                ActorList.Clear();
            }
            // If we unload our assets and not just actors, do that too 
            if(UnloadOnEnd) AssetLoader.UnloadAllAssets(false);
            loaded_rooms.Remove(this);
            Console.WriteLine("=======X Room Unloaded : " + GetType());
            Console.WriteLine("=====================================================");
        }
    }
}