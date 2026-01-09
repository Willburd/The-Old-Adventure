using System.Drawing;
using System.Numerics;
using Rendering;

namespace Environments
{
    public class Environment
    {
        public float FogDistance { get; set; } = 100f;

        public Color FogColor { get; set; } = Color.CornflowerBlue;

        public Vector4 AmbientLight { get; set; } = new Vector4(1f,1f,1f,1f);
    }
}