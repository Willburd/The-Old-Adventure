using System.Numerics;
using EntComponents;
using Assets;
using Engine.ColliderShapes;
using EntComponents.ActorBehavior;
using EntComponents.Cutscenes;

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

        public Room(string room_id, EntranceType entrance_used) : base(Transform.Identity, room_id, "Engine::Room")
        {
            // Specific entrance controls what happens when we enter
            EntranceUsed = entrance_used;
        }

        public void RoomInit()
        {
            Console.WriteLine("=====================================================");
            Console.WriteLine("=======> Room Loading : " + GetType());
            // All scenes implicitly have these
            if (Core.EditorMode)
            {
                // Meant for debugging scenes
                new EditorCamera(new Transform(new Vector3(0f, 0f, 0f)), EntityID + "_editorcam", true, this);
            }
            else
            {
                // standard camera
                new PlayerCamera(new Transform(new Vector3(0f, 0f, 0f)), EntityID + "_cam", true, this);
            }
            WorldRender renderer = new WorldRender(this);
            Collider terrain_collider = new Collider(this);

            // Setup room
            LoadAssets();
            LoadActors();
            LoadExits();
            LoadPlayer(EntranceUsed);
            Console.WriteLine("-------> Room Loaded : " + EntityID);

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
            loaded_rooms.Add(this);
        }

        public EntranceType EntranceUsed { get; private set; }

        /// <summary>
        /// Helper for spawning a room exit trigger.
        /// </summary>
        public void CreateExitTrigger(string exit_name, int use_index, Vector3 position, Vector2 size, RoomExit exit_destination)
        {
            Actor exit = EntityFactory.CreateActor(exit_name, "room_exit", new Transform(position, Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);
            RoomExitBehavior behavior = (RoomExitBehavior)exit.GetComponent(typeof(RoomExitBehavior));
            behavior.ExitID = use_index;
            behavior.Size = size;
            behavior.ExitDestination = exit_destination;
        }
        
        /// <summary>
        /// Helper for spawning a player.
        /// </summary>
        public void CreatePlayer(Vector3 position, Quaternion rotation)
        {
            EntityFactory.CreateActor(PlayerActorBehavior.player_actor_id, "actor_player", new Transform(position, rotation, Vector3.One), this, AssetLoader.AssetSource.engine);
        }

        /// <summary>
        /// Loads assets for the room itself, such as textures, materials, and even setting the room's render model.
        /// </summary>
        public virtual void LoadAssets() { }

        /// <summary>
        /// Called during setup, used to spawn actors based on the current game's story information, time of day, or other logic.
        /// </summary>
        public virtual void LoadActors() { }

        /// <summary>
        /// Called during setup, used to assign exit_list types to spawn when that particular exit is used. Can also be used to spawn exit actors.
        /// </summary>
        public virtual void LoadExits() { }

        /// <summary>
        /// Called during setup, used to spawn a player or cutscene when a specific entrance is used.
        /// </summary>
        public virtual void LoadPlayer(EntranceType entrance_used)
        {
            EntityFactory.CreateActor(PlayerActorBehavior.player_actor_id, "actor_player", new Transform(new Vector3(0f, 0f, 0f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);
        }

        /// <summary>
        /// Called during the update loop, when the room is Enabled
        /// </summary>
        public virtual void OnRoomUpdate() { }

        /// <summary>
        /// Called during the update loop, but only when the room is not Enabled
        /// </summary>
        public virtual void OnRoomDisabledUpdate() { }

        /// <summary>
        /// Called when a player actor enters a roomexit, allows special handling for certain room types. Default behavior waits for room transition, unloads the current room, and loads the new room. Returns true if the function has handled the exit.
        /// </summary>
        public virtual bool OnUseExit(RoomExit exit_destination, Entity exit_entity)
        {
            if (!Cutscene.CanStartCutscene) return false; // can't exit, we're in a cutscene
            Console.WriteLine("Exit triggered " + exit_destination.room_goal + " : " + exit_destination.destination);
            CutsceneExitRoom exit_scene = new(this)
            {
                GoalPos = exit_entity.Position,
                ExitData = exit_destination
            };
            return true;
        }

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


        public struct RoomExit(Type roomtype_goal, EntranceType room_entrance)
        {
            public Type room_goal = roomtype_goal;
            public EntranceType destination = room_entrance;
        }


        /// <summary>
        /// Enum of generic room entrances, use these instead of magic numbers to avoid conflicts. Should contain enough variety and common entrance types for most scenes.
        /// </summary>
        public enum EntranceType
        {
            error,
            titlescreen,
            menu,
            newgame,
            death,
            reset,
            failure,
            pit_respawn,

            pathA,
            pathB,
            pathC,
            pathD,

            caveA,
            caveB,
            caveC,
            caveD,

            riverA,
            riverB,
            riverC,
            riverD,

            northA,
            northB,
            northC,
            northD,

            southA,
            southB,
            southC,
            southD,

            eastA,
            eastB,
            eastC,
            eastD,

            westA,
            westB,
            westC,
            westD,

            tunnelA,
            tunnelB,
            tunnelC,
            tunnelD,

            holeA,
            holeB,
            holeC,
            holeD,

            pitA,
            pitB,
            pitC,
            pitD,

            bossA,
            bossB,
            bossC,
            bossD,

            questA,
            questB,
            questC,
            questD,

            warpA,
            warpB,
            warpc,
            warpD,

            minigameA,
            minigameB,
            minigameC,
            minigameD,
            minigameE,
            minigameF,
            minigameG,
            minigameH,

            cutsceneA,
            cutsceneB,
            cutsceneC,
            cutsceneD,
            cutsceneE,
            cutsceneF,
            cutsceneG,
            cutsceneH,

            cutsceneEndA,
            cutsceneEndB,
            cutsceneEndC,
            cutsceneEndD,
            cutsceneEndE,
            cutsceneEndF,
            cutsceneEndG,
            cutsceneEndH,

            debug = 1000
        }
    }
}