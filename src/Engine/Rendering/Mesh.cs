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

        private struct VBOInit(int element_count, VertexAttribPointerType type, bool normalized, uint size)
        {   
            public int element_count = element_count;
            public VertexAttribPointerType type = type;
            public bool normalized = normalized;
            public uint size = size;
        };

        public unsafe void SetupMesh()
        {
            EBO = new BufferObject<uint>(GL, Indices, BufferTargetARB.ElementArrayBuffer);
            VBO = new BufferObject<float>(GL, Vertices, BufferTargetARB.ArrayBuffer);
            VAO = new VertexArrayObject<float, uint>(GL, VBO, EBO);

            // Attribute format
            uint total_vertex_size = 0;
            List<VBOInit> buffersizes = [
                new VBOInit(3, VertexAttribPointerType.Float, false, 3), // Position
                new VBOInit(2, VertexAttribPointerType.Float, true,  2)  // UV
            ];

            // Self assemble it...
            foreach(VBOInit vbo_dat in buffersizes)
            {
                total_vertex_size += vbo_dat.size;
            }
            uint current_offset = 0;
            uint current_index = 0;
            foreach(VBOInit vbo_dat in buffersizes)
            {
                VAO.VertexAttributePointer(current_index, vbo_dat.element_count, vbo_dat.type, vbo_dat.normalized, total_vertex_size, current_offset);
                current_offset += vbo_dat.size;
                current_index++;
            }
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