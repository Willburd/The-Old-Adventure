using System.Numerics;
using Silk.NET.Maths;

// The eternal curse.
// From: https://stackoverflow.com/questions/70462758/c-sharp-how-to-convert-quaternions-to-euler-angles-xyz
namespace Engine
{
    public static partial class Tools
    {
        public static Quaternion Euler(float xangle, float yangle, float zangle)
        {
            return Euler(new Vector3(xangle, yangle, zangle));
        }
        
        public static Quaternion Euler(Vector3 v)
        {
            // We assume degrees.
            v.X = DegreesToRadians(v.X);
            v.Y = DegreesToRadians(v.Y);
            v.Z = DegreesToRadians(v.Z);
            return Radians(v);
        }
        
        public static Quaternion Radians(float xangle, float yangle, float zangle)
        {
            return Radians(new Vector3(xangle, yangle, zangle));
        }

        public static Quaternion Radians(Vector3 v)
        {
            // Four dimensional rotation WOOOOOOOOOOOOO
            float cy = (float)Math.Cos(v.Z * 0.5);
            float sy = (float)Math.Sin(v.Z * 0.5);
            float cp = (float)Math.Cos(v.Y * 0.5);
            float sp = (float)Math.Sin(v.Y * 0.5);
            float cr = (float)Math.Cos(v.X * 0.5);
            float sr = (float)Math.Sin(v.X * 0.5);
            return new Quaternion
            {
                W = cr * cp * cy + sr * sp * sy,
                X = sr * cp * cy - cr * sp * sy,
                Y = cr * sp * cy + sr * cp * sy,
                Z = cr * cp * sy - sr * sp * cy
            };
        }

        public static Vector3 ToRadians(Quaternion q)
        {
            Vector3 angles = new();

            // roll / x
            double sinr_cosp = 2 * (q.W * q.X + q.Y * q.Z);
            double cosr_cosp = 1 - 2 * (q.X * q.X + q.Y * q.Y);
            angles.X = (float)Math.Atan2(sinr_cosp, cosr_cosp);

            // pitch / y
            double sinp = 2 * (q.W * q.Y - q.Z * q.X);
            if (Math.Abs(sinp) >= 1)
            {
                angles.Y = (float)Math.CopySign(Math.PI / 2, sinp);
            }
            else
            {
                angles.Y = (float)Math.Asin(sinp);
            }

            // yaw / z
            double siny_cosp = 2 * (q.W * q.Z + q.X * q.Y);
            double cosy_cosp = 1 - 2 * (q.Y * q.Y + q.Z * q.Z);
            angles.Z = (float)Math.Atan2(siny_cosp, cosy_cosp);

            return angles;
        }

        public static Vector3 ToEulers(Quaternion q)
        {
            Vector3 angles = ToRadians(q);
            angles.X = RadiansToDegrees(angles.X);
            angles.Y = RadiansToDegrees(angles.Y);
            angles.Z = RadiansToDegrees(angles.Z);
            return angles;
        }

        // Yet another yoink from stackoverflow https://stackoverflow.com/questions/12435671/quaternion-lookat-function
        /// <summary>
        /// Evaluates a rotation needed to be applied to an object positioned at sourcePoint to face destPoint
        /// </summary>
        /// <param name="sourcePoint">Coordinates of source point</param>
        /// <param name="destPoint">Coordinates of destionation point</param>
        /// <returns></returns>
        public static Quaternion PointTo(Vector3 sourcePoint, Vector3 destPoint)
        {
            Vector3 forwardVector = Vector3.Normalize(destPoint - sourcePoint);

            float dot = Vector3.Dot(Tools.Forward, forwardVector);

            if (Math.Abs(dot - (-1.0f)) < 0.000001f)
            {
                return new Quaternion(0f,-1f,0f,0f);
            }
            if (Math.Abs(dot - (1.0f)) < 0.000001f)
            {
                return Quaternion.Identity;
            }

            float rotAngle = (float)Math.Acos(dot);
            Vector3 rotAxis = Vector3.Normalize(Vector3.Cross(Tools.Forward, forwardVector));
            return Quaternion.Normalize(CreateFromAxisAngle(rotAxis, rotAngle));
        }
        
        /// Similar to above, but the up axis is locked to Y+, TODO - Merge with above and allow specifying an axis of rotation
        public static Quaternion LookAt(Vector3 sourcePoint, Vector3 destPoint)
        {
            if(sourcePoint == destPoint) return Quaternion.Identity;
            // Solve rotation around Y axis
            Vector3 dirvec = Tools.DirVector(sourcePoint, destPoint);
            float rad_angle = MathF.Atan2(dirvec.X, dirvec.Z);
            Quaternion around_y_rot = CreateFromAxisAngle( Tools.Up, rad_angle);
            // Now rotate to face Y axis difference
            dirvec = Vector3.Transform(dirvec, Quaternion.Inverse(around_y_rot));
            return around_y_rot * CreateFromAxisAngle( Tools.Right, -MathF.Atan2(dirvec.Y, dirvec.Z));
        }

        // just in case you need that function also
        public static Quaternion CreateFromAxisAngle(Vector3 axis, float angle)
        {
            float halfAngle = angle * .5f;
            float s = (float)System.Math.Sin(halfAngle);
            Quaternion q;
            q.X = axis.X * s;
            q.Y = axis.Y * s;
            q.Z = axis.Z * s;
            q.W = (float)System.Math.Cos(halfAngle);
            return q;
        }

        public static Quaternion LookAtLockedZ(Vector3 current, Vector3 target)
        {
            current *= new Vector3(1f,0f,1f);
            target *= new Vector3(1f,0f,1f);
            return LookAt(current, target);
        }
    }
}
