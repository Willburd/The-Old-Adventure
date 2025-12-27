using System.Numerics;

namespace Engine
{
    /// <summary>
    /// Lightweight class used for holding a position and rotation in 3D space.
    /// </summary>
    public class Transform
    {
        public Vector3 Scale { get; set; } = Vector3.One;

        public Vector3 Position { get; set; } = Vector3.Zero;

        public Quaternion Rotation { get; set; } = Quaternion.Identity;
        
        public Matrix4x4 ViewMatrix
        {
            get
            {
                return Matrix4x4.Identity * Matrix4x4.CreateFromQuaternion(Rotation) * Matrix4x4.CreateScale(Scale) * Matrix4x4.CreateTranslation(Position);
            }
        } 
    }
}