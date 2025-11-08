using System;
using System.Runtime.InteropServices;

namespace FreshEngine.Managed.Input;

/// <summary>
/// Key codes matching GLFW key definitions
/// </summary>
public enum KeyCode
{
    // Printable keys
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    
    Key0 = 48,
    Key1 = 49,
    Key2 = 50,
    Key3 = 51,
    Key4 = 52,
    Key5 = 53,
    Key6 = 54,
    Key7 = 55,
    Key8 = 56,
    Key9 = 57,
    
    Semicolon = 59,
    Equal = 61,
    
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    
    // Function keys
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    
    // Keypad
    Keypad0 = 320,
    Keypad1 = 321,
    Keypad2 = 322,
    Keypad3 = 323,
    Keypad4 = 324,
    Keypad5 = 325,
    Keypad6 = 326,
    Keypad7 = 327,
    Keypad8 = 328,
    Keypad9 = 329,
    KeypadDecimal = 330,
    KeypadDivide = 331,
    KeypadMultiply = 332,
    KeypadSubtract = 333,
    KeypadAdd = 334,
    KeypadEnter = 335,
    KeypadEqual = 336,
    
    // Modifiers
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
}

/// <summary>
/// Mouse button codes
/// </summary>
public enum MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7
}

/// <summary>
/// Input actions that can be mapped to keys
/// </summary>
public enum InputAction
{
    MoveForward = 0,
    MoveBackward = 1,
    MoveLeft = 2,
    MoveRight = 3,
    Jump = 4,
    Crouch = 5,
    Sprint = 6,
    Use = 7,
    Attack = 8,
    PlaceBlock = 9,
    BreakBlock = 10,
    OpenInventory = 11,
    OpenMenu = 12,
    ToggleEditor = 13,
    OpenChat = 14
}

/// <summary>
/// Input modes
/// </summary>
public enum InputMode
{
    GameMode = 0,
    UIMode = 1,
    BuildMode = 2
}

