using System.Numerics;
using EntComponents;
using Assets;
using Engine.ColliderShapes;

namespace Engine
{
    public class Room : Entity
    {
        public static List<Room> loaded_rooms = [];

        public bool UnloadOnEnd { get; private set; } = true;
        public bool RemoveActorsOnEnd { get; private set; } = true;

        /// <summary>
        /// Environment controller for the room, used to set fog, light properties, skybox, and other effects not stored in the room itself. So that a room may use different environments if it needs to.
        /// </summary>
        public EnvironmentData? Environment { get; set; } = null;

        /// <summary>
        /// Environment controller override. Used when something needs to temporarily take control of the environment without permanently changing it. Needs to handle it's own fade in and out.
        /// </summary>
        public EnvironmentData? TempEnvironmentOverride { get; set; } = null;

        public List<Actor> ActorList { get; private set; } = [];
        public Dictionary<string, Actor> ActorLookupList { get; private set; } = [];

        public Room(string room_id) : base(Transform.Identity, room_id, "Engine::Room")
        {
            Console.WriteLine("=====================================================");
            Console.WriteLine("=======> Room Loading : " + GetType());
            if (Core.EditorMode)
            {
                // Meant for debugging scenes
                new EditorCamera(new Transform(new Vector3(0f, 0f, 0f)), room_id + "_editorcam", true, this);
            }
            else
            {
                // standard camera
                new PlayerCamera(new Transform(new Vector3(0f, 0f, 0f)), room_id + "_cam", true, this);
            }
            loaded_rooms.Add(this);
            // All scenes implicitly have these
            WorldRender renderer = new WorldRender(this);
            Collider terrain_collider = new Collider(this);
            // Setup room
            LoadAssets();
            LoadActors();
            LoadExits();
            Console.WriteLine("-------> Room Loaded : " + room_id);
            // Environment
            Environment?.ApplyEnvironment(this);
            MinimumRenderDistance = float.PositiveInfinity; // Do not hide room geometry
            // Collision from render mesh
            MeshData? mesh = renderer.GetMeshByName("col.001");
            if (mesh != null)
            {
                terrain_collider.SetShape(new ColliderShapes.WorldGeometryCol(mesh));
                terrain_collider.CollisionMask = Collider.mask_worldgeo;
                Console.WriteLine("-------> Collision mesh created : " + (terrain_collider.CollisionShape as WorldGeometryCol).MeshTriCount() + " tris");
            }
            // Console.WriteLine("-------> Pathfinding mesh created : " + room_id); // TODO 
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


        public override void OnCleanup()
        {
            Console.WriteLine("-----------------------------------------------------");
            Console.WriteLine("-------> Room unloading : " + GetType());
            // Lets handle special logic
            SendGlobalSignal(Core.Signals.global_room_unloaded);
            // Lets clean up our room, and anything tied to us.
            if (UnloadOnEnd || RemoveActorsOnEnd)
            {
                List<Actor> cleanup_list = [.. ActorList];
                foreach (Actor act in cleanup_list)
                {
                    act.Destroy();
                }
                ActorList.Clear();
            }
            // If we unload our assets and not just actors, do that too
            TempEnvironmentOverride?.Unload();
            TempEnvironmentOverride = null;
            Environment?.Unload();
            Environment = null;
            if (UnloadOnEnd) AssetLoader.UnloadAllAssets(false);
            loaded_rooms.Remove(this);
            Console.WriteLine("=======X Room Unloaded : " + GetType());
            Console.WriteLine("=====================================================");
        }
    }
}