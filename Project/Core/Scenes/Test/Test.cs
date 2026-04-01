namespace GameScenes
{
    public partial class Test : SceneController
    {
        protected override void SceneStarted(string entrance_id)
        {
            LoadEntityLayer("Default");
        }
    }
}