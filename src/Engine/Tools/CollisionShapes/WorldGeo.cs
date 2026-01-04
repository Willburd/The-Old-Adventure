using System.Numerics;
using EntComponents;

namespace Engine
{
    public class WorldGeometryCol(Collider host) : ColShape(host)
    {
        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}