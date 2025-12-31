using Engine;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadActors()
        {
            new Camera( new Transform( new Vector3(0f,0f,0f)), true);

            new TestActor( new Transform( new Vector3(0f,-0.5f,0f), Tools.Euler(0f,90f,0f)), this);
            new CubeActor( new Transform( new Vector3(0f,-1f,0f), Quaternion.Identity, Vector3.One), this);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}