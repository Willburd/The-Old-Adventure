using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that inherets from Collider, but acts as a trigger area. Intended for scripting events instead of physics.
    /// </summary>
    public class TriggerVolume(Entity host_entity) : Collider(host_entity)
    {
        public new bool IsTrigger { get; set; } = true;
    }
}