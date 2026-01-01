
using Silk.NET.Input;
using System.Numerics;

namespace Engine
{
    public class InputHandler
    {
        public static Dictionary<Key,bool> previous_input_state = [];
        public static Dictionary<Key,bool> input_state = [];

        // Input statics, todo - Move to a config struct
        public static Key input_key_forward = Key.W;
        public static Key input_key_backward = Key.S;
        public static Key input_key_left = Key.A;
        public static Key input_key_Right = Key.D;
        public static Key input_key_editor_up = Key.R;
        public static Key input_key_editor_down = Key.F;
        public static Key input_key_exit = Key.W;

        // Public interface

        /// <summary>
        /// Used to check if a specific key was pressed this frame.
        /// </summary>
        public static bool KeyPressed(Key key)
        {
            if(!input_state.ContainsKey(key)) return false;
            return input_state[key] && !previous_input_state[key];
        }
        
        /// <summary>
        /// Used to check if a specific key was released this frame.
        /// </summary>
        public static bool KeyReleased(Key key)
        {
            if(!input_state.ContainsKey(key)) return false;
            return !input_state[key] && previous_input_state[key];
        }
        
        /// <summary>
        /// Used to check if a specific key is held
        /// </summary>
        public static bool KeyHeld(Key key)
        {
            if(!input_state.ContainsKey(key)) return false;
            return input_state[key];
        }

        // The following is for engine use only.

        /// <summary>
        /// Fired from the window itself in HandleKeyDown(). Do not use externally.
        /// </summary>
        public void InvokeKeyPressed(IKeyboard keyboard, Key key, int keyCode)
        {
            // A new hand touches the beacon
            if(!input_state.ContainsKey(key))
            {
                previous_input_state.Add(key,false);
                input_state.Add(key,true);
                return;
            }
            // Update previous state
            input_state[key] = true;
        }

        /// <summary>
        /// Fired from the window itself in HandleKeyUp(). Do not use externally.
        /// </summary>
        public void InvokeKeyReleased(IKeyboard keyboard, Key key, int keyCode)
        {
            if(!input_state.ContainsKey(key))
            {
                previous_input_state.Add(key,true);
                input_state.Add(key,false);
                return;
            }
            input_state[key] = false;
        }

        /// <summary>
        /// Updates the previously held key state. Do not use externally.
        /// </summary>
        public void HeldKeyUpdate()
        {
            // Updates the previous state at the end of the frame
            foreach((Key key, bool state) in input_state)
            {
                previous_input_state[key] = state;
            }
        }
    }
}