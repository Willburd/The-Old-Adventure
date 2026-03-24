namespace GameScenes
{
    public partial class Test : SceneController
    {
        protected override void SceneStarted()
        {
            // Example, don't call parent.
            LoadEntityLayer("Default");
        }
    }
}