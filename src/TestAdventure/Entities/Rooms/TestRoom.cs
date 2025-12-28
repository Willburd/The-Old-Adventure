using Engine;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadActors()
        {
            float spread = 0.8f;
            new TestActor(Transform.Identity, this);
            new TestActor(new Transform(new Vector3(Tools.RandRange(-spread,spread),-0.5f + Tools.RandRange(-spread,spread),Tools.RandRange(-spread,spread))), this);
            new TestActor(new Transform(new Vector3(Tools.RandRange(-spread,spread),-0.5f + Tools.RandRange(-spread,spread),Tools.RandRange(-spread,spread))), this);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}