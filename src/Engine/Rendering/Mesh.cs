// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Diagnostics;
using System.Numerics;
using Silk.NET.OpenGL;

namespace Rendering
{
    public class MeshData : IDisposable
    {
        public MeshData(GL gl, float[] vertices, uint[] indices, string name, int mesh_index, ModelData owner)
        {
            GL = gl;
            Vertices = vertices;
            Indices = indices;
            
            // Metadata
            MeshIndex = mesh_index;
            RawName = name;

            SetupMesh();

            // materials index as seperate mesh
            foreach(MeshData data in owner.Meshes)
            {
                if(data.RawName == RawName) mesh_name_offset++;
            }
        }

        private int mesh_name_offset = 0;
        public string RawName { get; private set; }
        public string MeshName { get { return RawName + (mesh_name_offset > 0 ? "_"+mesh_name_offset : ""); } }
        public int MeshIndex { get; private set; }

        public float[] Vertices { get; private set; }
        public uint[] Indices { get; private set; }
        public VertexArrayObject<float, uint>? VAO { get; set; }
        public BufferObject<float>? VBO { get; set; }
        public BufferObject<uint>? EBO { get; set; }
        public GL GL { get; }

        public List<Vim.Math3d.Triangle> CollisionTriangles { get; private set; } = [];

        private struct VBOInit(int element_count, VertexAttribPointerType type, bool normalized, uint size, bool is_collision)
        {   
            public int element_count = element_count;
            public VertexAttribPointerType type = type;
            public bool normalized = normalized;
            public uint size = size;
            public bool is_collision = is_collision;
        };

        public void SetupMesh()
        {
            EBO = new BufferObject<uint>(GL, Indices, BufferTargetARB.ElementArrayBuffer);
            VBO = new BufferObject<float>(GL, Vertices, BufferTargetARB.ArrayBuffer);
            VAO = new VertexArrayObject<float, uint>(GL, VBO, EBO);

            // Attribute format
            uint total_vertex_size = 0;
            List<VBOInit> buffersizes = [
                new VBOInit(3, VertexAttribPointerType.Float, false, 3, true), // Position
                new VBOInit(2, VertexAttribPointerType.Float, true,  2, false)  // UV
            ];
            foreach(VBOInit vbo_dat in buffersizes)
            {
                total_vertex_size += vbo_dat.size;
            }

            // Collision creation
            if(RawName == "col.001")
            {
                // collect position data in the verts
                List<Vim.Math3d.Vector3> vert_collection = [];
                int index = 0;
                while(index < Vertices.Length)
                {
                    foreach(VBOInit vbo_dat in buffersizes)
                    {
                        if(vbo_dat.is_collision)
                        {
                            Debug.Assert(vbo_dat.element_count == 3, "VBO for collision data expects to be 3 elements long");
                            vert_collection.Add( new( Vertices[index++], Vertices[index++], Vertices[index++] ));
                        }
                    }
                }
                // Assemble position floats into tris
                if(Indices.Length > 0)
                {
                    index = 0;
                    while(index < Indices.Length)
                    {
                        Vim.Math3d.Vector3 vertA = vert_collection[ (int)Indices[index++]];
                        Vim.Math3d.Vector3 vertB = vert_collection[ (int)Indices[index++]];
                        Vim.Math3d.Vector3 vertC = vert_collection[ (int)Indices[index++]];
                        CollisionTriangles.Add( new Vim.Math3d.Triangle(vertA,vertB,vertC) );
                    }
                }
            }
            // Self assemble it...
            uint current_offset = 0;
            uint current_index = 0;
            foreach(VBOInit vbo_dat in buffersizes)
            {
                VAO.VertexAttributePointer(current_index, vbo_dat.element_count, vbo_dat.type, vbo_dat.normalized, total_vertex_size, current_offset);
                current_offset += vbo_dat.size;
                current_index++;
            }

            // We don't want other meshes to cross contaminate
            GL.BindVertexArray(0);
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