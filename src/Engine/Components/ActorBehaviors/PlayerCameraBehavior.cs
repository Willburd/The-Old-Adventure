using Engine;

namespace EntComponents.ActorBehavior
{
    public class PlayerCameraBehavior : EntComponent
    {
        public PlayerCameraBehavior(Entity host_entity) : base(host_entity)
        {
            current_state = new CameraStates.CameraOrbitFollow(this);
        }

        protected CameraStates.CameraState? current_state;

        public void SetPlayerState(CameraStates.CameraState? new_state)
        {
            current_state?.End(new_state);
            new_state?.Start(current_state);
            current_state = new_state;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.create, Core.Signals.update];
        }

        protected override uint HandleUpdate()
        {
            // Change state depending on gameplay


            // Process current state
            current_state?.Process();
            return 1;
        }
    }
}