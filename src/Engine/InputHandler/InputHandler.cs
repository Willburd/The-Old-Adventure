
using Silk.NET.Input;
using System.Numerics;

namespace Engine
{
    public partial class InputHandler
    {
        private static Dictionary<Key, bool> previous_input_state = [];
        private static Dictionary<Key, bool> input_state = [];
        private static Dictionary<ButtonName, bool> previous_button_state = [];
        private static Dictionary<ButtonName, bool> button_state = [];

        // Input statics, todo - Move to a config struct
        public static Key KeyIDForward { get; private set; } = Key.Up;
        public static Key KeyIDBackward { get; private set; } = Key.Down;
        public static Key KeyIDLeft { get; private set; } = Key.Left;
        public static Key KeyIDRight { get; private set; } = Key.Right;
        public static Key KeyIDEditorUp { get; private set; } = Key.PageUp;
        public static Key KeyIDEditorDown { get; private set; } = Key.PageDown;
        public static Key KeyIDEditorRotateCW { get; private set; } = Key.End;
        public static Key KeyIDEditorRotateCCW { get; private set; } = Key.Delete;

        public static Key KeyIDExit { get; private set; } = Key.Escape;
        public static Key KeyIDConfirm { get; private set; } = Key.Z;
        public static Key KeyIDCancel { get; private set; } = Key.X;
        public static Key KeyIDMenu { get; private set; } = Key.Enter;

        public static ButtonName ButtonIDConfirm { get; private set; } = ButtonName.A;
        public static ButtonName ButtonIDCancel { get; private set; } = ButtonName.B;
        public static ButtonName ButtonIDMenu { get; private set; } = ButtonName.Start;

        public static int CameraSignX { get; private set; } = -1;
        public static int CameraSignY { get; private set; } = -1;
        public static float MouseCameraSensitivity { get; private set; } = 0.001f;
        public static float GamepadCameraSensitivity { get; private set; } = 0.1f;

        private static Vector2 OldMousePos { get; set; } = Vector2.Zero;
        public static Vector2 MousePos { get; private set; }
        public static Vector2 MouseDelta { get; private set; }
        public static Vector2 GamepadMove { get; private set; }
        public static Vector2 GamepadCamera { get; private set; }

        /// /// <summary>
        /// Used to check if a specific key was pressed this frame.
        /// </summary>
        public static bool KeyPressed(Key key)
        {
            if (!input_state.TryGetValue(key, out bool value))
            {
                value = false;
                input_state.Add(key, value);
                previous_input_state.Add(key, false);
                return false;
            }
            return value && !previous_input_state[key];
        }

        /// <summary>
        /// Used to check if a specific key was released this frame.
        /// </summary>
        public static bool KeyReleased(Key key)
        {
            if (!input_state.TryGetValue(key, out bool value))
            {
                value = false;
                input_state.Add(key, value);
                previous_input_state.Add(key, false);
                return false;
            }
            return input_state[key] != value && previous_input_state[key];
        }

        /// <summary>
        /// Used to check if a specific button was pressed this frame.
        /// </summary>
        public static bool ButtonPressed(ButtonName button)
        {
            if (!button_state.TryGetValue(button, out bool value))
            {
                value = false;
                button_state.Add(button, value);
                previous_button_state.Add(button, false);
            }
            return value && !previous_button_state[button];
        }

        /// <summary>
        /// Used to check if a specific key is held
        /// </summary>
        public static bool KeyHeld(Key key)
        {
            if (!input_state.TryGetValue(key, out bool value)) return false;
            return value;
        }

        /// <summary>
        /// Used to check if a specific button was released this frame.
        /// </summary>
        public static bool ButtonReleased(ButtonName button)
        {
            if (!button_state.TryGetValue(button, out bool value))
            {
                value = false;
                button_state.Add(button, value);
                previous_button_state.Add(button, false);
            }
            return button_state[button] != value && previous_button_state[button];
        }

        /// <summary>
        /// Used to check if a specific button is held
        /// </summary>
        public static bool ButtonHeld(ButtonName button)
        {
            if (!button_state.TryGetValue(button, out bool value)) return false;
            return value;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // The following is for engine use only.
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        public static void InvokeKeyPressed(IKeyboard keyboard, Key key, int keyCode)
        {
            // Exit game
            if (key == KeyIDExit) // TODO - Move this to a real menu handler
            {
                Core.RequestShutdown();
                return;
            }
            // A new hand touches the beacon
            if (input_state.TryAdd(key, true))
            {
                previous_input_state.Add(key, false);
                return;
            }
            // Update previous state
            input_state[key] = true;
            Entity.SendGlobalSignal(Core.Signals.global_input_pressed, key, null);
        }

        public static void InvokeKeyReleased(IKeyboard keyboard, Key key, int keyCode)
        {
            if (input_state.TryAdd(key, false))
            {
                previous_input_state.Add(key, true);
                return;
            }
            input_state[key] = false;
            Entity.SendGlobalSignal(Core.Signals.global_input_released, key, null);
        }

        public static void InvokeMouseWheel(IMouse mouse, ScrollWheel scrollWheel)
        {

        }

        public static void InvokeInputConnection(IInputDevice device, bool was_connected)
        {
            string connection_state = was_connected ? "connected" : "disconnected";
            if (device is IGamepad)
            {
                InvokeGamepadConnection(was_connected, connection_state, (IGamepad)device);
            }
        }

        public static void InvokeGamepadConnection(bool was_connected, string state, IGamepad gamepad)
        {
            if (was_connected)
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
            if (button_state.TryAdd(button.Name, true))
            {
                previous_button_state.Add(button.Name, false);
                return;
            }
            // Update previous state
            button_state[button.Name] = true;
            Entity.SendGlobalSignal(Core.Signals.global_input_pressed, null, button.Name);
        }

        public static void InvokeButtonReleased(IGamepad gamepad, Button button)
        {
            if (button_state.TryAdd(button.Name, false))
            {
                previous_button_state.Add(button.Name, true);
                return;
            }
            button_state[button.Name] = false;
            Entity.SendGlobalSignal(Core.Signals.global_input_released, null, button.Name);
        }

        public static void InvokeThumbstickMoved(IGamepad gamepad, Thumbstick stick)
        {
            if (stick.Index == 0)
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
            foreach ((Key key, bool state) in input_state)
            {
                previous_input_state[key] = state;
            }
            foreach ((ButtonName button, bool state) in button_state)
            {
                previous_button_state[button] = state;
            }
            // Mouse too!
            MouseUpdate();
        }

        /// <summary>
        /// Updates the mouse position each frame based on the current raw mouse, but has it's own delta and old position vars to compensate.
        /// </summary>
        private void MouseUpdate()
        {
            MousePos = Core.RawMousePos;
            MouseDelta = MousePos - OldMousePos;
            OldMousePos = MousePos;
        }
    }
}