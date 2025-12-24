using Engine;

namespace EntComponents
{
    public class EntComponent
    {
        /// <summary>
        /// Contains a type indexed dictionary of lists, including every instantilized component of that type that currently exists. Objects are added and removed in the base ActorComponent constructor and destructor.
        /// </summary>
        private static readonly Dictionary<Type,List<EntComponent>> all_components = [];

        private readonly Entity host;

        public Entity Host
        {
            get
            {
                return host;
            }
        }

        public EntComponent(Entity host_entity)
        {
            host = host_entity;
            if(all_components[GetType()] == null) all_components[GetType()] = [];
            all_components[GetType()].Add(this);
            host.AddComponent(this);
            RegisterSignals();
        }
        
        ~EntComponent()
        {
            UnregisterSignals();
            all_components[GetType()].Remove(this);
            host.RemoveComponent(this);
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
        /// Connects specific signals to the host entity, based on the component's desired design.
        /// </summary>
        public virtual void RegisterSignals()
        {
            // Example: host.RegisterSignal(Core.Signals.test, this);
        }

        /// <summary>
        /// Disconnects the signals previously attached using RegisterSignals().
        /// </summary>
        public virtual void UnregisterSignals()
        {
            // Example: host.UnregisterSignal(Core.Signals.test, this);
        }
        
        /// <summary>
        /// Fired when a signal is sent from the host entity. Signals are sent based on registeration order, so an assumed order should not be expected when designing component behaviors.
        /// </summary>
        public virtual int ReceiveSignal(Core.Signals signal, object[] args)
        {
            return 0;
        }
    }
}