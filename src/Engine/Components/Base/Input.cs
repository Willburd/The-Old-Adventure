using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that handles input from engine.
    /// </summary>
    public class Input(Entity host_entity) : EntComponent(host_entity)
    {
        public Vector3 MoveInput
        {
            get
            {
                Vector3 dir = Vector3.Zero;
                if(!ActiveUpdate) return dir;
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
                return -InputHandler.MouseDelta;
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