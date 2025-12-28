
namespace Engine
{
    public class Room : Entity
    {
        public static List<Room> loaded_rooms = [];

        public bool UnloadOnEnd {get; private set;} = true;
        public bool RemoveActorsOnEnd {get; private set;} = true;

        public List<Actor> ActorList {get; private set;} = [];

        public Room(Transform initial_location) : base(initial_location)
        {
            Console.WriteLine("=====================================================");
            Console.WriteLine("=======> Room Loading : " + GetType());
            loaded_rooms.Add(this);
            LoadAssets();
            LoadActors();
            Console.WriteLine("-------> Room Loaded : " + GetType());
            Console.WriteLine("-----------------------------------------------------");
        }

        public virtual void LoadAssets()
        {
            
        }

        public virtual void LoadActors()
        {
            
        }
        
        public virtual void OnRoomUpdate()
        {
        }

        public virtual void OnRoomDisabledUpdate()
        {
        }

        protected override void OnCleanup()
        {
            Console.WriteLine("-----------------------------------------------------");
            Console.WriteLine("=-------> Room unloading : " + GetType());
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