using System.Diagnostics;
using Newtonsoft.Json.Linq;

namespace Engine
{
    public static class EntityFactory
    {
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
                Type type = Type.GetType("EntComponents." + component_key);
                Debug.Assert(type != null, "A non existant component typekey was added to a " + actual_key + " during json decode, are you missing a namespace?: " + component_key);
                Activator.CreateInstance(type, [actor]);
            }
            ErrorThreshold(pre_count, 500, "!!WARNING: Extreme entity count, 500 ents!!");
            ErrorThreshold(pre_count, 1000, "!!!!!!!WARNING: Extreme entity count, 1000 ents!!!!!!!");
            ErrorThreshold(pre_count, 2000, "!!!!!!!!!!!!!!WARNING: Extreme entity count, 2000 ents!!!!!!!!!!!!!!");
            return actor;
        }

        /// <summary>
        /// Give a warning for excessive entity counts.
        /// </summary>
        private static void ErrorThreshold(int pre_count, int warn_at_count, string message)
        {
            if (pre_count < warn_at_count && (Entity.UninitEntityList.Count + Entity.EntityList.Count) >= warn_at_count) Console.WriteLine(message);
        }
    }
}