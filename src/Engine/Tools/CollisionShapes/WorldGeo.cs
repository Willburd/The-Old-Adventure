using System.Numerics;
using EntComponents;

namespace Engine
{
    public class WorldGeometryCol(Collider host) : ColShape(host)
    {
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            // No implimentation
            return null;
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            // No implimentation
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            // No implimentation
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            // No implimentation
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
        
    }
}