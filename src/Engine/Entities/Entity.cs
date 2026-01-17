using EntComponents;
using System.Numerics;

namespace Engine
{
    /// <summary>
    /// Base level type for all objects in the game engine. Room and Actor both inheret from this, as it handles the component and signal system, both of which use it.
    /// </summary>
    public class Entity
    {

        public static List<Entity> UninitEntityList { get; set; } = [];

        public static List<Entity> EntityList { get; set; } = [];
        public static Dictionary<string, Entity> EntityLookupList { get; set; } = [];

        public static List<Entity> DestructingEntities { get; set; } = [];

        public string EntityID { get; private set; }
        public string AssetKey { get; private set; }

        public static void DestroyAllEntities()
        {
            // Do rooms first
            List<Entity> removal_list = [.. EntityList];
            foreach (Entity ent in removal_list)
            {
                if (ent is Room) ent.Destroy();
            }
            // Now unload the rest!
            removal_list = [.. EntityList];
            foreach (Entity ent in removal_list)
            {
                ent.Destroy();
            }
            EntityList.Clear();
        }

        /// <summary>
        /// If an entity has completed initilization. Initilization happens before the pre_update signal is called during the game loop. As doing this in the constructor is not possible due to execution order. (The signal system is not yet init on the entity, but the child is calling assetload/create signals.)
        /// </summary>
        private bool initilized = false;
        public bool IsInitilized { get { return initilized; } }

        public int UniqueSeed { get; private set; }

        public void OnInit()
        {
            initilized = true;
            SendSignal(Core.Signals.load_assets);
            SendSignal(Core.Signals.create);
        }

        /// <summary>
        /// If an entity is currently processed in the game update loop. Also disables rendering.
        /// </summary>
        public bool Enabled { get; set; } = true;

        /// <summary>
        /// If an entity is currently processed, checks if the room the entity is in is active as well. At least for actors...
        /// </summary>
        public virtual bool RoomEnabled()
        {
            return Enabled;
        }

        public Entity(Transform initial_location, string entity_id, string entity_asset_key)
        {
            UninitEntityList.Add(this);
            EntityID = entity_id;
            AssetKey = entity_asset_key;
            SetTransform(initial_location);
            UniqueSeed = (int)Tools.RandRange(0, 99999999);
        }


        /// <summary>
        /// Destroys an entity.
        /// </summary>
        public void Destroy()
        {
            Enabled = false;
            if (!DestructingEntities.Contains(this))
            {
                DestructingEntities.Add(this);
                SendSignal(Core.Signals.destroy);
                foreach (EntComponent component in GetAllComponents())
                {
                    RemoveComponent(component);
                }
                // Debug info
                if (GetType() == typeof(Actor))
                {
                    // Show our linked room
                    Actor us_as_actor = (Actor)this;
                    Console.WriteLine("EntityDestroy-X (" + EntityID + ")[" + us_as_actor.OwnerRoom?.EntityID + "] : " + AssetKey);
                }
                else
                {
                    // Just delete info
                    Console.WriteLine("EntityDestroy-X (" + EntityID + ") : " + AssetKey);
                }
                // finish up
                OnCleanup();
            }
        }

        /// <summary>
        /// Called at the end of Destroy, handles any special cleanup an entity type does.
        /// </summary>
        protected virtual void OnCleanup() { }

        public float MinimumRenderDistance { get; set; } = 8f;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Component helpers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private readonly Dictionary<Type, List<EntComponent>> attached_components = [];
        private readonly Dictionary<Core.Signals, List<EntComponent>> linked_signals = [];

        /// <summary>
        /// Adds a component to the entity. Do not call directly, called by EntComponent constructor. You only need to instantiate the component, and provide the entity as it's target.
        /// </summary>
        public EntComponent AddComponent(EntComponent component)
        {
            if (!attached_components.ContainsKey(component.GetType())) attached_components.Add(component.GetType(), []);
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
            attached_components.TryGetValue(comp_type, out List<EntComponent> found_list);
            if (found_list == null) return [];
            return found_list;
        }

        /// <summary>
        /// Gets the first component of a specific type attached to this entity.
        /// </summary>
        public EntComponent? GetComponent(Type comp_type)
        {
            attached_components.TryGetValue(comp_type, out List<EntComponent> found_list);
            if (found_list == null) return null;
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
                foreach (EntComponent comp in ent_list)
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
            if (!linked_signals.ContainsKey(signal)) linked_signals.Add(signal, []);
            linked_signals[signal].Add(target);
        }

        /// <summary>
        /// Unregisters a signal on this entity, breaking the link between entity and component.
        /// </summary>
        public void UnregisterSignal(Core.Signals signal, EntComponent target)
        {
            linked_signals[signal].Remove(target);
        }

        /// <summary>
        /// Sends signal to all components attached to the entity. Responds with a uint bitflag of information related to the signal. How that is used is entirely dependant on the signal.
        /// </summary>
        public uint SendSignal(Core.Signals signal, params object?[] args)
        {
            if (!IsInitilized) return 0; // Nope, we don't do anything with this until we are initilized!
            if (!linked_signals.TryGetValue(signal, out List<EntComponent> signal_list)) return 0;
            uint return_flags = 0;
            foreach (EntComponent comp in signal_list)
            {
                return_flags |= comp.ReceiveSignal(signal, args);
            }
            return return_flags;
        }

        public static void SendGlobalSignal(Core.Signals signal, params object?[] args)
        {
            foreach (Entity ent in EntityList)
            {
                ent.SendSignal(signal, args);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Position rotation and scale in world.
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        protected readonly Transform transform = new();
        protected readonly Transform last_transform = new();

        public Transform Location
        {
            get { return GetTransform(); }
            set { SetTransform(Location); }
        }

        public Vector3 Position
        {
            get { return transform.Position; }
            set { transform.Position = value; }
        }

        public Quaternion Rotation
        {
            get { return transform.Rotation; }
            set { transform.Rotation = value; }
        }

        public Vector3 Scale
        {
            get { return transform.Scale; }
            set { transform.Scale = value; }
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

        private void SetTransform(Transform assignment)
        {
            if (SendSignal(Core.Signals.move_absolute, assignment) > 0) return;
            transform.Set(assignment);
            SnapTransform();
        }

        public Transform GetTransform()
        {
            return transform;
        }

        public Vector3 GetInterpolatedPosition(double tick_delta)
        {
            return Vector3.Lerp(last_transform.Position, transform.Position, (float)tick_delta);
        }

        public Quaternion GetInterpolatedRotation(double tick_delta)
        {
            return Quaternion.Lerp(last_transform.Rotation, transform.Rotation, (float)tick_delta);
        }

        public Vector3 GetInterpolatedScale(double tick_delta)
        {
            return Vector3.Lerp(last_transform.Scale, transform.Scale, (float)tick_delta);
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
            return Matrix4x4.Identity * Matrix4x4.CreateScale(GetInterpolatedScale(tick_delta)) * Matrix4x4.CreateFromQuaternion(GetInterpolatedRotation(tick_delta)) * Matrix4x4.CreateTranslation(GetInterpolatedPosition(tick_delta));
        }
    }
}