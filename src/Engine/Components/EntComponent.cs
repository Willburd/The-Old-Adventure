using Engine;
using Silk.NET.Input;

namespace EntComponents
{
    public class EntComponent
    {
        /// <summary>
        /// Contains a type indexed dictionary of lists, including every instantilized component of that type that currently exists. Objects are added and removed in the base ActorComponent constructor and destructor.
        /// </summary>
        private static readonly Dictionary<Type, List<EntComponent>> all_components = [];

        public Entity Host { get; }

        public bool ActiveUpdate { get; set; } = true;

        public EntComponent(Entity host_entity)
        {
            Host = host_entity;
            if (!all_components.ContainsKey(GetType())) all_components.Add(GetType(), []);
            all_components[GetType()].Add(this);
            Host._InternalAddComponent(this);
            RegisterSignals();
        }

        /// <summary>
        /// Performs component removal from the global list, as well as calls the virtual destroy action. Should not be manually called, called by Entity.RemoveComponent()
        /// </summary>
        public void InternalDestroyComponent()
        {
            OnDestroy();
            UnregisterSignals();
            all_components[GetType()].Remove(this);
        }

        /// <summary>
        /// Returns a list with all components of the type specified. If an invalid component type, or no components are instantiated of that type, it will return an empty list.
        /// </summary>
        public static List<EntComponent> GetAllOfType(Type comp_type)
        {
            all_components.TryGetValue(comp_type, out List<EntComponent> found_list);
            if (found_list == null) return [];
            return found_list;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Returns a list of signals that this component subscribes to by default, for both register and unregister. Making it easy to sub to multiple signals with minimal conditions. Otherwise override RegisterSignals() and UnregisterSignals().
        /// </summary>
        public virtual List<Core.Signals> PrepareSignals()
        {
            return [];
        }

        /// <summary>
        /// Connects specific signals to the host entity, based on the component's desired design. Default behavior uses the signals listed in create_with_signals, to automagically do the work for you.
        /// </summary>
        public virtual void RegisterSignals()
        {
            foreach (Core.Signals sig in PrepareSignals())
            {
                Host.RegisterSignal(sig, this);
            }
        }

        /// <summary>
        /// Disconnects the signals previously attached using RegisterSignals(). Default behavior uses the signals listed in create_with_signals, to automagically do the work for you.
        /// </summary>
        public virtual void UnregisterSignals()
        {
            foreach (Core.Signals sig in PrepareSignals())
            {
                Host.UnregisterSignal(sig, this);
            }
        }

        /// <summary>
        /// Fired when a signal is sent from the host entity. Signals are sent based on registeration order, so an assumed order should not be expected when designing component behaviors.
        /// </summary>
        public virtual uint ReceiveSignal(Core.Signals signal, object?[] args)
        {
            switch (signal)
            {
                /////////////////////////////////////////////////////////
                // Creation and setup
                /////////////////////////////////////////////////////////
                case Core.Signals.load_assets:
                    return HandleAssetLoad();
                case Core.Signals.create:
                    return HandleCreate();

                /////////////////////////////////////////////////////////
                // Update handlers
                /////////////////////////////////////////////////////////
                case Core.Signals.pre_update:
                    if (!ActiveUpdate) return 0;
                    return HandlePreUpdate();
                case Core.Signals.editor_update:
                    return HandleEditorUpdate();

                case Core.Signals.update:
                    if (!ActiveUpdate) return 0;
                    return HandleUpdate();
                case Core.Signals.post_update:
                    if (!ActiveUpdate) return 0;
                    return HandlePostUpdate();

                /////////////////////////////////////////////////////////
                // Input handlers
                /////////////////////////////////////////////////////////
                case Core.Signals.input_pressed:
                    if (!ActiveUpdate) return 0;
                    return HandlePressed((Key?)args[0], (ButtonName?)args[1]);
                case Core.Signals.input_released:
                    if (!ActiveUpdate) return 0;
                    return HandleReleased((Key?)args[0], (ButtonName?)args[1]);

                /////////////////////////////////////////////////////////
                // Collision handling
                /////////////////////////////////////////////////////////
                case Core.Signals.collision:
                    return HandleCollisions((List<Collider.Collision>)args[0]);
                case Core.Signals.collision_start:
                    return HandleCollisionStart((Collider)args[0]);
                case Core.Signals.collision_end:
                    return HandleCollisionEnd((Collider)args[0]);

                case Core.Signals.trigger:
                    return HandleTriggers((List<Collider.Collision>)args[0]);
                case Core.Signals.trigger_start:
                    return HandleTriggerStart((Collider)args[0]);
                case Core.Signals.trigger_end:
                    return HandleTriggerEnd((Collider)args[0]);
            }
            return 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Loads all component related assets. Called before Init()
        /// </summary>
        protected virtual uint HandleAssetLoad()
        {
            // You would call AssetLoader.MaterialAssetLoad() and such here for the asset!
            // Shaders should be loaded by the game itself, and assets used in nearly every room should be treated the same way!

            return 0;
        }

        /// <summary>
        /// Used to set data after resources are loaded by HandleAssetLoad()
        /// </summary>
        protected virtual uint HandleCreate()
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to preupdate game ticks.
        /// </summary>
        protected virtual uint HandlePreUpdate()
        {
            return 0;
        }

        /// <summary>
        /// Editor only update tick. If these are happening, then the main update loop is disabled.
        /// </summary>
        protected virtual uint HandleEditorUpdate()
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to standard game ticks.
        /// </summary>
        protected virtual uint HandleUpdate()
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to post update game ticks.
        /// </summary>
        protected virtual uint HandlePostUpdate()
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to input key pressed
        /// </summary>
        protected virtual uint HandlePressed(Key? key, ButtonName? button)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to input key release
        /// </summary>
        protected virtual uint HandleReleased(Key? key, ButtonName? button)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to collisions.
        /// </summary>
        protected virtual uint HandleCollisions(List<Collider.Collision> collisions)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to collisions.
        /// </summary>
        protected virtual uint HandleCollisionStart(Collider new_collision)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to collisions.
        /// </summary>
        protected virtual uint HandleCollisionEnd(Collider was_colliding_with)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to trigger entry.
        /// </summary>
        protected virtual uint HandleTriggers(List<Collider.Collision> collisions)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to trigger entry.
        /// </summary>
        protected virtual uint HandleTriggerStart(Collider new_trigger)
        {
            return 0;
        }

        /// <summary>
        /// Used to respond to trigger entry.
        /// </summary>
        protected virtual uint HandleTriggerEnd(Collider was_triggered_by)
        {
            return 0;
        }

        /// <summary>
        /// Called at the start of component destroy. Used to cleanup.
        /// </summary>
        protected virtual uint OnDestroy()
        {
            return 0;
        }
    }
}