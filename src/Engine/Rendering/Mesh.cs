// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Silk.NET.OpenGL;

namespace Rendering
{
    public class MeshData : IDisposable
    {
        public MeshData(GL gl, float[] vertices, uint[] indices)
        {
            GL = gl;
            Vertices = vertices;
            Indices = indices;
            SetupMesh();
        }

        public float[] Vertices { get; private set; }
        public uint[] Indices { get; private set; }
        public VertexArrayObject<float, uint>? VAO { get; set; }
        public BufferObject<float>? VBO { get; set; }
        public BufferObject<uint>? EBO { get; set; }
        public GL GL { get; }

        public unsafe void SetupMesh()
        {
            EBO = new BufferObject<uint>(GL, Indices, BufferTargetARB.ElementArrayBuffer);
            VBO = new BufferObject<float>(GL, Vertices, BufferTargetARB.ArrayBuffer);
            VAO = new VertexArrayObject<float, uint>(GL, VBO, EBO);
            VAO.VertexAttributePointer(0, 3, VertexAttribPointerType.Float, 5, 0);
            VAO.VertexAttributePointer(1, 2, VertexAttribPointerType.Float, 5, 3);
        }

        public void Bind()
        {
            VAO?.Bind();
        }

        public void Dispose()
        {
            VAO?.Dispose();
            VBO?.Dispose();
            EBO?.Dispose();
        }
    }
}