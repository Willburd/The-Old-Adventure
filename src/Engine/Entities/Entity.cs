using EntComponents;
using System.Numerics;

namespace Engine
{
    /// <summary>
    /// Base level type for all objects in the game engine. Scene and Actor both inheret from this, as it handles the component and signal system, both of which use it.
    /// </summary>
    public class Entity
    {
        private static readonly List<Entity> entity_list = [];
        public static List<Entity> EntityList { get {return entity_list;} }

        public static void DestroyAllEntities()
        {
            List<Entity> removal_list = [.. EntityList]; 
            foreach(Entity ent in removal_list)
            {
                ent.Destroy();
            }
            EntityList.Clear();
        }


        /// <summary>
        /// If an entity has completed initilization. Initilization happens before the pre_update signal is called during the game loop. As doing this in the constructor is not possible due to execution order. (The signal system is not yet init on the entity, but the child is calling assetload/create signals.)
        /// </summary>
        private bool initilized = false;
        public bool IsInitilized { get {return initilized;} }

        public void OnInit()
        {
            initilized = true;
            SendSignal(Core.Signals.create);
            SendSignal(Core.Signals.cache_components);
        }

        /// <summary>
        /// If an entity is currently processed in the game update loop. Also disables rendering.
        /// </summary>
        public bool Enabled { get; set; } = true;

        public Entity(Transform initial_location)
        {
            entity_list.Add(this);
            SetTransform(initial_location);
        }

        /// <summary>
        /// Destroys an entity. Removing it from the entity processing list, and removing all components attached to it.
        /// </summary>
        public void Destroy()
        {
            SendSignal(Core.Signals.destroy);
            entity_list.Remove(this);
            Enabled = false;
            foreach(EntComponent component in GetAllComponents())
            {
                RemoveComponent(component);
            }
            attached_components.Clear();
            OnCleanup();
        }

        /// <summary>
        /// Called at the end of Destroy, handles any special cleanup an entity type does.
        /// </summary>
        protected virtual void OnCleanup()
        {
            
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Component helpers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private readonly Dictionary<Type,List<EntComponent>> attached_components = [];
        private readonly Dictionary<Core.Signals,List<EntComponent>> linked_signals = [];

        /// <summary>
        /// Adds a component to the entity. Do not call directly, called by EntComponent constructor. You only need to instantiate the component, and provide the entity as it's target.
        /// </summary>
        public EntComponent AddComponent(EntComponent component)
        {
            if(!attached_components.ContainsKey(component.GetType())) attached_components.Add(component.GetType(),[]);
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

        /// <summary>
        /// Gets all components attached to this entity in a list.
        /// </summary>
        public List<EntComponent> GetAllComponents()
        {
            List<EntComponent> found_list = [];
            foreach ((Type comp_type, List<EntComponent> ent_list) in attached_components)
            {
                foreach(EntComponent comp in ent_list)
                {
                    found_list.Add(comp);
                }
            }
            return found_list;
        }

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signaling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Registers a signal on this entity. When the signal is fired, the target component will be sent the signal. 
        /// </summary>
        public void RegisterSignal(Core.Signals signal, EntComponent target)
        {
            if(!linked_signals.ContainsKey(signal)) linked_signals.Add(signal,[]);
            linked_signals[signal].Add(target);
        }

        /// <summary>
        /// Unregisters a signal on this entity, breaking the link between entity and component.
        /// </summary>
        public void UnregisterSignal(Core.Signals signal, EntComponent target)
        {
            linked_signals[signal].Remove(target);
        }

        public uint SendSignal(Core.Signals signal, params object[] args)
        {
            if(!IsInitilized) return 0; // Nope, we don't do anything with this until we are initilized!
            if(!linked_signals.TryGetValue(signal, out List<EntComponent>? signal_list)) return 0;
            uint return_flags = 0;
            foreach(EntComponent comp in signal_list)
            {
                return_flags |= comp.ReceiveSignal(signal, args);
            }
            return return_flags;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Position rotation and scale in world.
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         
        protected readonly Transform transform = new();     
        protected readonly Transform last_transform = new();  
        
        public Vector3 Position
        {
            get {return transform.Position;}
            set {transform.Position = value;}
        }

        public Quaternion Rotation
        {
            get {return transform.Rotation;}
            set {transform.Rotation = value;}
        }

        public Vector3 Scale
        {
            get {return transform.Scale;}
            set {transform.Scale = value;}
        }

        /// <summary>
        /// Sets the last position of the transform to the current position. Preventing the renderer from interpolating the object from one position to another over long distances, such as teleporting.
        /// </summary>
        public void SnapTransform()
        {
            last_transform.Position = transform.Position;
            last_transform.Rotation = transform.Rotation;
            last_transform.Scale = transform.Scale;
        }

        public void MoveTransform(Transform addition)
        {
            if(SendSignal(Core.Signals.move_relative, addition) > 0) return;
            transform.Add(addition);
        }
        
        public void SetTransform(Transform assignment)
        {
            if(SendSignal(Core.Signals.move_absolute, assignment) > 0) return;
            transform.Set(assignment);
            SnapTransform();
        }

        public Vector3 GetInterpolatedPosition(double tick_delta)
        {
            return Vector3.Lerp( last_transform.Position, transform.Position, (float)tick_delta);
        }

        public Quaternion GetInterpolatedRotation(double tick_delta)
        {
            return Quaternion.Lerp( last_transform.Rotation, transform.Rotation, (float)tick_delta);
        }
        
        public Vector3 GetInterpolatedScale(double tick_delta)
        {
            return Vector3.Lerp( last_transform.Scale, transform.Scale, (float)tick_delta);
        }

        public Matrix4x4 GetViewMatrix()
        {
            return GetInterpolatedViewMatrix(1);
        }
        public Matrix4x4 GetLastViewMatrix()
        {
            return GetInterpolatedViewMatrix(0);
        }

        public Matrix4x4 GetInterpolatedViewMatrix(double tick_delta)
        {
            return Matrix4x4.Identity * Matrix4x4.CreateFromQuaternion(GetInterpolatedRotation(tick_delta)) * Matrix4x4.CreateScale(GetInterpolatedScale(tick_delta)) * Matrix4x4.CreateTranslation(GetInterpolatedPosition(tick_delta));
        }
    }
}