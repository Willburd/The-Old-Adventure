using System.Drawing;
using System.Numerics;

namespace Engine
{
    public static partial class Tools
    {
        public static Vector3 ColorToVector(Color color)
        {
            return new Vector3(Color.CornflowerBlue.R / 255f, Color.CornflowerBlue.G / 255f, Color.CornflowerBlue.B / 255f);
        }
    }
}