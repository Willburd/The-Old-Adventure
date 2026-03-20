using Godot;

namespace EntComponents
{
	public partial class Component : Node
	{
		public Node3D NodeParent
		{
			get
			{
				return GetParent<Node3D>();
			}
		}

		public Entity EntityParent
		{
			get
			{
				return GetParent<Entity>();
			}
		}

		// Don't inheret from this as a Component, use OnEntityTick()
		public override void _Process(double delta)
		{
			if(!EntityParent.CanTick()) return;
			OnEntityTick(delta);
		}

        // Don't inheret from this as a Component, use OnEntityPhysicsTick()
        public override void _PhysicsProcess(double delta)
        {
			if(!EntityParent.CanTick()) return;
			OnEntityPhysicsTick(delta);
		}

		/// <summary>
		/// Fired on Process, but only if CanTick() allows it.
		/// </summary>
		public virtual void OnEntityTick(double delta)
		{
			
		}

		/// <summary>
		/// Fired on PhysicsProcess, but only if CanTick() allows it.
		/// </summary>
		public virtual void OnEntityPhysicsTick(double delta)
		{
			
		}

		/// <summary>
		/// Allows a component to override the default behavior of pausing when its host entity is paused.
		/// </summary>
		public virtual bool CanTick()
		{
			return EntityParent.CanTick();
		}
	}
}