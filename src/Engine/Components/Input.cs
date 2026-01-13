using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that handles input from engine.
    /// </summary>
    public class Input(Entity host_entity) : EntComponent(host_entity)
    {
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.global_key_pressed, Core.Signals.global_key_released];
        }
        
        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)            
            {
                // Forward global signal of key being pressed, to all components on our entity that are listening to inputs.
                case Core.Signals.global_key_pressed:
                    return Host.SendSignal(Core.Signals.key_pressed, args[0]);
                case Core.Signals.global_key_released:
                    return Host.SendSignal(Core.Signals.key_released, args[0]);
            }
            return base.ReceiveSignal(signal,args);
        }

        public Vector3 MoveInput
        {
            get
            {
                Vector3 dir = Vector3.Zero;
                if(!ActiveUpdate) return dir;
                Vector2 pad_move = InputHandler.GamepadMove;
                if(pad_move.Length() > 0)
                {
                    return new Vector3(pad_move.X,0f,pad_move.Y);
                }
                if(InputHandler.KeyHeld( InputHandler.input_key_forward )) dir += Tools.Forward;
                if(InputHandler.KeyHeld( InputHandler.input_key_backward )) dir += Tools.Backward;
                if(InputHandler.KeyHeld( InputHandler.input_key_left )) dir += Tools.Left;
                if(InputHandler.KeyHeld( InputHandler.input_key_Right )) dir += Tools.Right;
                if(InputHandler.KeyHeld( InputHandler.input_key_editor_up )) dir += Tools.Up;
                if(InputHandler.KeyHeld( InputHandler.input_key_editor_down )) dir += Tools.Down;
                return dir;
            }
        }

        public Vector2 CameraInput
        {
            get
            {
                if(!ActiveUpdate) return Vector2.Zero;
                Vector2 pad_move = InputHandler.GamepadCamera;
                if(pad_move.Length() > 0)
                {
                    return pad_move;
                }
                return -new Vector2(InputHandler.MouseDelta.X * InputHandler.camera_sign_x, InputHandler.MouseDelta.Y * InputHandler.camera_sign_y);
            }
        }

        public float CameraRoll
        {
            get
            {
                float roll = 0f;
                if(!ActiveUpdate) return roll;
                if(InputHandler.KeyHeld( InputHandler.input_key_editor_rotate_cw )) roll += 1f;
                if(InputHandler.KeyHeld( InputHandler.input_key_editor_rotate_ccw )) roll -= 1f;
                return roll;
            }
        }
    }
}