using EntComponents;

namespace Engine
{
    /// <summary>
    /// Base level type for all objects in the game engine. Scene and Actor both inheret from this, as it handles the component and signal system, both of which use it.
    /// </summary>
    public class Entity
    {
        private readonly Dictionary<Type,List<EntComponent>> attached_components = [];
        private readonly Dictionary<Core.Signals,List<EntComponent>> linked_signals = [];

        /// <summary>
        /// Adds a component to the entity. Do not call directly, called by EntComponent constructor.
        /// </summary>
        public EntComponent AddComponent(EntComponent component)
        {
            if(attached_components[component.GetType()] == null) attached_components[component.GetType()] = [];
            attached_components[component.GetType()].Add(component);
            return component;
        }

        /// <summary>
        /// Removes a component from an entity. Do not call directly, called by EntComponent destructor.
        /// </summary>
        public void RemoveComponent(EntComponent component)
        {
            attached_components[component.GetType()].Remove(component);
        }

        /// <summary>
        /// Gets a list of all components attached to this entity, of a specific type.
        /// </summary>
        public List<EntComponent> GetComponentList(Type comp_type)
        {
            attached_components.TryGetValue(comp_type, out List<EntComponent>? found_list);
            if(found_list == null) return [];
            return found_list;
        }

        /// <summary>
        /// Gets the first component of a specific type attached to this entity.
        /// </summary>
        public EntComponent? GetComponent(Type comp_type)
        {
            attached_components.TryGetValue(comp_type, out List<EntComponent>? found_list);
            if(found_list == null) return null;
            return found_list[0];
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signaling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        

        /// <summary>
        /// Registers a signal on this entity. When the signal is fired, the target component will be sent the signal. 
        /// </summary>
        public void RegisterSignal(Core.Signals signal, EntComponent target)
        {
            linked_signals[signal].Add(target);
        }

        /// <summary>
        /// Unregisters a signal on this entity, breaking the link between entity and component.
        /// </summary>
        public void UnregisterSignal(Core.Signals signal, EntComponent target)
        {
            linked_signals[signal].Remove(target);
        }

        public int SendSignal(Core.Signals signal)
        {
            if(linked_signals[signal] == null) return 0;
            int return_flags = 0;
            foreach(EntComponent comp in linked_signals[signal])
            {
                return_flags |= comp.RecieveSignal(signal);
            }
            return return_flags;
        }
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Used to design render behavior at atom level, as making a render component for every type of actor would quickly get out of hand.
        /// </summary>
        public virtual void OnRender()
        {
            
        }
    }
}