using EntComponents;

namespace StateMachines
{
    public class StateMachine(Behavior owner)
    {

        public Behavior Behavior { private set; get; } = owner;

        public virtual void Begin(StateMachine previous_state)
        {
            
        }

        public virtual void End(StateMachine next_state)
        {
            
        }

        public virtual void OnStateTick(double delta)
        {
            
        }
        
        public virtual void OnStatePhysicsTick(double delta)
        {
            
        }
    }
}