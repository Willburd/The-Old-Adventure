using System.Drawing;
using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static Vector4 ColorToVector(Color color)
        {
            return new Vector4(Color.CornflowerBlue.R/255f, Color.CornflowerBlue.G/255f, Color.CornflowerBlue.B/255f, Color.CornflowerBlue.A/255f);
        }
    }
}