using System;
using System.Drawing;
using System.Windows.Forms;

namespace FreshEngine.AssetManagement;

/// <summary>
/// Dialog for prompting user to select asset type
/// </summary>
public class AssetTypeDialog : Form
{
    private ComboBox assetTypeComboBox;
    private Button okButton;
    private Button cancelButton;
    private Label instructionLabel;

    public string SelectedAssetType { get; private set; } = string.Empty;

    public AssetTypeDialog(string fileName)
    {
        InitializeComponents(fileName);
    }

    private void InitializeComponents(string fileName)
    {
        // Form settings
        Text = "Select Asset Type";
        Size = new Size(400, 200);
        FormBorderStyle = FormBorderStyle.FixedDialog;
        StartPosition = FormStartPosition.CenterScreen;
        MaximizeBox = false;
        MinimizeBox = false;

        // Instruction label
        instructionLabel = new Label
        {
            Text = $"What type of asset is '{fileName}'?",
            Location = new Point(20, 20),
            Size = new Size(360, 40),
            TextAlign = ContentAlignment.MiddleLeft
        };

        // Asset type combo box
        assetTypeComboBox = new ComboBox
        {
            Location = new Point(20, 70),
            Size = new Size(360, 25),
            DropDownStyle = ComboBoxStyle.DropDownList
        };

        // Populate asset types
        assetTypeComboBox.Items.AddRange(new object[]
        {
            "Texture",
            "Model",
            "Script",
            "Audio",
            "Shader",
            "Config"
        });

        // Auto-detect based on extension
        string extension = System.IO.Path.GetExtension(fileName).ToLower();
        string suggestedType = DetectAssetType(extension);
        if (!string.IsNullOrEmpty(suggestedType))
        {
            assetTypeComboBox.SelectedItem = suggestedType;
        }
        else
        {
            assetTypeComboBox.SelectedIndex = 0;
        }

        // OK button
        okButton = new Button
        {
            Text = "OK",
            Location = new Point(200, 120),
            Size = new Size(80, 30),
            DialogResult = DialogResult.OK
        };
        okButton.Click += OkButton_Click;

        // Cancel button
        cancelButton = new Button
        {
            Text = "Cancel",
            Location = new Point(290, 120),
            Size = new Size(80, 30),
            DialogResult = DialogResult.Cancel
        };

        // Add controls
        Controls.Add(instructionLabel);
        Controls.Add(assetTypeComboBox);
        Controls.Add(okButton);
        Controls.Add(cancelButton);

        AcceptButton = okButton;
        CancelButton = cancelButton;
    }

    private void OkButton_Click(object? sender, EventArgs e)
    {
        if (assetTypeComboBox.SelectedItem != null)
        {
            SelectedAssetType = assetTypeComboBox.SelectedItem.ToString() ?? string.Empty;
        }
    }

    private string DetectAssetType(string extension)
    {
        return extension switch
        {
            ".png" or ".jpg" or ".jpeg" or ".bmp" or ".tga" or ".dds" => "Texture",
            ".obj" or ".fbx" or ".gltf" or ".glb" or ".dae" => "Model",
            ".lua" or ".js" or ".py" => "Script",
            ".wav" or ".mp3" or ".ogg" or ".flac" => "Audio",
            ".glsl" or ".hlsl" or ".vert" or ".frag" or ".vs" or ".fs" => "Shader",
            ".json" or ".xml" or ".yaml" or ".yml" or ".ini" => "Config",
            _ => string.Empty
        };
    }

    /// <summary>
    /// Show the dialog and return the selected asset type
    /// </summary>
    public static string? ShowDialog(string fileName)
    {
        using var dialog = new AssetTypeDialog(fileName);
        if (dialog.ShowDialog() == DialogResult.OK)
        {
            return dialog.SelectedAssetType;
        }
        return null;
    }
}
