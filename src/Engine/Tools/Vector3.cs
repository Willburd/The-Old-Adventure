using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static Vector3 Up
        {
            get
            {
                return new Vector3(0f, 1f, 0f);
            }
        }

        public static Vector3 Down
        {
            get
            {
                return new Vector3(0f, -1f, 0f);
            }
        }

        public static Vector3 Forward
        {
            get
            {
                return new Vector3(0f, 0f, -1f);
            }
        }

        public static Vector3 Backward
        {
            get
            {
                return new Vector3(0f, 0f, 1f);
            }
        }

        public static Vector3 Right
        {
            get
            {
                return new Vector3(1f, 0f, 0f);
            }
        }

        public static Vector3 Left
        {
            get
            {
                return new Vector3(-1f, 0f, 0f);
            }
        }

        public static Vector3 DirVector(Vector3 start, Vector3 end)
        {
            return Vector3.Normalize(end - start);
        }
        
        public static Vector3 FlatDirVector(Vector3 start, Vector3 end)
        {
            Vector3 vec = Vector3.Normalize(end - start);
            vec.Y = 0;
            return vec;
        }

        public static float FlatDistance(Vector3 start, Vector3 end)
        {
            start.Y = 0;
            end.Y = 0;
            return Vector3.Distance(start, end);
        }
    }
}