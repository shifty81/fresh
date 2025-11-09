// Voxel Rendering Shader for DirectX 11
// Combined vertex and pixel shader for rendering voxel world

// ============================================================================
// Constant Buffers
// ============================================================================

cbuffer MatrixBuffer : register(b0)
{
    matrix modelViewProj;
};

// ============================================================================
// Input/Output Structures
// ============================================================================

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
};

// ============================================================================
// Vertex Shader
// ============================================================================

PixelInput VSMain(VertexInput input)
{
    PixelInput output;
    
    // Transform position to clip space
    output.position = mul(float4(input.position, 1.0f), modelViewProj);
    
    // Pass through normal (should be transformed by normal matrix in production)
    output.normal = input.normal;
    
    // Pass world position for lighting
    output.worldPos = input.position;
    
    return output;
}

// ============================================================================
// Pixel Shader
// ============================================================================

float4 PSMain(PixelInput input) : SV_TARGET
{
    // Simple directional lighting
    float3 lightDir = normalize(float3(0.5f, 1.0f, 0.3f));
    float3 normal = normalize(input.normal);
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // Ambient + diffuse
    float3 ambient = float3(0.3f, 0.3f, 0.3f);
    float3 diffuse = float3(0.7f, 0.7f, 0.7f) * diff;
    
    // Base voxel color (green-ish grass)
    float3 color = float3(0.5f, 0.7f, 0.5f);
    float3 result = (ambient + diffuse) * color;
    
    return float4(result, 1.0f);
}
