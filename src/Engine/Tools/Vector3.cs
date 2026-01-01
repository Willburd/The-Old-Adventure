using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static Vector3 Up {
            get
            {
                return new Vector3(0f,1f,0f);
            }
        }
        
        public static Vector3 Down {
            get
            {
                return new Vector3(0f,-1f,0f);
            }
        }
        
        public static Vector3 Forward {
            get
            {
                return new Vector3(0f,0f,1f);
            }
        }
        
        public static Vector3 Backward {
            get
            {
                return new Vector3(0f,0f,-1f);
            }
        }
        
        public static Vector3 Right {
            get
            {
                return new Vector3(1f,0f,0f);
            }
        }
        
        public static Vector3 Left {
            get
            {
                return new Vector3(-1f,0f,0f);
            }
        }
    }
}