
using Engine;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class PlayerState
    {
        public const float ground_acceleration = 0.022f;
        public const float ground_friction = 0.014f;
        public const float ground_run_maxspeed = 0.1f;

        
        public const float ground_snap_threshold = 0.05f;


        public PlayerState(PlayerActorBehavior owner)
        {
            Owner = owner;
        }

        /// <summary>
        /// Owner behavior component.
        /// </summary>
        public PlayerActorBehavior Owner { get; private set; }
        
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
        public virtual void Start(PlayerState? previous_state) { }

        /// <summary>
        /// Called when the player leaves this state.
        /// </summary>
        public virtual void End(PlayerState? new_state) { }

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