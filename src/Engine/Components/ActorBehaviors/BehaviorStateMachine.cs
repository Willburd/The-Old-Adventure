
using Engine;

namespace EntComponents
{
    public class BehaviorStateMachine
    {
        public BehaviorStateMachine(EntComponent owner)
        {
            Owner = owner;
        }

        /// <summary>
        /// Owner behavior component.
        /// </summary>
        public EntComponent Owner { get; protected set; }
        
        /// <summary>
        /// Host actor that holds our component.
        /// </summary>
        public Actor Host 
        { 
            get
            {
                return (Actor)Owner.Host;
            }
        }

        /// <summary>
        /// Called when the player enters this state.
        /// </summary>
        public virtual void Start(BehaviorStateMachine? previous_state) { }

        /// <summary>
        /// Called when the player leaves this state.
        /// </summary>
        public virtual void End(BehaviorStateMachine? new_state) { }

        /// <summary>
        /// Called during update, performs logic for the current player state
        /// </summary>
        public virtual void Process() { }
        
        /// <summary>
        /// Gets the first component of a specific type attached to our player
        /// </summary>
        public EntComponent? GetComponent(Type comp_type)
        {
            return Owner.Host.GetComponent(comp_type);
        }
    }
}