using System;
using System.Runtime.InteropServices;

namespace FreshEngine.Managed;

/// <summary>
/// P/Invoke declarations for native Fresh Voxel Engine API
/// </summary>
internal static class NativeMethods
{
    private const string DllName = "FreshVoxelEngine.dll";

    // Engine lifecycle functions
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr Engine_Create();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Engine_Destroy(IntPtr engine);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Engine_Initialize(IntPtr engine);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Engine_Run(IntPtr engine);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Engine_Shutdown(IntPtr engine);

    // Window functions
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Engine_SetWindowTitle(IntPtr engine, [MarshalAs(UnmanagedType.LPStr)] string title);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Engine_SetWindowSize(IntPtr engine, int width, int height);
}

/// <summary>
/// Managed wrapper for the Fresh Voxel Engine
/// </summary>
public class Engine : IDisposable
{
    private IntPtr _nativeHandle;
    private bool _disposed;

    public Engine()
    {
        _nativeHandle = NativeMethods.Engine_Create();
        if (_nativeHandle == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native engine instance");
        }
    }

    public bool Initialize()
    {
        ThrowIfDisposed();
        return NativeMethods.Engine_Initialize(_nativeHandle);
    }

    public void Run()
    {
        ThrowIfDisposed();
        NativeMethods.Engine_Run(_nativeHandle);
    }

    public void Shutdown()
    {
        ThrowIfDisposed();
        NativeMethods.Engine_Shutdown(_nativeHandle);
    }

    public string WindowTitle
    {
        set
        {
            ThrowIfDisposed();
            NativeMethods.Engine_SetWindowTitle(_nativeHandle, value);
        }
    }

    public void SetWindowSize(int width, int height)
    {
        ThrowIfDisposed();
        NativeMethods.Engine_SetWindowSize(_nativeHandle, width, height);
    }

    private void ThrowIfDisposed()
    {
        if (_disposed)
        {
            throw new ObjectDisposedException(nameof(Engine));
        }
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (_nativeHandle != IntPtr.Zero)
            {
                NativeMethods.Engine_Destroy(_nativeHandle);
                _nativeHandle = IntPtr.Zero;
            }
            _disposed = true;
        }
    }

    ~Engine()
    {
        Dispose(false);
    }
}
