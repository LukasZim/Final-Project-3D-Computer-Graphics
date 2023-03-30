#version 450

layout(location = 3) uniform sampler2D colorMap;
layout(location = 4) uniform bool hasTexCoords;

layout(location = 5) uniform vec3 lightPosition;
layout(location = 6) uniform vec3 lightColor;
layout(location = 7) uniform vec3 viewPosition;

layout(location = 8) uniform vec3 materialAmbient;
layout(location = 9) uniform vec3 materialDiffuse;
layout(location = 10) uniform vec3 materialSpecular;
layout(location = 11) uniform float m_materialShininess;
layout(location = 12) in vec4 texColor;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;


layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 ambient, diffuse, specular;
    const vec3 Normalized = normalize(fragNormal);
    vec3 new_pos = normalize(lightPosition - fragPosition);
    
    // Ambient component
    ambient = materialAmbient * lightColor;
    
    // Diffuse component
    float intensity = max(dot(Normalized, new_pos), 0.0);
    diffuse = materialDiffuse * intensity * lightColor;
    
    // Specular component
    vec3 reflection = reflect(-new_pos, Normalized);
    vec3 new_pos_specular = normalize(viewPosition - fragPosition);
    float specular_intensity = pow(max(dot(reflection, new_pos_specular), 1.0), m_materialShininess);
    specular = materialSpecular * specular_intensity * lightColor;
    
    vec3 finalColor = ambient + diffuse + specular;
    vec4 texColor = vec4(1.0);
if (hasTexCoords) {
    texColor = texture(colorMap, fragTexCoord);
} else {
    texColor = vec4(1.0);
}
fragColor = vec4(finalColor * texColor.rgb, 1.0);
}