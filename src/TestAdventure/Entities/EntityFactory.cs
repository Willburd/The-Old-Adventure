namespace Engine
{
    public class TestAdventureEntityFactory : EntityFactory
    {
        protected override EntComponents.EntComponent ProduceComponents(Entity ent, string entity_key, string component_key)
        {
            switch(component_key)
            {
                default:
                    return base.ProduceComponents(ent, entity_key, component_key);

                ///////////////////////////////////////////////////
                // Actor behaviors
                ///////////////////////////////////////////////////
                
                case "TestActorBehavior":
                    return new EntComponents.ActorBehavior.TestActorBehavior(ent);

                ///////////////////////////////////////////////////
                // Room behaviors
                ///////////////////////////////////////////////////

            }            
        }
    }
}