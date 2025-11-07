using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace FreshEngine.AssetManagement;

/// <summary>
/// Main form for Asset Management with drag-and-drop support
/// </summary>
public class AssetManagementForm : Form
{
    private AssetManagerWrapper? assetManager;
    private FileSystemWatcher? fileWatcher;
    private TextBox statusTextBox;
    private Label dropLabel;
    private Panel dropPanel;
    private Button browseButton;
    private CheckBox watchFolderCheckBox;
    private string assetRootPath;

    public AssetManagementForm(string assetRootPath)
    {
        this.assetRootPath = assetRootPath;
        InitializeComponents();
        InitializeAssetManager();
        SetupFileWatcher();
    }

    private void InitializeComponents()
    {
        // Form settings
        Text = "Fresh Engine - Asset Management";
        Size = new Size(700, 500);
        StartPosition = FormStartPosition.CenterScreen;

        // Drop panel
        dropPanel = new Panel
        {
            Location = new Point(20, 20),
            Size = new Size(640, 200),
            BorderStyle = BorderStyle.FixedSingle,
            BackColor = Color.FromArgb(240, 240, 255),
            AllowDrop = true
        };
        dropPanel.DragEnter += DropPanel_DragEnter;
        dropPanel.DragDrop += DropPanel_DragDrop;

        // Drop label
        dropLabel = new Label
        {
            Text = "Drag and drop asset files here\n\n" +
                   "Supported types:\n" +
                   "• Textures (.png, .jpg, .bmp, .tga, .dds)\n" +
                   "• Models (.obj, .fbx, .gltf, .glb)\n" +
                   "• Scripts (.lua, .js, .py)\n" +
                   "• Audio (.wav, .mp3, .ogg, .flac)\n" +
                   "• Shaders (.glsl, .hlsl, .vert, .frag)\n" +
                   "• Configs (.json, .xml, .yaml, .ini)",
            Dock = DockStyle.Fill,
            TextAlign = ContentAlignment.MiddleCenter,
            Font = new Font("Segoe UI", 10F, FontStyle.Regular),
            ForeColor = Color.FromArgb(80, 80, 120)
        };
        dropPanel.Controls.Add(dropLabel);

        // Browse button
        browseButton = new Button
        {
            Text = "Browse for Files...",
            Location = new Point(20, 240),
            Size = new Size(150, 35)
        };
        browseButton.Click += BrowseButton_Click;

        // Watch folder checkbox
        watchFolderCheckBox = new CheckBox
        {
            Text = $"Monitor asset folder for changes",
            Location = new Point(200, 245),
            Size = new Size(300, 25),
            Checked = true
        };
        watchFolderCheckBox.CheckedChanged += WatchFolderCheckBox_CheckedChanged;

        // Status text box
        statusTextBox = new TextBox
        {
            Location = new Point(20, 280),
            Size = new Size(640, 150),
            Multiline = true,
            ScrollBars = ScrollBars.Vertical,
            ReadOnly = true,
            BackColor = Color.White
        };

        // Add controls
        Controls.Add(dropPanel);
        Controls.Add(browseButton);
        Controls.Add(watchFolderCheckBox);
        Controls.Add(statusTextBox);

        // Enable drag-and-drop for the form itself
        AllowDrop = true;
        DragEnter += DropPanel_DragEnter;
        DragDrop += DropPanel_DragDrop;

        LogStatus($"Asset Management initialized. Asset root: {assetRootPath}");
    }

