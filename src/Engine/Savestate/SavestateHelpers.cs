namespace Engine
{
    public static partial class Savestate
    {
        public static bool PlayerAlive()
        {
            return GetValue(player_health) > 0;
        }
        
        public static bool PlayerDead()
        {
            return GetValue(player_health) <= 0;
        }
        
        public static bool PlayerFullHealth()
        {
            return GetValue(player_health) == GetValue(player_max_health);
        }
        
        public static int PlayerHeartCount()
        {
            return (int)MathF.Floor(GetValue(player_health) / 4f);
        }

        public static int PlayerMaxHeartCount()
        {
            return (int)MathF.Floor(GetValue(player_max_health) / 4f);
        }

        public static float PlayerHealthPercent()
        {
            return Math.Clamp(GetValue(player_health) / GetValue(player_max_health), 0f, 1f);
        }
    }
}