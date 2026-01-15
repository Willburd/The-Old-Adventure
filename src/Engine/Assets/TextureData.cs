// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Diagnostics;
using Engine;
using Silk.NET.Assimp;
using Silk.NET.OpenGL;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Assets
{
    public class TextureData : IDisposable
    {
        private uint _handle;
        private GL _gl;

        public string Path { get; set; } = "NO PATH";
        public TextureType Type { get; }

        public TextureTarget TexTarget { get; private set; }

        public unsafe TextureData(string path, TextureTarget tex_targ)
        {
            _gl = Core.OpenGLContext;
            Path = path;
            TexTarget = tex_targ;
            _handle = _gl.GenTexture();

            Bind();

            Debug.Assert(System.IO.File.Exists(path), "Texture Assets file does not exist : " + path);

            using (var img = Image.Load<Rgba32>(path))
            {
                _gl.TexImage2D(TexTarget, 0, InternalFormat.Rgba8, (uint)img.Width, (uint)img.Height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, null);
                img.ProcessPixelRows(accessor =>
                {
                    for (int y = 0; y < accessor.Height; y++)
                    {
                        fixed (void* data = accessor.GetRowSpan(y))
                        {
                            // Flip texture on import. Because there is literally no stanrdards in 3D software, GL uses bottom left, d3d uses top left.
                            _gl.TexSubImage2D(TexTarget, 0, 0, (accessor.Height - 1) - y, (uint)accessor.Width, 1, PixelFormat.Rgba, PixelType.UnsignedByte, data);
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
                _gl.TexImage2D(TexTarget, 0, (int)InternalFormat.Rgba, width, height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, d);
                SetParameters();
            }
        }

        private void SetParameters()
        {
            _gl.TexParameter(TexTarget, TextureParameterName.TextureWrapS, (int)GLEnum.ClampToEdge);
            _gl.TexParameter(TexTarget, TextureParameterName.TextureWrapT, (int)GLEnum.ClampToEdge);
            _gl.TexParameter(TexTarget, TextureParameterName.TextureMinFilter, (int)GLEnum.LinearMipmapLinear);
            _gl.TexParameter(TexTarget, TextureParameterName.TextureMagFilter, (int)GLEnum.Linear);
            _gl.TexParameter(TexTarget, TextureParameterName.TextureBaseLevel, 0);
            _gl.TexParameter(TexTarget, TextureParameterName.TextureMaxLevel, 8);
            _gl.GenerateMipmap(TexTarget);
        }

        public static TextureUnit IntToUnit(int texture_unit)
        {
            // No this can't just be casted from an int, they're all offsets stored in an enum.
            var textureSlot = texture_unit switch
            {
                1 => TextureUnit.Texture1,
                2 => TextureUnit.Texture2,
                3 => TextureUnit.Texture3,
                4 => TextureUnit.Texture4,
                5 => TextureUnit.Texture5,
                6 => TextureUnit.Texture6,
                7 => TextureUnit.Texture7,
                8 => TextureUnit.Texture8,
                9 => TextureUnit.Texture9,
                10 => TextureUnit.Texture10,
                11 => TextureUnit.Texture11,
                12 => TextureUnit.Texture12,
                13 => TextureUnit.Texture13,
                14 => TextureUnit.Texture14,
                15 => TextureUnit.Texture15,
                16 => TextureUnit.Texture16,
                17 => TextureUnit.Texture17,
                18 => TextureUnit.Texture18,
                19 => TextureUnit.Texture19,
                20 => TextureUnit.Texture20,
                21 => TextureUnit.Texture21,
                22 => TextureUnit.Texture22,
                23 => TextureUnit.Texture23,
                24 => TextureUnit.Texture24,
                25 => TextureUnit.Texture25,
                26 => TextureUnit.Texture26,
                27 => TextureUnit.Texture27,
                28 => TextureUnit.Texture28,
                29 => TextureUnit.Texture29,
                30 => TextureUnit.Texture30,
                31 => TextureUnit.Texture31,
                _ => TextureUnit.Texture0,
            };
            return textureSlot;
        }

        public void Bind(int texture_unit = 0)
        {
            _gl.ActiveTexture(IntToUnit(texture_unit));
            _gl.BindTexture(TexTarget, _handle);
        }

        public bool IsValid()
        {
            return _handle > 0;
        }

        public uint Handle()
        {
            return _handle;
        }

        public void Dispose()
        {
            _gl.DeleteTexture(_handle);
        }
    }
}