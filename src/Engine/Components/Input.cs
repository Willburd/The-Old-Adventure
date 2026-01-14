using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that handles input from engine.
    /// </summary>
    public class Input(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.global_input_pressed, Core.Signals.global_input_released];
        }
        
        public override uint ReceiveSignal(Core.Signals signal, object?[] args)
        {
            switch(signal)            
            {
                // Forward global signal of key being pressed, to all components on our entity that are listening to inputs.
                case Core.Signals.global_input_pressed:
                    return Host.SendSignal(Core.Signals.input_pressed, args[0], args[1]);
                case Core.Signals.global_input_released:
                    return Host.SendSignal(Core.Signals.input_released, args[0], args[1]);
            }
            return base.ReceiveSignal(signal,args);
        }

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Game interaction
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        public enum CheckState
        {
            pressed,
            released,
            held
        }

        public Vector3 Move
        {
            get
            {
                if(!ActiveUpdate) return Vector3.Zero;
                Vector3 dir = Vector3.Zero;
                Vector2 pad_move = InputHandler.GamepadMove;
                if(pad_move.Length() > 0) return new Vector3(pad_move.X,0f,pad_move.Y);
                if(InputHandler.KeyHeld( InputHandler.KeyIDForward ))   dir += Tools.Forward;
                if(InputHandler.KeyHeld( InputHandler.KeyIDBackward ))  dir += Tools.Backward;
                if(InputHandler.KeyHeld( InputHandler.KeyIDLeft ))      dir += Tools.Left;
                if(InputHandler.KeyHeld( InputHandler.KeyIDRight ))     dir += Tools.Right;
                if(InputHandler.KeyHeld( InputHandler.KeyIDEditorUp ))  dir += Tools.Up;
                if(InputHandler.KeyHeld( InputHandler.KeyIDEditorDown ))dir += Tools.Down;
                return dir;
            }
        }

        public Vector2 CameraMove
        {
            get
            {
                if(!ActiveUpdate) return Vector2.Zero;
                Vector2 pad_move = InputHandler.GamepadCamera;
                if(pad_move.Length() > 0) return pad_move;
                return -new Vector2(InputHandler.MouseDelta.X * InputHandler.CameraSignX * InputHandler.MouseCameraSensitivity, InputHandler.MouseDelta.Y * InputHandler.CameraSignY * InputHandler.MouseCameraSensitivity);
            }
        }

        public float CameraRoll
        {
            get
            {
                if(!ActiveUpdate) return 0f;
                float roll = 0f;
                if(InputHandler.KeyHeld( InputHandler.KeyIDEditorRotateCW )) roll += 1f;
                if(InputHandler.KeyHeld( InputHandler.KeyIDEditorRotateCCW )) roll -= 1f;
                return roll;
            }
        }

        public bool Confirm(CheckState state)
        {
            if(!ActiveUpdate) return false;
            if(state == CheckState.pressed) return InputHandler.KeyPressed(InputHandler.KeyIDConfirm) || InputHandler.ButtonPressed(InputHandler.ButtonIDConfirm);
            if(state == CheckState.released) return InputHandler.KeyReleased(InputHandler.KeyIDConfirm) || InputHandler.ButtonReleased(InputHandler.ButtonIDConfirm);
            if(state == CheckState.held) return InputHandler.KeyHeld(InputHandler.KeyIDConfirm) || InputHandler.ButtonHeld(InputHandler.ButtonIDConfirm);
            return false;
        }

        public bool Cancel(CheckState state)
        {
            if(!ActiveUpdate) return false;
            if(state == CheckState.pressed) return InputHandler.KeyPressed(InputHandler.KeyIDCancel) || InputHandler.ButtonPressed(InputHandler.ButtonIDCancel);
            if(state == CheckState.released) return InputHandler.KeyReleased(InputHandler.KeyIDCancel) || InputHandler.ButtonReleased(InputHandler.ButtonIDCancel);
            if(state == CheckState.held) return InputHandler.KeyHeld(InputHandler.KeyIDCancel) || InputHandler.ButtonHeld(InputHandler.ButtonIDCancel);
            return false;
        }
        
        public bool Exit(CheckState state)
        {
            if(!ActiveUpdate) return false;
            if(state == CheckState.pressed) return InputHandler.KeyPressed(InputHandler.KeyIDExit);
            if(state == CheckState.released) return InputHandler.KeyReleased(InputHandler.KeyIDExit);
            if(state == CheckState.held) return InputHandler.KeyHeld(InputHandler.KeyIDExit);
            return false;
        }
    }
}