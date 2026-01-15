using Assets;
using Engine;
using Silk.NET.OpenGL;
using System.Numerics;

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
        private uint _depthbuffer;

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
            if (_depthbuffer > 0) gl.DeleteRenderbuffer(_depthbuffer);
            _depthbuffer = gl.GenRenderbuffer();
            gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, _depthbuffer);
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
                _depthbuffer);

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
        public void BindTexture()
        {
            Core.OpenGLContext.ActiveTexture(TextureData.IntToUnit(0));
            Core.OpenGLContext.BindTexture(TextureTarget.Texture2D, _tex);
        }

        public void Render(double tick_delta)
        {
            List<ShaderData.Uniform> vertex_uniforms = [];
            vertex_uniforms.Add(new("uTransform", Matrix4x4.Identity));
            vertex_uniforms.Add(new("uProjection", Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(45f), Core.DisplayAspectRatio, 0.0001f, 1000f)));
            vertex_uniforms.Add(new("uView", Matrix4x4.CreateFromQuaternion(Quaternion.Identity) * Matrix4x4.CreateTranslation(Tools.Forward)));
            Core.RenderSprite(this, vertex_uniforms);
        }

        public uint Handle()
        {
            return _handle;
        }

        public uint Texture()
        {
            return _tex;
        }

        public bool IsValid()
        {
            return _handle > 0 && _tex > 0 && _depthbuffer > 0;
        }

        public void Dispose()
        {
            GL gl = Core.OpenGLContext;
            gl.DeleteRenderbuffer(_depthbuffer);
            gl.DeleteTexture(_tex);
            gl.DeleteFramebuffer(_handle);
        }
    }
}