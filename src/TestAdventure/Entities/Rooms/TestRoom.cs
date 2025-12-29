using Engine;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadActors()
        {
            new TestActor( new Transform( new Vector3(0f,-0.5f,0f)), this);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}