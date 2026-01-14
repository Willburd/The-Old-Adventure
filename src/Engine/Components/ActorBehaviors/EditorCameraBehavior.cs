using System.Numerics;
using Engine;
using Silk.NET.Input;

namespace EntComponents.ActorBehavior
{
    public class EditorCameraBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.create, Core.Signals.editor_update, Core.Signals.input_pressed, Core.Signals.input_released];
        }

        protected override uint HandleCreate()
        {
            
            Collider? col = (Collider?)Host.GetComponent(typeof(Collider));
            col?.SetShape(new Engine.ColliderShapes.PointCol());
            col?.SyncRelativePosition = false;
            col?.CollisionMask = Collider.mask_none; // Nope, using this for debugging

            return 1;
        }

        protected override uint HandleEditorUpdate()
        {
            // Handle editor camera logic
            Input? input = (Input?)Host.GetComponent(typeof(Input));
            if(input != null)
            {
                // Movement
                float camera_speed = 0.1f;
                Host.Position += Vector3.Transform(input.Move * camera_speed, Host.Location.Rotation);
                float rotate_speed = 0.02f;
                Host.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Forward, input.CameraRoll * rotate_speed);

                // Mouse movement
                Host.Rotation *= Quaternion.CreateFromAxisAngle( Vector3.Transform(Tools.Up, Quaternion.Inverse(Host.Rotation)), input.CameraMove.X);
                Host.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Right, input.CameraMove.Y);
            }
            return 1;
        }

        protected override uint HandlePressed(Key? key, ButtonName? button)
        {
            if(key == InputHandler.KeyIDConfirm || button == InputHandler.ButtonIDConfirm)
            {
                // Raycast testing
                Collider? col = (Collider?)Host.GetComponent(typeof(Collider));
                Collider.RaycastHit? hit = Collider.DoRaycastNearest( Host.Position, Vector3.Transform(Tools.Forward * 5f,Host.Rotation));
                if(hit != null) 
                {
                    col?.OffsetPos = hit.Value.HitPosition;
                    Console.WriteLine("COLLISION POSITION [" + hit.Value.HitPosition + "]");
                }
                return 1;
            }
            return 0;
        }

        protected override uint HandleReleased(Key? key, ButtonName? button)
        {
            return 1;
        }

        protected override uint HandleCollisions(List<Collider.Collision> collisions)
        {
            return 1;
        }
    }
}