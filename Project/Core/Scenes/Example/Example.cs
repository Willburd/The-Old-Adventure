namespace GameScenes
{
    public partial class Example : SceneController
    {
        protected override void SceneStarted(string entrance_id)
        {
            // Example, don't call parent.
            LoadEntityLayer("Default");
            // LoadEntityLayer("Day");
            // LoadEntityLayer("Night");

            // if(entrance_id) LoadEntityLayer("Cutscene");
        }
    }
}