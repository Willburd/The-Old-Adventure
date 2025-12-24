using System.Collections.Generic;

namespace ActorComponents
{
    public class ActorComponent
    {
        /// <summary>
        /// Contains a type indexed dictionary of lists, including every instantilized component of that type that currently exists. Objects are added and removed in the base ActorComponent constructor and destructor.
        /// </summary>
        private static readonly Dictionary<Type,List<ActorComponent>> all_components = [];

        public ActorComponent()
        {
            if(all_components[GetType()] == null) all_components[GetType()] = [];
            all_components[GetType()].Add(this);
        }
        
        ~ActorComponent()
        {
            all_components[GetType()].Remove(this);
        }

        /// <summary>
        /// Returns a list with all components of the type specified. If an invalid component type, or no components are instantiated of that type, it will return an empty list.
        /// </summary>
        public static List<ActorComponent> GetAllOfType(Type comp_type)
        {
            all_components.TryGetValue(comp_type, out List<ActorComponent>? found_list);
            if(found_list == null) return [];
            return found_list;
        } 
    }
}