using Engine;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadActors()
        {
            new EditorCamera(new Transform( new Vector3(0f,0f,0f)), true, this);

            EntityFactory.CreateActor( "actor_pointer", new Transform( new Vector3(0f,0f,0f)), this, AssetLoader.AssetSource.engine);
            
            float radius = 6f;
            
            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(0f,-1f,radius), Quaternion.Identity, Vector3.One * 1.5f), this, AssetLoader.AssetSource.engine);
            
            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(radius,-1f,0f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(-radius,-1f,0f), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f)), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(0f,-1f,-radius), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f), Vector3.One * 0.15f), this, AssetLoader.AssetSource.engine);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}