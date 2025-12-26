// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Diagnostics;
using Engine;
using Silk.NET.Assimp;
using Silk.NET.OpenGL;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Rendering
{
    public class TextureData : IDisposable
    {
        private uint _handle;
        private GL _gl;

        public string Path { get; set; } = "NO PATH";
        public TextureType Type { get; }

        public unsafe TextureData(string path)
        {
            _gl = Core.OpenGLContext;
            Path = path;
            _handle = _gl.GenTexture();

            Bind();

            Debug.Assert(System.IO.File.Exists(path),"Texture Asset file does not exist : " + path);

            using (var img = Image.Load<Rgba32>(path))
            {
                _gl.TexImage2D(TextureTarget.Texture2D, 0, InternalFormat.Rgba8, (uint) img.Width, (uint) img.Height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, null);
                img.ProcessPixelRows(accessor =>
                {
                    for (int y = 0; y < accessor.Height; y++)
                    {
                        fixed (void* data = accessor.GetRowSpan(y))
                        {
                            _gl.TexSubImage2D(TextureTarget.Texture2D, 0, 0, y, (uint) accessor.Width, 1, PixelFormat.Rgba, PixelType.UnsignedByte, data);
                        }
                    }
                });
            }

            SetParameters();
        }

        public unsafe TextureData(Span<byte> data, uint width, uint height)
        {
            _gl = Core.OpenGLContext;
            _handle = _gl.GenTexture();

            Bind();

            fixed (void* d = &data[0])
            {
                _gl.TexImage2D(TextureTarget.Texture2D, 0, (int) InternalFormat.Rgba, width, height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, d);
                SetParameters();
            }
        }

        private void SetParameters()
        {
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int) GLEnum.ClampToEdge);
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int) GLEnum.ClampToEdge);
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int) GLEnum.LinearMipmapLinear);
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int) GLEnum.Linear);
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureBaseLevel, 0);
            _gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMaxLevel, 8);
            _gl.GenerateMipmap(TextureTarget.Texture2D);
        }

        public void Bind(TextureUnit textureSlot = TextureUnit.Texture0)
        {
            _gl.ActiveTexture(textureSlot);
            _gl.BindTexture(TextureTarget.Texture2D, _handle);
        }

        public bool IsValid()
        {
            return _handle > 0;
        }

        public void Dispose()
        {
            _gl.DeleteTexture(_handle);
        }
    }
}