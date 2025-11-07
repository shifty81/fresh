using System;
using System.IO;
using System.Windows.Forms;
using FreshEngine.AssetManagement;

namespace FreshEngine.AssetDemo;

/// <summary>
/// Sample application demonstrating the Asset Management System
/// </summary>
public class Program
{
    [STAThread]
    public static void Main(string[] args)
    {
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        Application.SetHighDpiMode(HighDpiMode.SystemAware);

        // Determine asset root path
        string assetRootPath = args.Length > 0 
            ? args[0] 
            : Path.Combine(Environment.CurrentDirectory, "Assets");

        Console.WriteLine("==============================================");
        Console.WriteLine("Fresh Engine - Asset Management System Demo");
        Console.WriteLine("==============================================");
        Console.WriteLine($"Asset Root: {assetRootPath}");
        Console.WriteLine();
        Console.WriteLine("Features:");
        Console.WriteLine("  - Drag and drop files directly into the window");
        Console.WriteLine("  - Automatic asset type detection");
        Console.WriteLine("  - File system monitoring with FileSystemWatcher");
        Console.WriteLine("  - Integration with C++ core via P/Invoke");
        Console.WriteLine("  - Lua configuration file management");
        Console.WriteLine();
        Console.WriteLine("Opening Asset Management window...");
        Console.WriteLine("==============================================");
        Console.WriteLine();

        try
        {
            // Create and run the asset management form
            using var form = new AssetManagementForm(assetRootPath);
            Application.Run(form);
        }
        catch (Exception ex)
        {
            MessageBox.Show(
                $"Fatal error: {ex.Message}\n\n" +
                $"Stack trace:\n{ex.StackTrace}",
                "Application Error",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            
            Console.WriteLine($"ERROR: {ex.Message}");
            Console.WriteLine(ex.StackTrace);
        }
    }
}
