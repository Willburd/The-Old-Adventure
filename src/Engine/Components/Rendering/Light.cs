using System.Drawing;
using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that influences vertex lights.
    /// </summary>
    public class Light(Entity host_entity) : EntComponent(host_entity)
    {

        public Vector4 Color { get; set; } = new Vector4(1f, 1f, 1f, 1f);

        /// <summary>
        /// Radius of light.
        /// </summary>
        public float Radius { get; set; } = 1f;

        /// <summary>
        /// Offset vector for the following collision transform vars
        /// </summary>
        private Vector3 light_offset = Vector3.Zero;

        /// <summary>
        /// If true, the Offset getter will return a relative transformation of this vector, including rotation, from the host entity when getting the collider's Position. If false it will be the world position.
        /// </summary>
        public bool SyncRelativePosition = true;

        /// <summary>
        /// Gets the origin position of the collider. If SyncRelativePosition is true it will be tied to the host's position and rotation in world space, if false it will be a untransformed world position.
        /// </summary>
        public Vector3 OffsetPos
        {
            get
            {
                if (!SyncRelativePosition) return light_offset; // Use world position
                return Host.Position + Vector3.Transform(light_offset, Host.Rotation); // Use relative position, including rotation from host.
            }

            set
            {
                light_offset = value;
            }
        }

        public void SetData(Vector4 color, float radius = 1f, Vector3 offset = new Vector3(), bool relative = true)
        {
            OffsetPos = offset;
            Radius = radius;
            OffsetPos = offset;
            SyncRelativePosition = relative;
        }
    }
}