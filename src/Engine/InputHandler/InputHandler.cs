
using Silk.NET.Input;
using System.Numerics;

namespace Engine
{
    public class InputHandler
    {
        public static Dictionary<Key,bool> previous_input_state = [];
        public static Dictionary<Key,bool> input_state = [];
        public static Dictionary<Button,bool> previous_button_state = [];
        public static Dictionary<Button,bool> button_state = [];

        // Input statics, todo - Move to a config struct
        public static Key input_key_forward = Key.Up;
        public static Key input_key_backward = Key.Down;
        public static Key input_key_left = Key.Left;
        public static Key input_key_Right = Key.Right;
        public static Key input_key_editor_up = Key.PageUp;
        public static Key input_key_editor_down = Key.PageDown;
        public static Key input_key_editor_rotate_cw = Key.End;
        public static Key input_key_editor_rotate_ccw = Key.Delete;
        public static Key input_key_exit = Key.Escape;

        public static Key input_key_confirm = Key.Z;
        public static Key input_key_cancel = Key.X;
        public static Key input_key_menu = Key.Enter;

        public static int camera_sign_x = -1;
        public static int camera_sign_y = -1;
        public static float mouse_sensitivity = 0.001f;

        // Public interface

        /// <summary>
        /// Used to check if a specific key was pressed this frame.
        /// </summary>
        public static bool KeyPressed(Key key)
        {
            if(!input_state.TryGetValue(key, out bool value)) 
            {
                value = false;
                input_state.Add(key, value);
                previous_input_state.Add(key,false);
                return false;
            }
            return value && !previous_input_state[key];
        }
        
        /// <summary>
        /// Used to check if a specific key was released this frame.
        /// </summary>
        public static bool KeyReleased(Key key)
        {
            if(!input_state.TryGetValue(key, out bool value)) 
            {
                value = false;
                input_state.Add(key, value);
                previous_input_state.Add(key,false);
                return false;
            }
            return input_state[key] != value && previous_input_state[key];
        }
        
        /// <summary>
        /// Used to check if a specific button was pressed this frame.
        /// </summary>
        public static bool ButtonPressed(Button button)
        {
            if(!button_state.TryGetValue(button, out bool value)) 
            {
                value = false;
                button_state.Add(button, value);
                previous_button_state.Add(button,false);
            }
            return value && !previous_button_state[button];
        }
        
        /// <summary>
        /// Used to check if a specific key is held
        /// </summary>
        public static bool KeyHeld(Key key)
        {
            if(!input_state.TryGetValue(key, out bool value)) return false;
            return value;
        }

        /// <summary>
        /// Used to check if a specific button was released this frame.
        /// </summary>
        public static bool ButtonReleased(Button button)
        {
            if(!button_state.TryGetValue(button, out bool value)) 
            {
                value = false;
                button_state.Add(button, value);
                previous_button_state.Add(button,false);
            }
            return button_state[button] != value && previous_button_state[button];
        }

        /// <summary>
        /// Used to check if a specific button is held
        /// </summary>
        public static bool ButtonHeld(Button button)
        {
            if(!button_state.TryGetValue(button, out bool value)) return false;
            return value;
        }
        


        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // The following is for engine use only.
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        public static void InvokeKeyPressed(IKeyboard keyboard, Key key, int keyCode)
        {
            // Exit game
            if(key == input_key_exit)
            {
                Core.RequestShutdown();
                return;
            }
            // A new hand touches the beacon
            if(input_state.TryAdd(key, true))
            {
                previous_input_state.Add(key,false);
                return;
            }
            // Update previous state
            input_state[key] = true;
            Entity.SendGlobalSignal(Core.Signals.global_key_pressed, key);
        }

        public static void InvokeKeyReleased(IKeyboard keyboard, Key key, int keyCode)
        {
            if(input_state.TryAdd(key, false))
            {
                previous_input_state.Add(key,true);
                return;
            }
            input_state[key] = false;
            Entity.SendGlobalSignal(Core.Signals.global_key_released, key);
        }

        public static void InvokeMouseWheel(IMouse mouse, ScrollWheel scrollWheel)
        {
            
        }

        public static void InvokeInputConnection(IInputDevice device, bool was_connected)
        {
            string connection_state = was_connected ? "connected" : "disconnected";
            if(device is IGamepad)
            {
                InvokeGamepadConnection(was_connected, connection_state, (IGamepad)device);
            }
        }

        public static void InvokeGamepadConnection(bool was_connected, string state, IGamepad gamepad)
        {
            if(was_connected)
            {
                gamepad.Deadzone = new(0.2f, DeadzoneMethod.AdaptiveGradient);
                gamepad.ButtonDown += InvokeButtonPressed;
                gamepad.ButtonUp += InvokeButtonReleased;
                gamepad.ThumbstickMoved += InvokeThumbstickMoved;
                gamepad.TriggerMoved += InvokeTriggerMoved;
            }
            else
            {
                gamepad.ButtonDown -= InvokeButtonPressed;
                gamepad.ButtonUp -= InvokeButtonReleased;
                gamepad.ThumbstickMoved -= InvokeThumbstickMoved;
                gamepad.TriggerMoved -= InvokeTriggerMoved;
            }
        }

        public static void InvokeButtonPressed(IGamepad gamepad, Button button)
        {
            // A new hand touches the beacon
            if(button_state.TryAdd(button, true))
            {
                previous_button_state.Add(button,false);
                return;
            }
            // Update previous state
            button_state[button] = true;
            Entity.SendGlobalSignal(Core.Signals.global_key_pressed, button);
        }

        public static void InvokeButtonReleased(IGamepad gamepad, Button button)
        {
            if(button_state.TryAdd(button, false))
            {
                previous_button_state.Add(button,true);
                return;
            }
            button_state[button] = false;
            Entity.SendGlobalSignal(Core.Signals.global_key_released, button);
        }

        public static Vector2 GamepadMove {get; private set;}
        public static Vector2 GamepadCamera {get; private set;}
        public static void InvokeThumbstickMoved(IGamepad gamepad, Thumbstick stick)
        {
            if(stick.Index == 0)
            {
                // left stick
                GamepadMove = new Vector2(stick.X, stick.Y);
            }
            else
            {
                // right stick
                GamepadCamera = new Vector2(stick.X, stick.Y);
            }
        }

        public static void InvokeTriggerMoved(IGamepad gamepad, Trigger trigger)
        {
            
        }


        /// <summary>
        /// Updates the previously held key state. Do not use externally.
        /// </summary>
        public void InputStateUpdate()
        {
            // Updates the previous state at the end of the frame
            foreach((Key key, bool state) in input_state)
            {
                previous_input_state[key] = state;
            }
            foreach((Button button, bool state) in button_state)
            {
                previous_button_state[button] = state;
            }
            // Mouse too!
            MouseUpdate();
        }

        public static Vector2 MousePos {get; private set;}
        public static Vector2 MouseDelta {get; private set;}
        private static Vector2 old_mouse_pos = Vector2.Zero;
        
        /// <summary>
        /// Updates the mouse position each frame based on the current raw mouse, but has it's own delta and old position vars to compensate.
        /// </summary>
        private void MouseUpdate()
        {
            MousePos = Core.RawMousePos;
            MouseDelta = MousePos - old_mouse_pos;
            old_mouse_pos = MousePos;
        }
    }
}