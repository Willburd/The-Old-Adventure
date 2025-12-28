namespace Engine
{
    public static partial class Tools
    {
        public static float DegreesToRadians(float degrees)
        {
            return MathF.PI / 180f * degrees;
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
    }
}