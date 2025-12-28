namespace Engine
{
    public class Actor : Entity
    {
        public Room? OwnerRoom {get; private set;} = null;

        public Actor(Transform initial_location, Room? room_link) : base(initial_location)
        {
            if(room_link != null) LinkRoom(room_link);
        }

        private void LinkRoom(Room room)
        {
            OwnerRoom = room;
            OwnerRoom.ActorList.Add(this);
        }
        
        public void UnlinkRoom()
        {
            OwnerRoom?.ActorList.Remove(this);
            OwnerRoom = null;
        }

        protected override void OnCleanup()
        {
            UnlinkRoom();
        }
    }
}