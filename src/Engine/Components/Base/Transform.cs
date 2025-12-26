using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that holds the location in 3D space of an entity.
    /// </summary>
    public class Transform(Entity host_entity) : EntComponent(host_entity)
    {
        protected readonly Tools.Location location = new();     
        protected readonly Tools.Location last_location = new();  


        public Vector3 Position
        {
            get {return location.Position;}
            set {location.Position = value;}
        }

        public Quaternion Rotation
        {
            get {return location.Rotation;}
            set {location.Rotation = value;}
        }


        /// <summary>
        /// Sets the last position of the transform to the current position. Preventing the renderer from interpolating the object from one position to another over long distances, such as teleporting.
        /// </summary>
        public void SnapTransform()
        {
            last_location.Position = location.Position;
            last_location.Rotation = location.Rotation;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Interpolation helpers for rendering
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         
        public Vector3 GetInterpolatedPosition(double tick_delta)
        {
            return Vector3.Lerp( last_location.Position, location.Position, (float)tick_delta);
        }

        public Quaternion GetInterpolatedRotation(double tick_delta)
        {
            return Quaternion.Slerp( last_location.Rotation, location.Rotation, (float)tick_delta);
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