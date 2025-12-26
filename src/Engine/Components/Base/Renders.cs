using Engine;
using Silk.NET.OpenGL;

namespace EntComponents
{
    public class Renders(Entity host_entity) : EntComponent(host_entity)
    {
#pragma warning disable CS8618 // We get this during binding InitBindings() due to constructor call order being unsuitable for our needs
        protected GL Gl;
#pragma warning restore CS8618

        protected uint Vbo;
        protected uint Ebo;
        protected uint Vao;
        protected Rendering.Shader Shader;

        //Vertex data, uploaded to the VBO.
        protected float[] Vertices = [];

        //Index data, uploaded to the EBO.
        protected uint[] Indices = [];

        public unsafe void InitBindings()
        {
            // Set context
            Gl = Core.OpenGLContext;
            
            //Creating a vertex array.
            Vao = Gl.GenVertexArray();
            Gl.BindVertexArray(Vao);

            //Initializing a vertex buffer that holds the vertex data.
            Vbo = Gl.GenBuffer(); //Creating the buffer.
            Gl.BindBuffer(BufferTargetARB.ArrayBuffer, Vbo); //Binding the buffer.
            fixed (void* v = &Vertices[0])
            {
                Gl.BufferData(BufferTargetARB.ArrayBuffer, (nuint) (Vertices.Length * sizeof(uint)), v, BufferUsageARB.StaticDraw); //Setting buffer data.
            }

            //Initializing a element buffer that holds the index data.
            Ebo = Gl.GenBuffer(); //Creating the buffer.
            Gl.BindBuffer(BufferTargetARB.ElementArrayBuffer, Ebo); //Binding the buffer.
            fixed (void* i = &Indices[0])
            {
                Gl.BufferData(BufferTargetARB.ElementArrayBuffer, (nuint) (Indices.Length * sizeof(uint)), i, BufferUsageARB.StaticDraw); //Setting buffer data.
            }
        }

        public override void OnDestroy()
        {
            Gl.DeleteBuffer(Vbo);
            Gl.DeleteBuffer(Ebo);
            Gl.DeleteVertexArray(Vao);
        }

        public bool Visible { get; set; } = true;
        private int Priority { get; set; } = 1;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.render_priority,Core.Signals.render];
        }

        public override int ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    if(!Visible) return 0; // Do not add us to render queue
                    return Priority;

                case Core.Signals.pre_render:
                    if(Host.Enabled)
                    {
                        HandlePreRender((double)args[0]);
                    }
                    return 1;

                case Core.Signals.render:
                    if(Host.Enabled)
                    {
                        HandleRender((double)args[0]);
                    }
                    else
                    {
                        HandleRenderDisabled((double)args[0]);
                    }
                    return 1;
                    
                case Core.Signals.hud_render:
                    if(Host.Enabled)
                    {
                        HandleHudRender((double)args[0]);
                    }
                    return 1;

            }
            return 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual void HandlePreRender(double delta_time)
        {
            
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual void HandleRender(double delta_time)
        {
            
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual void HandleRenderDisabled(double delta_time)
        {
            
        }
        
        /// <summary>
        /// Render function run after all others, meant for drawing the hud over the the render canvas.
        /// </summary>
        public virtual void HandleHudRender(double delta_time)
        {
            
        }
    }
}