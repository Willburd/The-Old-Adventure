using Silk.NET.OpenGL;

namespace Engine
{
    /// <summary>
    /// Shader asset, stores a compiled shader's index in the GL context. Either loaded from a file or directly compiled.
    /// </summary>
    public class AssetShader : Asset
    {
        public AssetShader(string asset_key, string file_path, string VertexShaderSource, string FragmentShaderSource) : base(asset_key, file_path)
        {
            // Compile source strings
            uint vertexShader = ShaderCompileVertex(VertexShaderSource);
            uint fragmentShader = ShaderCompileFragment(FragmentShaderSource);

            // Get the shader program as our asset
            data = ShaderCompileLink(vertexShader, fragmentShader);
        }

        public override void Unload()
        {
            GL Gl = Core.OpenGLContext;

            // Clean out the compiled shader program
            Gl.DeleteProgram((uint)data);
            base.Unload();
        }
        
        public override bool CheckIntegrity()
        {
            GL Gl = Core.OpenGLContext;
            return Gl.IsProgram((uint)data);
        }

        /// <summary>
        /// Compiles a sourcecode string of GLSL into a a vertex shader.
        /// </summary>
        private static uint ShaderCompileVertex(string VertexShaderSource)
        {
            GL Gl = Core.OpenGLContext;
            uint vertexShader = Gl.CreateShader(ShaderType.VertexShader);
            Gl.ShaderSource(vertexShader, VertexShaderSource);
            Gl.CompileShader(vertexShader);

            //Checking the shader for compilation errors.
            string infoLog = Gl.GetShaderInfoLog(vertexShader);
            if (!string.IsNullOrWhiteSpace(infoLog))
            {
                Console.WriteLine($"Error compiling vertex shader {infoLog}");
            }

            return vertexShader;
        }

        /// <summary>
        /// Compiles a sourcecode string of GLSL into a a fragment shader.
        /// </summary>
        private static uint ShaderCompileFragment(string FragmentShaderSource)
        {
            GL Gl = Core.OpenGLContext;
            uint fragmentShader = Gl.CreateShader(ShaderType.FragmentShader);
            Gl.ShaderSource(fragmentShader, FragmentShaderSource);
            Gl.CompileShader(fragmentShader);

            //Checking the shader for compilation errors.
            string infoLog = Gl.GetShaderInfoLog(fragmentShader);
            if (!string.IsNullOrWhiteSpace(infoLog))
            {
                Console.WriteLine($"Error compiling fragment shader {infoLog}");
            }

            return fragmentShader;
        }

        /// <summary>
        /// Compiles a vertex and fragment shader into a shader program, this is the actual asset stored by an AssetShader. The component vertex and fragment are deleted and cleaned up after compilation is finished, as the program does not need them to function once created.
        /// </summary>
        private static uint ShaderCompileLink(uint vertexShader, uint fragmentShader)
        {
            GL Gl = Core.OpenGLContext;

            //Combining the shaders under one shader program.
            uint current_shader = Gl.CreateProgram();
            Gl.AttachShader(current_shader, vertexShader);
            Gl.AttachShader(current_shader, fragmentShader);
            Gl.LinkProgram(current_shader);

            //Checking the linking for errors.
            Gl.GetProgram(current_shader, GLEnum.LinkStatus, out var status);
            if (status == 0)
            {
                Console.WriteLine($"Error linking shader {Gl.GetProgramInfoLog(current_shader)}");
            }

            //Delete the no longer useful individual shaders;
            Gl.DetachShader(current_shader, vertexShader);
            Gl.DetachShader(current_shader, fragmentShader);
            Gl.DeleteShader(vertexShader);
            Gl.DeleteShader(fragmentShader);

            return current_shader;
        }
    }
}