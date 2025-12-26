using System.Numerics;

namespace Engine
{
    public static class Tools
    {
        public static string WorkingDirectory
        {
            get
            {
                // TODO - get this to not be godawful and actually use the project folder
                return AppDomain.CurrentDomain.BaseDirectory + "../../../";
            }
        }

        public static string AssetDirectory
        {
            get
            {
                return WorkingDirectory + "Assets";
            }
        }
    }
}