using EntComponents;

namespace Engine
{
    public class ColShape
    {
        public virtual Collider.Collision? Overlap(ColShape other)
        {
            return null;
        }
        
        public virtual Collider.RaycastHit? RayIntersect(Collider.Raycast ray)
        {
            return null;
        }
    }

    public class LineCol : ColShape
    {
        
    }

    public class SphereCol : ColShape
    {
        
    }

    public class AxisCubeCol : ColShape
    {
        
    }

    public class CylinderCol : ColShape
    {
        
    }

    public class PlaneCol : ColShape
    {
        
    }

    public class TriCol : ColShape
    {
        
    }
    
    public class WorldGeometryCol : ColShape
    {
        
    }
}