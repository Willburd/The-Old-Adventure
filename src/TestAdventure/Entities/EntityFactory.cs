namespace Engine
{
    public class TestAdventureEntityFactory : EntityFactory
    {
        protected override void ProduceComponents(Entity ent, string entity_key, string component_key)
        {
            switch(component_key)
            {
                default:
                    base.ProduceComponents(ent, entity_key, component_key);
                    return;

                ///////////////////////////////////////////////////
                // Actor behaviors
                ///////////////////////////////////////////////////
                
                case "TestActorBehavior":
                    new EntComponents.ActorBehavior.TestActorBehavior(ent);
                    return;


                ///////////////////////////////////////////////////
                // Scene behaviors
                ///////////////////////////////////////////////////

            }            
        }
    }
}