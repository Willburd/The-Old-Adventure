using System.Numerics;
using Engine;

namespace EntComponents
{
    public class EditorCameraBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.update];
        }

        protected override uint HandleUpdate()
        {
            if(InputHandler.KeyPressed( InputHandler.input_key_exit ))
            {
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

            return 1;
        }
    }
}