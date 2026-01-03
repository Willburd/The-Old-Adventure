using System.Numerics;
using EntComponents;

namespace Engine
{
    public class TriCol(Collider host) : ColShape(host)
    {
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(PlaneCol plane_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(TriCol triangle_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(WorldGeometryCol geometry_col)
        {
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}