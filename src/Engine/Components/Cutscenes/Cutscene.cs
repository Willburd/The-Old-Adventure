using Engine;
using System.Numerics;

namespace EntComponents.Cutscenes
{
    /// <summary>
    /// Entity Component that acts as the base for other cutscenes. Expects to be attached to a room. CanStartCutscene should be called to see if a cutscene can start before the scene is instantiated.
    /// Cutscenes override the Input component with it's own input by default. 
    /// </summary>
    public class Cutscene : EntComponent
    {
        public static bool CanStartCutscene
        {
            get
            {
                return Current == null;
            }
        }

        public static Cutscene? Current { get; set; } = null;

        public Cutscene(Entity host_entity) : base(host_entity)
        {
            // Use CanStartCutscene before to check if we should start!
            host_entity.RemoveAllComponentsOfType(typeof(Cutscene));
            Current = this;
        }

        public virtual bool BlockInput()
        {
            return true;
        }

        public virtual Vector3 Move()
        {
            return Vector3.Zero;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.update];
        }

        protected override uint OnDestroy()
        {
            if (Current == this) Current = null;
            return 1;
        }
    }
}