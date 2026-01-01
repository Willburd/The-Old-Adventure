namespace Engine
{
    public class TestAdventureEntityFactory : EntityFactory
    {
        protected override void ProduceComponents(Entity ent, string entity_key, string component_key)
        {
            switch(component_key)
            {

                // Actor behaviors
                case "TestActorBehavior":
                    new EntComponents.TestActorBehavior(ent);
                    return;


                // Scene behaviors



                default:
                    base.ProduceComponents(ent, entity_key, component_key);
                    return;
            }            
        }
    }
}