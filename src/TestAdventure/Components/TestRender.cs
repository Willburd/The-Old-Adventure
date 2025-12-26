using Engine;
using Silk.NET.Assimp;
using Rendering;

namespace EntComponents
{
    public class TestRender(Entity host_entity) : Renders(host_entity)
    {

        public override uint HandleAssetLoad()
        {
            AssetLoader.ModelAssetLoad("model_test", Tools.AssetDirectory + "/Models/test.obj");
            ShaderData test_shader = AssetLoader.ShaderAssetLoad("shader_test", Tools.AssetDirectory + "/Shaders/test");
            TextureData test_tex = AssetLoader.TextureAssetLoad("texture_test", Tools.AssetDirectory + "/Textures/test.png");

            // Materials for each mesh in the model
            materials.Add(new MaterialData([new MaterialUniformData("uTexture0", (int)test_tex.Handle())], test_shader));
            materials.Add(new MaterialData([new MaterialUniformData("uTexture0", (int)test_tex.Handle())], test_shader));
            materials.Add(new MaterialData([new MaterialUniformData("uTexture0", (int)test_tex.Handle())], test_shader));

            // Load the model with the materials we assigned
            model = AssetLoader.ModelAssetGet("model_test");

            return 1;
        }
    }
}