#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 outColor;

void main() {
    // Simple directional lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
    
    // Ambient + diffuse
    vec3 ambient = vec3(0.3);
    vec3 diffuse = vec3(0.7) * diff;
    
    vec3 color = vec3(0.5, 0.7, 0.5); // Base voxel color
    vec3 result = (ambient + diffuse) * color;
    
    outColor = vec4(result, 1.0);
}
