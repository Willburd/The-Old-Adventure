namespace Engine
{
    public class Actor : Entity
    {
        public Room? OwnerRoom { get; private set; } = null;

        public Actor(Transform initial_location, string actor_id, string entity_asset_key, Room? room_link) : base(initial_location, actor_id, entity_asset_key)
        {
            if (room_link != null) LinkRoom(room_link);
        }


        /// <summary>
        /// Gets a loaded actor by its entity key. 
        /// </summary>
        public static Actor? GetActor(string entity_key, Room? specific_room = null)
        {
            if (specific_room != null)
            {
                if (!specific_room.ActorLookupList.TryGetValue(entity_key, out Actor? roomactor)) return null;
                return roomactor;
            }
            if (!EntityLookupList.TryGetValue(entity_key, out Entity? globalactor)) return null;
            return (Actor)globalactor;
        }

        public override bool RoomEnabled()
        {
            return Enabled && (OwnerRoom == null || OwnerRoom.Enabled);
        }

        private void LinkRoom(Room room)
        {
            OwnerRoom = room;
            OwnerRoom.ActorList.Add(this);
            OwnerRoom.ActorLookupList.Add(EntityID, this);
        }

        public void UnlinkRoom()
        {
            OwnerRoom?.ActorList.Remove(this);
            OwnerRoom?.ActorLookupList.Remove(EntityID);
            OwnerRoom = null;
        }

        protected override void OnCleanup()
        {
            UnlinkRoom();
        }
    }
}