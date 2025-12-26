using System.Numerics;

namespace Engine
{
    /// <summary>
    /// Lightweight class used for holding a position and rotation in 3D space.
    /// </summary>
    public class Location
    {
        public Vector3 position;
        public Quaternion rotation;

        public Vector3 Position
        {
            get {return position;}
            set {position = value;}
        }

        public Quaternion Rotation
        {
            get {return rotation;}
            set {rotation = value;}
        }
    }
}