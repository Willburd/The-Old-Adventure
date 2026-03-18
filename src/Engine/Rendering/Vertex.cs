// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Numerics;

namespace Rendering
{
    public struct Vertex
    {
        public Vector3 Position;
        public Vector3 Normal;
        public Vector3 Color;
        public Vector3 Tangent;
        public Vector2 TexCoords;
        public Vector3 Bitangent;

        public const int MAX_BONE_INFLUENCE = 4;
        public uint[] BoneIds;
        public float[] Weights;
    }
}