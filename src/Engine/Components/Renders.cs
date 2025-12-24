namespace EntComponents
{
    /// <summary>
    /// Component that enables the OnRender() function in an entity, and stores various other rendering information.
    /// </summary>
    public class Renders(Engine.Entity host_entity) : EntComponent(host_entity)
    {
        public void Render()
        {
            host_entity.OnRender();
        }
    }
}