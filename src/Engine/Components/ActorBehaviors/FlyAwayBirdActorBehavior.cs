using Engine;
using Silk.NET.Input;
using System.Numerics;

namespace EntComponents.ActorBehavior
{
    public class FlyAwayBirdActorBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        private bool flying_away = false;
        private Vector3 fly_destination;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.cache_components, Core.Signals.update, Core.Signals.trigger];
        }

        protected override uint HandleAssetLoad()
        {

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Pick a direction to fly to, but stay disabled until requested to activate.
            fly_destination = new Vector3(MathF.Round(Tools.RandRange(-1,1)) * 1000f, 60f, MathF.Round(Tools.RandRange(-1,1)) * 1000f);
            if(fly_destination.X == fly_destination.Z) fly_destination.X = -1000;

            return 1;
        }

        protected override uint HandleCacheComponents()
        {
            TriggerVolume? trigger = (TriggerVolume?)Host.GetComponent(typeof(TriggerVolume));
            
            // TODO - Set trigger volume size

            return 1;
        }

        protected override uint HandleUpdate()
        {
            if(!flying_away)
            {
                // Idle ground animations
                return 1;
            }

            // TODO - Rigid body physics being fed velocity to fly away

            return 1;
        }

        protected override uint HandleTrigger(List<Collider.Collision> collisions)
        {
            // Activated by anything
            flying_away = true;

            // TODO - Call "fly" animation on animation component

            return 1;
        }
    }
}