    private void InitializeAssetManager()
    {
        try
        {
            assetManager = new AssetManagerWrapper(assetRootPath);
            LogStatus("Asset Manager initialized successfully.");
        }
        catch (Exception ex)
        {
            LogStatus($"ERROR: Failed to initialize Asset Manager: {ex.Message}");
            MessageBox.Show($"Failed to initialize Asset Manager:\n{ex.Message}", 
                "Initialization Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }

    private void SetupFileWatcher()
    {
        try
        {
            // Ensure directory exists
            if (!Directory.Exists(assetRootPath))
            {
                Directory.CreateDirectory(assetRootPath);
                LogStatus($"Created asset directory: {assetRootPath}");
            }

            fileWatcher = new FileSystemWatcher(assetRootPath)
            {
                NotifyFilter = NotifyFilters.FileName | NotifyFilters.LastWrite | NotifyFilters.DirectoryName,
                IncludeSubdirectories = true,
                EnableRaisingEvents = watchFolderCheckBox.Checked
            };

            fileWatcher.Created += FileWatcher_Changed;
            fileWatcher.Changed += FileWatcher_Changed;
            fileWatcher.Deleted += FileWatcher_Changed;
            fileWatcher.Renamed += FileWatcher_Renamed;

            LogStatus("File system watcher initialized.");
        }
        catch (Exception ex)
        {
            LogStatus($"WARNING: Could not set up file watcher: {ex.Message}");
        }
    }

    private void DropPanel_DragEnter(object? sender, DragEventArgs e)
    {
        if (e.Data?.GetDataPresent(DataFormats.FileDrop) == true)
        {
            e.Effect = DragDropEffects.Copy;
            dropPanel.BackColor = Color.FromArgb(220, 240, 255);
        }
        else
        {
            e.Effect = DragDropEffects.None;
        }
    }

    private void DropPanel_DragDrop(object? sender, DragEventArgs e)
    {
        dropPanel.BackColor = Color.FromArgb(240, 240, 255);

        if (e.Data?.GetData(DataFormats.FileDrop) is not string[] files || files.Length == 0)
        {
            return;
        }

        foreach (string filePath in files)
        {
            ProcessDroppedFile(filePath);
        }
    }

    private void BrowseButton_Click(object? sender, EventArgs e)
    {
        using var openFileDialog = new OpenFileDialog
        {
            Title = "Select Asset Files",
            Multiselect = true,
            Filter = "All Supported Files|*.png;*.jpg;*.jpeg;*.bmp;*.tga;*.dds;*.obj;*.fbx;*.gltf;*.glb;*.dae;*.lua;*.js;*.py;*.wav;*.mp3;*.ogg;*.flac;*.glsl;*.hlsl;*.vert;*.frag;*.vs;*.fs;*.json;*.xml;*.yaml;*.yml;*.ini|" +
                     "All Files|*.*"
        };

        if (openFileDialog.ShowDialog() == DialogResult.OK)
        {
            foreach (string filePath in openFileDialog.FileNames)
            {
                ProcessDroppedFile(filePath);
            }
        }
    }

    private void ProcessDroppedFile(string filePath)
    {
        try
        {
            if (!File.Exists(filePath))
            {
                LogStatus($"ERROR: File not found: {filePath}");
                return;
            }

            string fileName = Path.GetFileName(filePath);
            LogStatus($"Processing file: {fileName}");

            // Prompt for asset type
            string? assetType = AssetTypeDialog.ShowDialog(fileName);
            
            if (string.IsNullOrEmpty(assetType))
            {
                LogStatus("Asset import canceled by user.");
                return;
            }

            // Process the asset using the native manager
            if (assetManager != null && assetManager.ProcessAsset(filePath, assetType))
            {
                LogStatus($"SUCCESS: {fileName} imported as {assetType}");
            }
            else
            {
                LogStatus($"ERROR: Failed to import {fileName}");
            }
        }
        catch (Exception ex)
        {
            LogStatus($"ERROR: Exception during file processing: {ex.Message}");
        }
    }

    private void WatchFolderCheckBox_CheckedChanged(object? sender, EventArgs e)
    {
        if (fileWatcher != null)
        {
            fileWatcher.EnableRaisingEvents = watchFolderCheckBox.Checked;
            LogStatus(watchFolderCheckBox.Checked 
                ? "File system monitoring enabled." 
                : "File system monitoring disabled.");
        }
    }

    private void FileWatcher_Changed(object sender, FileSystemEventArgs e)
    {
        if (InvokeRequired)
        {
            Invoke(new Action(() => FileWatcher_Changed(sender, e)));
            return;
        }

        LogStatus($"File {e.ChangeType}: {e.Name}");
    }

    private void FileWatcher_Renamed(object sender, RenamedEventArgs e)
    {
        if (InvokeRequired)
        {
            Invoke(new Action(() => FileWatcher_Renamed(sender, e)));
            return;
        }

        LogStatus($"File Renamed: {e.OldName} -> {e.Name}");
    }

    private void LogStatus(string message)
    {
        if (InvokeRequired)
        {
            Invoke(new Action(() => LogStatus(message)));
            return;
        }

        string timestamp = DateTime.Now.ToString("HH:mm:ss");
        statusTextBox.AppendText($"[{timestamp}] {message}\r\n");
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            fileWatcher?.Dispose();
            assetManager?.Dispose();
        }
        base.Dispose(disposing);
    }
}
