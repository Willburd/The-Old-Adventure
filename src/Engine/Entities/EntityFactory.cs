using System.Diagnostics;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Engine
{
    public class EntityFactory
    {
        /// <summary>
        /// Override during OnPreInit() with your own subtype if your adventure requires an extended EntityFactory(). It likely does!
        /// </summary>
        public static EntityFactory entity_Factory = new EntityFactory();

        public static void InitLibrary()
        {
            ParseJson(AssetLoader.AssetDirectoryEngine + "/Entities.json", AssetLoader.AssetSource.engine);
            ParseJson(AssetLoader.AssetDirectoryAdventure + "/Entities.json", AssetLoader.AssetSource.adventure);
        }

        private static string GetEntityPrefix(AssetLoader.AssetSource source)
        {
            string prefix = "Engine";
            if(source == AssetLoader.AssetSource.adventure && Core.AdventureID != null) prefix = Core.AdventureID;
            return prefix + "::";
        }

        private static void ParseJson(string path, AssetLoader.AssetSource source)
        {
            JObject json_decode = JObject.Parse(File.ReadAllText( path));
            foreach(KeyValuePair<string,JToken?> entry in json_decode)
            {
                string actor_key = GetEntityPrefix(source) + entry.Key;
                List<string>? component_keys = entry.Value?.ToObject<List<string>>(); 
                if(component_keys != null && component_keys.Count > 0)
                {
                    entity_library.Add(actor_key, component_keys);
                }
                else
                {
                    Debug.Assert(false, "entities.json had invalid key " + actor_key + ". No component list?");
                }
            }
        }


        /// <summary>
        /// Library of entity keys to components they contain.
        /// </summary>
        private static Dictionary<string, List<string>> entity_library = [];

        public static Actor CreateActor(string entity_key, Transform initial_location,Room? room_link = null, AssetLoader.AssetSource source = AssetLoader.AssetSource.adventure)
        {
            string actual_key = GetEntityPrefix(source) + entity_key;
            Debug.Assert(entity_library.ContainsKey(actual_key), "Invalid asset key in entity library. Does it exist in the entities.json?");
            Console.WriteLine("ActorFactory-> " + actual_key);

            Actor actor = new(initial_location, actual_key, room_link);
            foreach(string component_key in entity_library[actual_key])
            {
                entity_Factory.ProduceComponents(actor, actual_key, component_key);
            }
            return actor;
        }


        /// <summary>
        /// Attaches components to entities.
        /// </summary>
        protected virtual EntComponents.EntComponent ProduceComponents(Entity ent, string entity_key, string component_key)
        {
            switch(component_key)
            {
                default:
                    Debug.Assert(false,"A non existant component typekey was added to a " + entity_key + " during json decode: " + component_key);
                    return null;

                ///////////////////////////////////////////////////
                // Base
                ///////////////////////////////////////////////////
                case "Collider":
                    return new EntComponents.Collider(ent);

                case "TriggerVolume":
                    return new EntComponents.TriggerVolume(ent);

                case "WorldRender":
                    return new EntComponents.WorldRender(ent);

                case "PhysicsBody":
                    return new EntComponents.PhysicsBody(ent);

                ///////////////////////////////////////////////////
                // Editor
                ///////////////////////////////////////////////////
                case "EditorCameraBehavior":
                    return new EntComponents.EditorCameraBehavior(ent);


                ///////////////////////////////////////////////////
                // Behaviors
                ///////////////////////////////////////////////////
                case "CubeActorBehavior":
                    return new EntComponents.ActorBehavior.CubeActorBehavior(ent);

                case "PointerActorBehavior":
                    return new EntComponents.ActorBehavior.PointerActorBehavior(ent);

                case "FlyAwayBirdActorBehavior":
                    return new EntComponents.ActorBehavior.FlyAwayBirdActorBehavior(ent);
            }
        
        }
    }
}