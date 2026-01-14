using System.Numerics;

namespace Engine
{
    /// <summary>
    /// Lightweight class used for holding a position and rotation in 3D space.
    /// </summary>
    public class Transform
    {
        public static Transform Identity
        {
            get
            {
                return new Transform();
            }
        }

        public static Transform Zero
        {
            get
            {
                Transform tra = new Transform();
                tra.Position = Vector3.Zero;
                tra.Rotation = Quaternion.Identity;
                tra.Scale = Vector3.Zero;
                return tra;
            }
        }

        public Transform()
        {
            // All to identity
            Position = Vector3.Zero;
            Rotation = Quaternion.Identity;
            Scale = Vector3.One;
        }

        public Transform(Vector3 position)
        {
            Position = position;
            Rotation = Quaternion.Identity;
            Scale = Vector3.One;
        }

        public Transform(Vector3 position, Vector3 scale)
        {
            Position = position;
            Rotation = Quaternion.Identity;
            Scale = scale;
        }

        public Transform(Quaternion rotation)
        {
            Position = Vector3.Zero;
            Rotation = rotation;
            Scale = Vector3.One;
        }

        public Transform(float anglex, float angley, float anglez)
        {
            Position = Vector3.Zero;
            Rotation = Tools.Degrees(anglex, angley, anglez);
            Scale = Vector3.One;
        }

        public Transform(Vector3 position, Quaternion rotation)
        {
            Position = position;
            Rotation = rotation;
            Scale = Vector3.One;
        }

        public Transform(Vector3 position, Quaternion rotation, Vector3 scale)
        {
            Position = position;
            Rotation = rotation;
            Scale = scale;
        }

        public Vector3 Position { get; set; }

        public Quaternion Rotation { get; set; }

        public Vector3 Scale { get; set; }

        public Matrix4x4 ViewMatrix
        {
            get
            {
                return Matrix4x4.Identity * Matrix4x4.CreateFromQuaternion(Rotation) * Matrix4x4.CreateScale(Scale) * Matrix4x4.CreateTranslation(Position);
            }
        }

        public void Translate(Vector3 translation)
        {
            Position += translation;
        }

        public void Rotate(Quaternion rotation)
        {
            Rotation *= rotation;
        }
        public void Scaler(Vector3 scaler)
        {
            Scale += scaler;
        }

        public void Add(Transform addition)
        {
            Position += addition.Position;
            Rotation *= addition.Rotation;
            Scale += addition.Scale;
        }

        public void Set(Transform assignment)
        {
            Position = assignment.Position;
            Rotation = assignment.Rotation;
            Scale = assignment.Scale;
        }
    }
}