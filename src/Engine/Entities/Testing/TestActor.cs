using System.Security.Cryptography.X509Certificates;

namespace Engine
{
    public class TestActor : Actor
    {
        public TestActor() : base()
        {
            new EntComponents.TestRender(this);
        }

        public override void OnAssetLoad()
        {
            AssetLoader.ShaderAssetCreate(
                "shader_test",
                @"
                #version 330 core //Using version GLSL version 3.3
                layout (location = 0) in vec4 vPos;
                
                void main()
                {
                    gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);
                }
                ",
                @"
                #version 330 core
                out vec4 FragColor;

                void main()
                {
                    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                }
                ");
        }
    }
}