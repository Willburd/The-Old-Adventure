using Assets;
using Engine;
using EntComponents;
using Silk.NET.OpenGL;
using System.Numerics;

namespace Rendering
{
    public class FrameBufferContainer : IDisposable
    {
        /// <summary>
        /// Binds the openGL context to the window's framebuffer, the quick and easy way to return to drawing to the main window. 
        /// </summary>
        public static void ResetFrameBuffer()
        {
            // Return to window's buffer
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            Core.OpenGLContext.Viewport(0, 0, Core.DisplayWidth, Core.DisplayHeight);
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
            // Bind for configuration
            GL gl = Core.OpenGLContext;
            Core.OpenGLContext.BindFramebuffer(FramebufferTarget.Framebuffer, _handle);
            Width = wid;
            Height = high;

            // Bind a new texture with the new size
            if (_tex > 0) gl.DeleteTexture(_tex);
            _tex = gl.GenTexture();
            Core.OpenGLContext.BindTexture(TextureTarget.Texture2D, _tex);
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
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)GLEnum.ClampToEdge);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)GLEnum.ClampToEdge);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)GLEnum.Nearest);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)GLEnum.Nearest);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureBaseLevel, 0);
            gl.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMaxLevel, 0);

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
            Core.OpenGLContext.BindRenderbuffer(RenderbufferTarget.Renderbuffer, _depthbuffer);
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
        public void BindTexture(int texture_unit = 0)
        {
            Core.OpenGLContext.ActiveTexture(TextureData.IntToUnit(texture_unit));
            Core.OpenGLContext.BindTexture(TextureTarget.Texture2D, _tex);
        }

        public void Render(double tick_delta, Vector2 offset, Vector2 cut_pos, Vector2 cut_size)
        {
            List<ShaderData.Uniform> vertex_uniforms = [];
            
            WorldRender.CreateBaseUniforms2D(Matrix4x4.Identity * Matrix4x4.CreateScale(new Vector3(1f, 1f, 1f)) * Matrix4x4.CreateTranslation(new Vector3(offset.X, offset.Y, Core.SpriteRenderDepthOffset)), vertex_uniforms);
            Core.RenderSprite(this, new Vector3(offset.X, offset.Y, 0f), vertex_uniforms);
        }

        public void Render(double tick_delta)
        {
            Render(tick_delta, Vector2.Zero, Vector2.Zero, Vector2.One);
        }

        public void Render(FrameBufferContainer to_buffer, double tick_delta, Vector2 offset, Vector2 cut_pos, Vector2 cut_size)
        {
            to_buffer.BindFrameBuffer();
            Core.OpenGLContext.Clear(ClearBufferMask.DepthBufferBit | ClearBufferMask.StencilBufferBit);
            Core.SpriteRenderDepthOffset = 0;
            Render(tick_delta, offset, cut_pos, cut_size);
        }

        public void Render(FrameBufferContainer to_buffer, double tick_delta)
        {
            Render(to_buffer, tick_delta, Vector2.Zero, Vector2.Zero, Vector2.One);
        }
        
        public void Clear()
        {
            BindFrameBuffer();
            Core.OpenGLContext.ClearColor(System.Drawing.Color.FromArgb(0, 0, 0, 0));
            Core.OpenGLContext.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit | ClearBufferMask.StencilBufferBit);
            Core.SpriteRenderDepthOffset = 0;
        }

        public uint Handle()
        {
            return _handle;
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