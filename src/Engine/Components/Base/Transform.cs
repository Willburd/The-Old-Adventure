using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that holds a transformed position in 3D space
    /// </summary>
    public class Transform(Entity host_entity) : EntComponent(host_entity)
    {
        private Vector3 position;
        private Vector3 last_position;

        

        public Vector3 Position
        {
            get {return position;}
            set {position = value;}
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
                    last_position = position;
                    return 1;
            }
            return base.ReceiveSignal(signal, args);
        }
    }
}