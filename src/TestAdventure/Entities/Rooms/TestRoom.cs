using Engine;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadActors()
        {
            new Camera( new Transform( new Vector3(0f,0f,0f)), true);

            float radius = 6f;

            new PointerActor( new Transform( new Vector3(0f,-0.5f,radius), Tools.Euler(0f,90f,0f)), this);
            new CubeActor( new Transform( new Vector3(0f,-1f,radius), Quaternion.Identity, Vector3.One), this);
            
            new CubeActor( new Transform( new Vector3(-radius,-1f,0f), Quaternion.Identity, Vector3.One), this);
            
            new CubeActor( new Transform( new Vector3(radius,-1f,0f), Quaternion.Identity, Vector3.One), this);
            
            new CubeActor( new Transform( new Vector3(0f,-1f,-radius), Quaternion.Identity, Vector3.One), this);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}