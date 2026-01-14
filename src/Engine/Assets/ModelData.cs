// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Silk.NET.Assimp;
using Silk.NET.OpenGL;
using System.Numerics;
using Rendering;
using AssimpMesh = Silk.NET.Assimp.Mesh;

namespace Assets
{
    public class ModelData : IDisposable
    {
        public ModelData(string path)
        {
            var assimp = Assimp.GetApi();
            _assimp = assimp;
            _gl = Engine.Core.OpenGLContext;
            LoadModel(path);
        }

        private readonly GL _gl;
        private Assimp _assimp;
        private List<TextureData> _texturesLoaded = new List<TextureData>();
        public List<MeshData> Meshes { get; protected set; } = new List<MeshData>();
        
        private unsafe void LoadModel(string path)
        {
            var scene = _assimp.ImportFile(path, (uint)PostProcessSteps.FixInFacingNormals | (uint)PostProcessSteps.CalculateTangentSpace | (uint)PostProcessSteps.FindInstances);

            if (scene == null || scene->MFlags == Assimp.SceneFlagsIncomplete || scene->MRootNode == null)
            {
                var error = _assimp.GetErrorStringS();
                throw new Exception(error);
            }

            ProcessNode(scene->MRootNode, scene);
        }

        private unsafe void ProcessNode(Node* node, Scene* scene)
        {
            for (var i = 0; i < node->MNumMeshes; i++)
            {
                var mesh = scene->MMeshes[node->MMeshes[i]];
                Meshes.Add(ProcessMesh(mesh, scene, mesh->MName, Meshes.Count));
            }

            for (var i = 0; i < node->MNumChildren; i++)
            {
                ProcessNode(node->MChildren[i], scene);
            }
        }

        private unsafe MeshData ProcessMesh(AssimpMesh* mesh, Scene* scene, string name, int index)
        {
            // data to fill
            List<Vertex> vertices = new List<Vertex>();
            List<uint> indices = new List<uint>();

            // walk through each of the mesh's vertices
            for (uint i = 0; i < mesh->MNumVertices; i++)
            {
                Vertex vertex = new Vertex();
                vertex.BoneIds = new int[Vertex.MAX_BONE_INFLUENCE];
                vertex.Weights = new float[Vertex.MAX_BONE_INFLUENCE];

                vertex.Position = mesh->MVertices[i];

                // normals
                if (mesh->MNormals != null)
                    vertex.Normal = mesh->MNormals[i];
                // tangent
                if (mesh->MTangents != null)
                    vertex.Tangent = mesh->MTangents[i];
                // bitangent
                if (mesh->MBitangents != null)
                    vertex.Bitangent = mesh->MBitangents[i];
                // colors
                if (mesh->MColors[0] != null)
                {
                    Vector4 colorvert = mesh->MColors[0][i];
                    vertex.Color = colorvert;
                }
                else
                {
                    // default vert color if none packed
                    vertex.Color = new Vector4(1f,1f,1f,0f);
                }
                // texture coordinates
                if (mesh->MTextureCoords[0] != null) // does the mesh contain texture coordinates
                {
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    Vector3 texcoord3 = mesh->MTextureCoords[0][i];
                    vertex.TexCoords = new Vector2(texcoord3.X, texcoord3.Y);
                }

                vertices.Add(vertex);
            }

            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (uint i = 0; i < mesh->MNumFaces; i++)
            {
                Face face = mesh->MFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (uint j = 0; j < face.MNumIndices; j++)
                    indices.Add(face.MIndices[j]);
            }

            // return a mesh object created from the extracted mesh data
            var result = new MeshData(_gl, BuildVertices(vertices), BuildIndices(indices), name, index, this);
            return result;
        }

        private float[] BuildVertices(List<Vertex> vertexCollection)
        {
            var vertices = new List<float>();

            foreach (var vertex in vertexCollection)
            {
                // Position
                vertices.Add(vertex.Position.X);
                vertices.Add(vertex.Position.Y);
                vertices.Add(vertex.Position.Z);
                // UV
                vertices.Add(vertex.TexCoords.X);
                vertices.Add(vertex.TexCoords.Y);
                // Normals
                vertices.Add(vertex.Normal.X);
                vertices.Add(vertex.Normal.Y);
                vertices.Add(vertex.Normal.Z);
                // Colors
                vertices.Add(vertex.Color.X);
                vertices.Add(vertex.Color.Y);
                vertices.Add(vertex.Color.Z);
                vertices.Add(vertex.Color.W);
            }

            return vertices.ToArray();
        }

        private uint[] BuildIndices(List<uint> indices)
        {
            return indices.ToArray();
        }

        public void Dispose()
        {
            foreach (var mesh in Meshes)
            {
                mesh.Dispose();
            }
            _texturesLoaded = [];
        }

        public bool IsValid()
        {
            return Meshes.Count > 0;
        }

        public List<string> GetMeshNames()
        {
            List<string> names = [];
            foreach(MeshData mesh in Meshes)
            {
                names.Add(mesh.MeshName);
            }
            return names;
        }

        public int GetMeshIndex(string mesh_name)
        {
            int index = 0;
            foreach(MeshData mesh in Meshes)
            {
                if(mesh_name == mesh.MeshName)
                {
                    return index;
                }
                index++;
            }
            return -1;
        }

        public void DebugMeshNames()
        {
            int index = 0;
            foreach(string name in GetMeshNames())
            {
                Console.WriteLine("mesh index [" + index + "] -> " + name);
                index++;
            }
        }
    }
}