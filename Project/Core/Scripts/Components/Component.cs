using Godot;

namespace EntComponents
{
	public partial class Component : Node
	{
		public Game.States MaximumActiveState = Game.States.active;
		
		public Node3D NodeParent
		{
			get
			{
				return (Node3D)GetParent();
			}
		}

		// Don't inheret from this as a Component, use OnEntityTick()
		public override void _Process(double delta)
		{
			if(!CanTick()) return;
			OnEntityTick(delta);
		}

        // Don't inheret from this as a Component, use OnEntityPhysicsTick()
        public override void _PhysicsProcess(double delta)
        {
			if(!CanTick()) return;
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
			return Game.CurrentState <= MaximumActiveState;
		}
	}
}