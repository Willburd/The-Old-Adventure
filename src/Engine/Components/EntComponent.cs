using Engine;

namespace EntComponents
{
    public class EntComponent
    {
        /// <summary>
        /// Contains a type indexed dictionary of lists, including every instantilized component of that type that currently exists. Objects are added and removed in the base ActorComponent constructor and destructor.
        /// </summary>
        private static readonly Dictionary<Type,List<EntComponent>> all_components = [];

        public Entity Host { get; }

        public EntComponent(Entity host_entity)
        {
            Host = host_entity;
            if(!all_components.ContainsKey(GetType())) all_components.Add(GetType(),[]);
            all_components[GetType()].Add(this);
            Host.AddComponent(this);
            RegisterSignals();
        }
        
        ~EntComponent()
        {
            UnregisterSignals();
            all_components[GetType()].Remove(this);
            Host.RemoveComponent(this);
            OnDestroy();
        }

        /// <summary>
        /// Returns a list with all components of the type specified. If an invalid component type, or no components are instantiated of that type, it will return an empty list.
        /// </summary>
        public static List<EntComponent> GetAllOfType(Type comp_type)
        {
            all_components.TryGetValue(comp_type, out List<EntComponent>? found_list);
            if(found_list == null) return [];
            return found_list;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Returns a list of signals that this component subscribes to by default, for both register and unregister. Making it easy to sub to multiple signals with minimal conditions. Otherwise override RegisterSignals() and UnregisterSignals().
        /// </summary>
        public virtual List<Core.Signals> DefaultSignals()
        {
            return [];
        }

        /// <summary>
        /// Connects specific signals to the host entity, based on the component's desired design. Default behavior uses the signals listed in create_with_signals, to automagically do the work for you.
        /// </summary>
        public virtual void RegisterSignals()
        {
            foreach(Core.Signals sig in DefaultSignals())
            {
                Host.RegisterSignal(sig, this);
            }
        }

        /// <summary>
        /// Disconnects the signals previously attached using RegisterSignals(). Default behavior uses the signals listed in create_with_signals, to automagically do the work for you.
        /// </summary>
        public virtual void UnregisterSignals()
        {
            foreach(Core.Signals sig in DefaultSignals())
            {
                Host.UnregisterSignal(sig, this);
            }
        }
        
        /// <summary>
        /// Fired when a signal is sent from the host entity. Signals are sent based on registeration order, so an assumed order should not be expected when designing component behaviors.
        /// </summary>
        public virtual int ReceiveSignal(Core.Signals signal, object[] args)
        {
            return 0;
        }

        /// <summary>
        /// Fired during component destroy. Used to cleanup.
        /// </summary>
        public virtual void OnDestroy()
        {
            
        }
    }
}