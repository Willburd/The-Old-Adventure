namespace GameScenes
{
    public partial class Example : SceneController
    {
        protected override void SceneStarted()
        {
            // Example, don't call parent.
            LoadEntityLayer("Default");
            ClearLayerTemplates();
        }
    }
}