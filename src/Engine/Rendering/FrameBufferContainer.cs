using Engine;
using Silk.NET.OpenGL;

namespace Rendering
{
    public class FrameBufferContainer : IDisposable
    {
        public static void BindDefaultFrameBuffer()
        {
            // Return to window's buffer
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
        }

        private uint _handle;
        private uint _tex;
        private uint _renderbuffer;

        public uint Width { get; private set; }
        public uint Height { get; private set; }

        public FrameBufferContainer(uint width, uint height)
        {
            GL gl = Core.OpenGLContext;
            _handle = gl.GenFramebuffer();
            Resize(width, height);
        }

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

        public void Bind()
        {
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, _handle);
            Core.OpenGLContext.Viewport(0, 0, Width, Height);
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