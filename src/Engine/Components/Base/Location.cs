using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that holds the location in 3D space of an entity.
    /// </summary>
    public class WorldLocation(Entity host_entity) : EntComponent(host_entity)
    {
        protected readonly Transform transform = new();     
        protected readonly Transform last_transform = new();  



        public Vector3 Position
        {
            get {return transform.Position;}
            set {transform.Position = value;}
        }

        public Quaternion Rotation
        {
            get {return transform.Rotation;}
            set {transform.Rotation = value;}
        }

        public Vector3 Scale
        {
            get {return transform.Scale;}
            set {transform.Scale = value;}
        }


        /// <summary>
        /// Sets the last position of the transform to the current position. Preventing the renderer from interpolating the object from one position to another over long distances, such as teleporting.
        /// </summary>
        public void SnapTransform()
        {
            last_transform.Position = transform.Position;
            last_transform.Rotation = transform.Rotation;
            last_transform.Scale = transform.Scale;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Interpolation helpers for rendering
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         
        public Vector3 GetInterpolatedPosition(double tick_delta)
        {
            return Vector3.Lerp( last_transform.Position, transform.Position, (float)tick_delta);
        }

        public Quaternion GetInterpolatedRotation(double tick_delta)
        {
            return Quaternion.Lerp( last_transform.Rotation, transform.Rotation, (float)tick_delta);
        }
        
        public Vector3 GetInterpolatedScale(double tick_delta)
        {
            return Vector3.Lerp( last_transform.Scale, transform.Scale, (float)tick_delta);
        }

        public Matrix4x4 GetViewMatrix()
        {
            return GetInterpolatedViewMatrix(1);
        }
        public Matrix4x4 GetLastViewMatrix()
        {
            return GetInterpolatedViewMatrix(0);
        }

        public Matrix4x4 GetInterpolatedViewMatrix(double tick_delta)
        {
            return Matrix4x4.Identity * Matrix4x4.CreateFromQuaternion(GetInterpolatedRotation(tick_delta)) * Matrix4x4.CreateScale(GetInterpolatedScale(tick_delta)) * Matrix4x4.CreateTranslation(GetInterpolatedPosition(tick_delta));
        } 

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.prepare_transform];
        }

        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.prepare_transform:
                    // We are done interpolating. If nothing updates our position again we'll already be at our new location.
                    SnapTransform();
                    return 1;
            }
            return base.ReceiveSignal(signal, args);
        }
    }
}