/// <summary>
/// P/Invoke declarations for native InputManager API
/// </summary>
internal static class InputNativeMethods
{
    private const string DllName = "FreshVoxelEngine.dll";

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr InputManager_GetInstance();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsKeyPressed(IntPtr handle, int keyCode);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsKeyJustPressed(IntPtr handle, int keyCode);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsMouseButtonPressed(IntPtr handle, int button);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsMouseButtonJustPressed(IntPtr handle, int button);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float InputManager_GetMouseX(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float InputManager_GetMouseY(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float InputManager_GetMouseDeltaX(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float InputManager_GetMouseDeltaY(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsActionActive(IntPtr handle, int action);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_IsActionJustPressed(IntPtr handle, int action);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void InputManager_SetKeyBinding(IntPtr handle, int action, int keyCode);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern float InputManager_GetMouseSensitivity(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void InputManager_SetMouseSensitivity(IntPtr handle, float sensitivity);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int InputManager_GetInputMode(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void InputManager_SetInputMode(IntPtr handle, int mode, int temporary);
}

/// <summary>
/// Managed wrapper for the InputManager providing keyboard, mouse, and action-based input
/// </summary>
public class InputManager
{
    private readonly IntPtr _nativeHandle;

    /// <summary>
    /// Initializes a new instance of the InputManager class
    /// </summary>
    public InputManager()
    {
        _nativeHandle = InputNativeMethods.InputManager_GetInstance();
        if (_nativeHandle == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to get native InputManager instance");
        }
    }

    /// <summary>
    /// Check if a key is currently pressed
    /// </summary>
    /// <param name="key">Key code to check</param>
    /// <returns>True if key is pressed</returns>
    public bool IsKeyPressed(KeyCode key)
    {
        return InputNativeMethods.InputManager_IsKeyPressed(_nativeHandle, (int)key) != 0;
    }

    /// <summary>
    /// Check if a key was just pressed this frame
    /// </summary>
    /// <param name="key">Key code to check</param>
    /// <returns>True if key was just pressed</returns>
    public bool IsKeyJustPressed(KeyCode key)
    {
        return InputNativeMethods.InputManager_IsKeyJustPressed(_nativeHandle, (int)key) != 0;
    }

    /// <summary>
    /// Check if a mouse button is currently pressed
    /// </summary>
    /// <param name="button">Mouse button to check</param>
    /// <returns>True if button is pressed</returns>
    public bool IsMouseButtonPressed(MouseButton button)
    {
        return InputNativeMethods.InputManager_IsMouseButtonPressed(_nativeHandle, (int)button) != 0;
    }

    /// <summary>
    /// Check if a mouse button was just pressed this frame
    /// </summary>
    /// <param name="button">Mouse button to check</param>
    /// <returns>True if button was just pressed</returns>
    public bool IsMouseButtonJustPressed(MouseButton button)
    {
        return InputNativeMethods.InputManager_IsMouseButtonJustPressed(_nativeHandle, (int)button) != 0;
    }

    /// <summary>
    /// Get the current mouse X position
    /// </summary>
    public float MouseX => InputNativeMethods.InputManager_GetMouseX(_nativeHandle);

    /// <summary>
    /// Get the current mouse Y position
    /// </summary>
    public float MouseY => InputNativeMethods.InputManager_GetMouseY(_nativeHandle);

    /// <summary>
    /// Get the mouse movement delta X (since last frame)
    /// </summary>
    public float MouseDeltaX => InputNativeMethods.InputManager_GetMouseDeltaX(_nativeHandle);

    /// <summary>
    /// Get the mouse movement delta Y (since last frame)
    /// </summary>
    public float MouseDeltaY => InputNativeMethods.InputManager_GetMouseDeltaY(_nativeHandle);

    /// <summary>
    /// Check if an action is currently active
    /// </summary>
    /// <param name="action">Action to check</param>
    /// <returns>True if action is active</returns>
    public bool IsActionActive(InputAction action)
    {
        return InputNativeMethods.InputManager_IsActionActive(_nativeHandle, (int)action) != 0;
    }

    /// <summary>
    /// Check if an action was just triggered this frame
    /// </summary>
    /// <param name="action">Action to check</param>
    /// <returns>True if action was just triggered</returns>
    public bool IsActionJustPressed(InputAction action)
    {
        return InputNativeMethods.InputManager_IsActionJustPressed(_nativeHandle, (int)action) != 0;
    }

    /// <summary>
    /// Bind a key to an action
    /// </summary>
    /// <param name="action">Action to bind</param>
    /// <param name="key">Key to bind to the action</param>
    public void SetKeyBinding(InputAction action, KeyCode key)
    {
        InputNativeMethods.InputManager_SetKeyBinding(_nativeHandle, (int)action, (int)key);
    }

    /// <summary>
    /// Get or set the mouse sensitivity
    /// </summary>
    public float MouseSensitivity
    {
        get => InputNativeMethods.InputManager_GetMouseSensitivity(_nativeHandle);
        set => InputNativeMethods.InputManager_SetMouseSensitivity(_nativeHandle, value);
    }

    /// <summary>
    /// Get or set the current input mode
    /// </summary>
    public InputMode InputMode
    {
        get => (InputMode)InputNativeMethods.InputManager_GetInputMode(_nativeHandle);
        set => InputNativeMethods.InputManager_SetInputMode(_nativeHandle, (int)value, 0);
    }

    /// <summary>
    /// Set the input mode with temporary flag
    /// </summary>
    /// <param name="mode">Input mode to set</param>
    /// <param name="temporary">If true, mode is temporary</param>
    public void SetInputMode(InputMode mode, bool temporary)
    {
        InputNativeMethods.InputManager_SetInputMode(_nativeHandle, (int)mode, temporary ? 1 : 0);
    }
}
