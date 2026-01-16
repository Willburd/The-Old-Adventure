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
            if (source == AssetLoader.AssetSource.adventure && Core.AdventureID != null) prefix = Core.AdventureID;
            return prefix + "::";
        }

        private static void ParseJson(string path, AssetLoader.AssetSource source)
        {
            JObject json_decode = JObject.Parse(File.ReadAllText(path));
            foreach (KeyValuePair<string, JToken?> entry in json_decode)
            {
                string actor_key = GetEntityPrefix(source) + entry.Key;
                if (entry.Value == null || !entry.Value.HasValues)
                {
                    Debug.Assert(false, "entities.json had invalid key " + actor_key + ". No component list?");
                    continue;
                }
                List<string> components = [];
                foreach (JProperty component_entry in entry.Value.Children().Cast<JProperty>())
                {
                    components.Add(component_entry.Name);
                }
                entity_library.Add(actor_key, components);
            }
        }

        /// <summary>
        /// Library of entity keys to components they contain.
        /// </summary>
        private static Dictionary<string, List<string>> entity_library = [];

        public static Actor CreateActor(string id, string asset_key, Transform initial_location, Room? room_link = null, AssetLoader.AssetSource source = AssetLoader.AssetSource.adventure)
        {
            int pre_count = Entity.UninitEntityList.Count + Entity.EntityList.Count;
            string actual_key = GetEntityPrefix(source) + asset_key;
            Debug.Assert(entity_library.ContainsKey(actual_key), "Invalid asset key in entity library. Does it exist in the entities.json?");
            Console.WriteLine("ActorFactory (" + id + ") -> " + actual_key);

            Actor actor = new(initial_location, id, actual_key, room_link);
            foreach (string component_key in entity_library[actual_key])
            {
                entity_Factory.ProduceComponents(actor, actual_key, component_key);
            }

            if (pre_count < 200 && (Entity.UninitEntityList.Count + Entity.EntityList.Count) >= 200) Console.WriteLine("WARNING: Excessive entity count, 200 ents.");
            if (pre_count < 300 && (Entity.UninitEntityList.Count + Entity.EntityList.Count) >= 300) Console.WriteLine("!!!!!WARNING: Extreme entity count, 300 ents!!!!!");
            if (pre_count < 1000 && (Entity.UninitEntityList.Count + Entity.EntityList.Count) >= 1000) Console.WriteLine("!!!!!!!!!!!!!!WARNING: Extreme entity count, 1000 ents!!!!!!!!!!!!!!");
            if (pre_count < 2000 && (Entity.UninitEntityList.Count + Entity.EntityList.Count) >= 2000) Console.WriteLine("!!!!!!!!!!!!!!WARNING: Extreme entity count, 2000 ents!!!!!!!!!!!!!!");
            return actor;
        }

        /// <summary>
        /// Attaches components to entities.
        /// </summary>
        protected virtual EntComponents.EntComponent? ProduceComponents(Entity ent, string asset_key, string component_key)
        {
            Type type = Type.GetType("EntComponents." + component_key);
            Debug.Assert(type != null, "A non existant component typekey was added to a " + asset_key + " during json decode, are you missing a namespace?: " + component_key);
            return (EntComponents.EntComponent?)Activator.CreateInstance(type, [ent]);
        }
    }
}