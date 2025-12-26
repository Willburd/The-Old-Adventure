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
            AssetLoader.ShaderAssetCreate("shader_test", Tools.AssetDirectory + "/Shaders/test");
        }
    }
}