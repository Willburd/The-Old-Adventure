using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static float DegreesToRadians(float degrees)
        {
            return MathF.PI / 180f * degrees;
        }
    }
}