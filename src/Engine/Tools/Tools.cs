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

        /// <summary>
        /// Lightweight class used for holding a position and rotation in 3D space.
        /// </summary>
        public class Location
        {
            public Vector3 position;
            public Quaternion rotation;

            public Vector3 Position
            {
                get {return position;}
                set {position = value;}
            }

            public Quaternion Rotation
            {
                get {return rotation;}
                set {rotation = value;}
            }
        }
    }
}