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
            OnDestroy();
            UnregisterSignals();
            all_components[GetType()].Remove(this);
            Host.RemoveComponent(this);
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
            return [Core.Signals.create, Core.Signals.update];
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
        public virtual uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)            
            {
                case Core.Signals.create:
                    return HandleCreate();
                    
                case Core.Signals.cache_components:
                    return HandleCacheComponents();

                case Core.Signals.pre_update:
                    return HandlePreUpdate();

                case Core.Signals.update:
                    return HandleUpdate();

                case Core.Signals.post_update:
                    return HandlePostUpdate();
            }
            return 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Used to set data after resources are loaded by HandleAssetLoad()
        /// </summary>
        protected virtual uint HandleCreate()
        {
            return 1;
        }

        /// <summary>
        /// Used to cache components for faster lookup on complex components that interact with multiple other components at once. Done after create to ensure all components have finished being added.
        /// </summary>
        protected virtual uint HandleCacheComponents()
        {
            return 1;
        }

        /// <summary>
        /// Used to respond to preupdate game ticks.
        /// </summary>
        protected virtual uint HandlePreUpdate()
        {
            return 1;
        }

        /// <summary>
        /// Used to respond to standard game ticks.
        /// </summary>
        protected virtual uint HandleUpdate()
        {
            return 1;
        }

        /// <summary>
        /// Used to respond to post update game ticks.
        /// </summary>
        protected virtual uint HandlePostUpdate()
        {
            return 1;
        }

        /// <summary>
        /// Called at the start of component destroy. Used to cleanup.
        /// </summary>
        protected virtual uint OnDestroy()
        {
            return 1;
        }
    }
}