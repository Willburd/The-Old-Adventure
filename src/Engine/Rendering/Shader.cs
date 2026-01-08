// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Numerics;
using Silk.NET.OpenGL;

namespace Rendering
{
    public class ShaderData : IDisposable
    {
        private uint _handle;
        private GL _gl;

        public ShaderData(string vertexPath, string fragmentPath)
        {
            _gl = Engine.Core.OpenGLContext;

            uint vertex = LoadShader(ShaderType.VertexShader, vertexPath);
            uint fragment = LoadShader(ShaderType.FragmentShader, fragmentPath);
            _handle = _gl.CreateProgram();
            _gl.AttachShader(_handle, vertex);
            _gl.AttachShader(_handle, fragment);
            _gl.LinkProgram(_handle);
            _gl.GetProgram(_handle, GLEnum.LinkStatus, out var status);
            if (status == 0)
            {
                throw new Exception($"Program failed to link with error: {_gl.GetProgramInfoLog(_handle)}");
            }
            _gl.DetachShader(_handle, vertex);
            _gl.DetachShader(_handle, fragment);
            _gl.DeleteShader(vertex);
            _gl.DeleteShader(fragment);
        }

        public void Use()
        {
            _gl.UseProgram(_handle);
        }

        public bool IsValid()
        {
            return _gl.IsProgram(_handle);
        }

        public unsafe void SetUniform(string name, object value)
        {
            int location = _gl.GetUniformLocation(_handle, name);
            if (location == -1)
            {
                throw new Exception($"{name} uniform not found on shader.");
            }
            // Numeral
            if(value.GetType() == typeof(int))
            {
                _gl.Uniform1(location, (int)value);
                return;
            }
            if(value.GetType() == typeof(float)) 
            {
                _gl.Uniform1(location, (float)value);
                return;
            }
            if(value.GetType() == typeof(double)) 
            {
                _gl.Uniform1(location, (double)value);
                return;
            }
            // Vectors
            if(value.GetType() == typeof(Vector2)) 
            {
                _gl.Uniform2(location, (Vector2)value);
                return;
            }
            if(value.GetType() == typeof(Vector3)) 
            {
                _gl.Uniform3(location, (Vector3)value);
                return;
            }
            if(value.GetType() == typeof(Vector4)) 
            {
                _gl.Uniform4(location, (Vector4)value);
                return;
            }
            if(value.GetType() == typeof(Matrix4x4)) 
            {
                Matrix4x4 mat_data = (Matrix4x4)value;
                _gl.UniformMatrix4(location, 1, false, (float*) &mat_data);
                return;
            }
            // Arrays
            if(value.GetType() == typeof(Vector2[])) 
            {
                Vector2[] data = (Vector2[])value;
                _gl.Uniform3(location, (uint)data.Length, (double*) &data);
                return;
            }
            if(value.GetType() == typeof(Vector3[])) 
            {
                Vector3[] data = (Vector3[])value;
                _gl.Uniform3(location, (uint)data.Length, (double*) &data);
                return;
            }
            if(value.GetType() == typeof(Vector4[])) 
            {
                Vector4[] data = (Vector4[])value;
                _gl.Uniform4(location, (uint)data.Length, (double*) &data);
                return;
            }
        }

        public void Dispose()
        {
            _gl.DeleteProgram(_handle);
        }

        private uint LoadShader(ShaderType type, string path)
        {
            string src = File.ReadAllText(path);
            uint handle = _gl.CreateShader(type);
            _gl.ShaderSource(handle, src);
            _gl.CompileShader(handle);
            string infoLog = _gl.GetShaderInfoLog(handle);
            if (!string.IsNullOrWhiteSpace(infoLog))
            {
                throw new Exception($"Error compiling shader of type {type}, failed with error {infoLog}");
            }

            return handle;
        }
    }
}