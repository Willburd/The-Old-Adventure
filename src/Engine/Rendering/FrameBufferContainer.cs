using Engine;
using Silk.NET.OpenGL;

namespace Rendering
{
    public class FrameBufferContainer : IDisposable
    {
        /// <summary>
        /// Binds the openGL context to the window's framebuffer, the quick and easy way to return to drawing to the main window. 
        /// </summary>
        public static void BindDefaultFrameBuffer()
        {
            // Return to window's buffer
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
        }

        // openGL context handles
        private uint _handle;
        private uint _tex;
        private uint _renderbuffer;

        // Size of the texture and buffer in pixels
        public uint Width { get; private set; }
        public uint Height { get; private set; }

        public FrameBufferContainer(uint width, uint height)
        {
            GL gl = Core.OpenGLContext;
            _handle = gl.GenFramebuffer();
            Resize(width, height);
        }

        /// <summary>
        /// Resizes the framebuffer and it's texture, completely rebuilding all the data in it. Clearing the data to garbage. Be sure to rerender before use.
        /// </summary>
        public unsafe void Resize(uint wid, uint high)
        {
            GL gl = Core.OpenGLContext;
            Width = wid;
            Height = high;

            // Clear texture on resize
            if (_tex > 0) gl.DeleteTexture(_tex);
            _tex = gl.GenTexture();
            gl.TexImage2D(
                TextureTarget.Texture2D,
                0,
                InternalFormat.Rgba,
                Width,
                Height,
                0,
                PixelFormat.Rgba,
                PixelType.UnsignedByte,
                null);

            // Filtering mode
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)GLEnum.Nearest);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)GLEnum.Nearest);

            // Bind texture to fbo
            gl.FramebufferTexture2D(
                FramebufferTarget.Framebuffer,
                FramebufferAttachment.ColorAttachment0,
                TextureTarget.Texture2D,
                _tex,
                0);

            // Depth buffer by default
            if (_renderbuffer > 0) gl.DeleteRenderbuffer(_renderbuffer);
            _renderbuffer = gl.GenRenderbuffer();
            gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, _renderbuffer);
            gl.RenderbufferStorage(
                RenderbufferTarget.Renderbuffer,
                InternalFormat.Depth24Stencil8,
                Width,
                Height);

            // Binding depth to buffer
            gl.FramebufferRenderbuffer(
                FramebufferTarget.Framebuffer,
                FramebufferAttachment.DepthStencilAttachment,
                RenderbufferTarget.Renderbuffer,
                _renderbuffer);

            if (gl.CheckFramebufferStatus(FramebufferTarget.Framebuffer) != GLEnum.FramebufferComplete) throw new Exception("Framebuffer not complete");
        }

        /// <summary>
        /// Bind framebuffer to the GL context so that things can be rendered into its texture.
        /// </summary>
        public void BindFrameBuffer()
        {
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, _handle);
            Core.OpenGLContext.Viewport(0, 0, Width, Height);
        }

        /// <summary>
        /// Bind the buffer's texture to a vertex unit so that it can be rendered by a shader onto a mesh.
        /// </summary>
        public void BindTexture(int texture_unit = 0)
        {
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
            Core.OpenGLContext.ActiveTexture(textureSlot);
            Core.OpenGLContext.BindTexture(TextureTarget.Texture2D, _tex);
        }

        public void Dispose()
        {
            GL gl = Core.OpenGLContext;
            gl.DeleteRenderbuffer(_renderbuffer);
            gl.DeleteTexture(_tex);
            gl.DeleteFramebuffer(_handle);
        }
    }
}