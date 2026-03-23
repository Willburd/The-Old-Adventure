using StateMachines;

namespace EntComponents
{
	public partial class Behavior : Component
    {
		private StateMachine _current_state;

        public StateMachine CurrentState
        {
            set
            {
                _current_state?.End(value);
                value.Begin(_current_state);
                _current_state = value;
            }
            get
            {
                return _current_state;
            }
        }

		public override void OnEntityTick(double delta)
		{
            // Use the state by default, override if the behavior is simple enough to not use states
			_current_state?.OnStateTick(delta);
		}
		
		public override void OnEntityPhysicsTick(double delta)
		{
            // Use the state by default, override if the behavior is simple enough to not use states
			_current_state?.OnStatePhysicsTick(delta);
		}
    }
}