using System.Numerics;
using Engine;

namespace Engine
{
    public static partial class Savestate
    {
        private readonly static Dictionary<string, int> temporary_flags = [];
        private readonly static Dictionary<string, int> save_flags = [];

        public static void ResetState()
        {
            ClearFlags();
            ClearTempFlags();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Save flag accessors
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void ClearFlags()
        {
            save_flags.Clear();
        }

        public static void SetValue(string key, int set_to)
        {
            save_flags.TryAdd(key, set_to);
        }

        public static void SetValue(string key, bool activated)
        {
            save_flags.TryAdd(key, activated ? 1 : 0);
        }

        public static void Activate(string key)
        {
            save_flags.TryAdd(key, 1);
        }

        public static int GetValue(string key)
        {
            save_flags.TryGetValue(key, out int value);
            return value;
        }
        
        public static bool GetActive(string key)
        {
            save_flags.TryGetValue(key, out int value);
            return value > 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Temporary flag accessors
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void ClearTempFlags()
        {
            temporary_flags.Clear();
        }

        public static void SetTempValue(string key, int set_to)
        {
            temporary_flags.TryAdd(key, set_to);
        }

        public static void SetTempValue(string key, bool activated)
        {
            temporary_flags.TryAdd(key, activated ? 1 : 0);
        }

        public static void ActivateTemp(string key)
        {
            temporary_flags.TryAdd(key, 1);
        }

        public static int GetTempValue(string key)
        {
            temporary_flags.TryGetValue(key, out int value);
            return value;
        }
        
        public static bool GetTempActive(string key)
        {
            temporary_flags.TryGetValue(key, out int value);
            return value > 0;
        }
    }
}