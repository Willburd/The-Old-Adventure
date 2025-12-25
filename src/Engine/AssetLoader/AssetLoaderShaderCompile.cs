using Silk.NET.OpenGL;

namespace Engine
{
    public static partial class AssetLoader
    {
        public class AssetShader : Asset
        {
            public AssetShader(uint new_shader) : base(new_shader)
            {
                // Shaders always stay once loaded, unlike other assets
                SetPersistent();
            }

            public new uint GetAsset 
            {
                get
                {
                    return (uint)data;
                }
            }
        }


#pragma warning disable CS8618 // Set during assetloader init
        private static GL Gl;
#pragma warning restore CS8618

        private static uint ShaderCompileVertex(string VertexShaderSource)
        {
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

        private static uint ShaderCompileFragment(string FragmentShaderSource)
        {
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

        private static uint ShaderCompileLink(uint vertexShader, uint fragmentShader)
        {
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

        private static uint ShaderBuild(string VertexShaderSource, string FragmentShaderSource)
        {
            uint vertexShader = ShaderCompileVertex(VertexShaderSource);
            uint fragmentShader = ShaderCompileFragment(FragmentShaderSource);
            return ShaderCompileLink(vertexShader, fragmentShader);
        }



        public static void ShaderAssetCreate(string asset_key, string VertexShaderSource, string FragmentShaderSource)
        {
            uint shader = ShaderBuild(VertexShaderSource, FragmentShaderSource);
            AddAsset(asset_key, new AssetShader(shader));
        }

        public static uint ShaderAssetGet(string asset_key)
        {
            return (uint)GetAsset(asset_key).GetAsset;
        }
    }
}