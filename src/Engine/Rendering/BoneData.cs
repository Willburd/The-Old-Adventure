
using System.Numerics;

namespace Rendering
{
    public struct BoneData
    {
        public Matrix4x4 Offset { get; set; }
        public Matrix4x4 CurrentTransform { get; set; }
        public string Name { get; set; }
    }
}