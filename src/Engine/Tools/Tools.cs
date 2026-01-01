using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static float DegreesToRadians(float degrees)
        {
            return degrees * (MathF.PI / 180f);
        }

        public static float RadiansToDegrees(float radians)
        {
            return radians * (180f / MathF.PI);
        }

        public static Random Rand {get; private set;} = new Random();

        public static double RandRange(double min = 0, double max = 1)
        {
            return double.Lerp(min,max, Rand.NextDouble());
        }

        public static float RandRange(float min = 0, float max = 1)
        {
            return float.Lerp(min,max, (float)Rand.NextDouble());
        }

        public static float Decelerate(float org, float decel)
        {
            int sign = MathF.Sign(org);
            float abs_org = MathF.Abs(org);
            abs_org -= decel;
            if(abs_org <= 0) abs_org = 0;
            return abs_org * sign;
        }

        public static Vector3 Decelerate(Vector3 org, float decel)
        {
            return new Vector3(Decelerate(org.X, decel), Decelerate(org.Y, decel), Decelerate(org.Z, decel));
        }
        
        public static Vector3 Decelerate(Vector3 org, Vector3 decel)
        {
            return new Vector3(Decelerate(org.X, decel.X), Decelerate(org.Y, decel.Y), Decelerate(org.Z, decel.Z));
        }
    }
}