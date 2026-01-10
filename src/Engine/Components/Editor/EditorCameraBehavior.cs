using System.Numerics;
using Engine;

namespace EntComponents
{
    public class EditorCameraBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.create, Core.Signals.editor_update];
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
            if(InputHandler.KeyPressed( InputHandler.input_key_exit ))
            {
                Core.RequestShutdown();
                return 1;
            }
            
            float camera_speed = 0.1f;
            if(InputHandler.KeyHeld( InputHandler.input_key_forward ))
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Forward * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_backward ))
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Backward * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_left ))
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Left * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_Right ))
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Right * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_editor_up ))
            {
                Camera.WorldCamera?.Position += Tools.Up * camera_speed;
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_editor_down ))
            {
                Camera.WorldCamera?.Position += Tools.Down * camera_speed;
            }
            
            float rotate_speed = 0.02f;
            if(InputHandler.KeyHeld( InputHandler.input_key_editor_rotate_cw ))
            {
                Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Forward, rotate_speed);
            }
            if(InputHandler.KeyHeld( InputHandler.input_key_editor_rotate_ccw ))
            {
                Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Forward, -rotate_speed);
            }

            // Mouse movement
            float mouse_multiplier = 0.001f;
            Vector2 mouse_delta = InputHandler.MouseDelta * mouse_multiplier;
            Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Up, -mouse_delta.X);
            Camera.WorldCamera?.Rotation *= Quaternion.CreateFromAxisAngle( Tools.Right, -mouse_delta.Y);

            // Raycast testing
            Collider? col = (Collider?)Host.GetComponent(typeof(Collider));
            Collider.RaycastHit? hit = Collider.DoRaycastNearest( Host.Position, Vector3.Transform(Tools.Forward * 5f,Host.Rotation));
            if(hit != null)
            {
                col?.OffsetPos = hit.Value.HitPosition;
            }
            return 1;
        }

        protected override uint HandleCollisions(List<Collider.Collision> collisions)
        {
            //Console.WriteLine(collisions.Count);
            
            return 1;
        }
    }
}