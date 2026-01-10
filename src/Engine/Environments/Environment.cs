using System.Drawing;
using System.Numerics;
using Rendering;

namespace Environments
{
    public class Environment(Vector4 fog_col, float fog_dist, Vector4 ambient_light)
    {
        public float FogDistance { get; set; } = fog_dist;

        public Vector4 FogColor { get; set; } = fog_col;

        public Vector4 AmbientLight { get; set; } = ambient_light;
    }
}