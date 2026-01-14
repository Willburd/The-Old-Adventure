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

        public static Random Rand { get; private set; } = new Random();

        public static double RandRange(double min = 0, double max = 1)
        {
            return double.Lerp(min, max, Rand.NextDouble());
        }

        public static float RandRange(float min = 0, float max = 1)
        {
            return float.Lerp(min, max, (float)Rand.NextDouble());
        }

        public static float Decelerate(float org, float decel)
        {
            int sign = MathF.Sign(org);
            float abs_org = MathF.Abs(org);
            abs_org -= MathF.Abs(decel);
            if (abs_org <= 0) abs_org = 0;
            return abs_org * sign;
        }

        public static float Accelerate(float org, float accel, float max_speed)
        {
            if (MathF.Abs(org) < max_speed || MathF.Abs(org + accel) < MathF.Abs(org)) org += accel;
            return org;
        }

        public static Vector3 Decelerate(Vector3 org, float decel)
        {
            if (org.Length() == 0) return org;
            Vector3 org_dir = Vector3.Normalize(org);
            float magnitude = org.Length();
            return org_dir * Decelerate(magnitude, decel);
        }

        public static Vector3 DecelerateFlat(Vector3 org, float decel)
        {
            Vector3 fix_dir = Decelerate(org, decel);
            fix_dir.Y = org.Y;
            return fix_dir;
        }

        public static Vector3 Decelerate(Vector3 org, Vector3 decel)
        {
            return new Vector3(Decelerate(org.X, decel.X), Decelerate(org.Y, decel.Y), Decelerate(org.Z, decel.Z));
        }

        public static Vector3 Accelerate(Vector3 org, Vector3 accel, float max_speed)
        {
            Vector3 new_vec = org + accel;
            if (new_vec.Length() < org.Length()) return new_vec; // Results in a slower vec, allow it
            if (new_vec.Length() < max_speed) return new_vec; // Less than max, allow it
            return org; // At limit
        }
    }
}