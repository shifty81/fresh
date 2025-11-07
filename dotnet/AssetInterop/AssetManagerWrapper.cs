using System;
using System.IO;
using System.Runtime.InteropServices;

namespace FreshEngine.AssetManagement;

/// <summary>
/// P/Invoke declarations for native Asset Manager API
/// </summary>
internal static class NativeAssetMethods
{
    private const string DllName = "FreshVoxelEngine.dll";

    // Asset Manager lifecycle
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr AssetManager_GetInstance();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool AssetManager_Initialize(IntPtr instance, [MarshalAs(UnmanagedType.LPStr)] string assetRootPath);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void AssetManager_Shutdown(IntPtr instance);

    // Asset operations
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool AssetManager_ProcessNewAsset(IntPtr instance, 
        [MarshalAs(UnmanagedType.LPStr)] string sourcePath,
        [MarshalAs(UnmanagedType.LPStr)] string assetType);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr AssetManager_GetAssetTypeFolder(IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string assetType);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool AssetManager_LoadAssetsFromLua(IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string configPath);
}

/// <summary>
/// Managed wrapper for the native Asset Manager
/// </summary>
public class AssetManagerWrapper : IDisposable
{
    private IntPtr _nativeHandle;
    private bool _disposed;
    private string _assetRootPath;

    /// <summary>
    /// Initializes the Asset Manager wrapper
    /// </summary>
    /// <param name="assetRootPath">Root path for assets</param>
    public AssetManagerWrapper(string assetRootPath)
    {
        _assetRootPath = assetRootPath;
        _nativeHandle = NativeAssetMethods.AssetManager_GetInstance();
        
        if (_nativeHandle == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to get native AssetManager instance");
        }

        if (!NativeAssetMethods.AssetManager_Initialize(_nativeHandle, assetRootPath))
        {
            throw new InvalidOperationException("Failed to initialize native AssetManager");
        }
    }

    /// <summary>
    /// Process a dropped asset file
    /// </summary>
    /// <param name="sourcePath">Full path to the source file</param>
    /// <param name="assetType">Type of asset (Texture, Model, Script, etc.)</param>
    /// <returns>True if successfully processed</returns>
    public bool ProcessAsset(string sourcePath, string assetType)
    {
        ThrowIfDisposed();
        return NativeAssetMethods.AssetManager_ProcessNewAsset(_nativeHandle, sourcePath, assetType);
    }

    /// <summary>
    /// Load assets from Lua configuration
    /// </summary>
    /// <param name="configPath">Path to asset_config.lua</param>
    /// <returns>True if successfully loaded</returns>
    public bool LoadAssetsFromLua(string configPath)
    {
        ThrowIfDisposed();
        return NativeAssetMethods.AssetManager_LoadAssetsFromLua(_nativeHandle, configPath);
    }

    /// <summary>
    /// Get the asset root path
    /// </summary>
    public string AssetRootPath => _assetRootPath;

    private void ThrowIfDisposed()
    {
        if (_disposed)
        {
            throw new ObjectDisposedException(nameof(AssetManagerWrapper));
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
                NativeAssetMethods.AssetManager_Shutdown(_nativeHandle);
                _nativeHandle = IntPtr.Zero;
            }
            _disposed = true;
        }
    }

    ~AssetManagerWrapper()
    {
        Dispose(false);
    }
}
