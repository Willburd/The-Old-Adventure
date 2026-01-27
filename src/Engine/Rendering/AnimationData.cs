
using System.Numerics;

namespace Rendering
{
    public struct AnimationData
    {
        public uint Index;
        public string Name;

        public List<PositionFrame> pos_frames;
        public List<RotationFrame> rot_frames;
        public List<ScaleFrame> scl_frames;



        public struct PositionFrame()
        {
            public int Time;
            public Vector3 Position;
        }

        public struct RotationFrame()
        {
            public int Time;
            public Quaternion Rotation;

        }

        public struct ScaleFrame()
        {
            public int Time;
            public Vector3 Scale;
        }
    }
}