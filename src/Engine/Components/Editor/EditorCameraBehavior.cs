using System.Numerics;
using Engine;
using Silk.NET.Input;

namespace EntComponents
{
    public class EditorCameraBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.create, Core.Signals.editor_update, Core.Signals.key_pressed, Core.Signals.key_released];
        }

        protected override uint HandleCreate()
        {
            
            Collider? col = (Collider?)Host.GetComponent(typeof(Collider));
            col?.SetShape(new Engine.ColliderShapes.PointCol());
            col?.SyncRelativePosition = false;
            col?.CollisionMask = 0; // Nope, using this for debugging

            return 1;
        }

        protected override uint HandleEditorUpdate()
        {
            // TODO - Move this exit game stuff to somewhere in the Core somehow.
            if(InputHandler.KeyPressed( InputHandler.input_key_exit ))
            {
                Core.RequestShutdown();
                return 1;
            }

            // Handle editor camera logic
            Input? input = (Input?)Host.GetComponent(typeof(Input));
            if(input != null)
            {
                // Movement
                float camera_speed = 0.1f;
                Camera.WorldCamera?.Position += Vector3.Transform(input.MoveInput * camera_speed, Camera.WorldCamera.Location.Rotation);
                float rotate_speed = 0.02f;
                Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Forward, input.CameraRoll * rotate_speed);

                // Mouse movement
                float mouse_multiplier = 0.001f;
                Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Vector3.Transform(Tools.Up, Quaternion.Inverse(Camera.WorldCamera.Rotation)), input.CameraInput.X * mouse_multiplier);
                Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Right, input.CameraInput.Y * mouse_multiplier);
            }
            return 1;
        }

        protected override uint HandleKeyPressed(Key key)
        {
            if(key == InputHandler.input_key_cancel)
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

        protected override uint HandleKeyReleased(Key key)
        {
            return 1;
        }

        protected override uint HandleCollisions(List<Collider.Collision> collisions)
        {
            return 1;
        }
    }
}