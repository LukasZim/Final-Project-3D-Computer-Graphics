#version 450

layout(location = 3) uniform sampler2D colorMap;
layout(location = 4) uniform bool hasTexCoords;

layout(location = 5) uniform vec3 lightPosition;
layout(location = 6) uniform vec3 lightColor;
layout(location = 7) uniform vec3 viewPosition;

layout(location = 8) uniform vec3 materialAmbient;
layout(location = 9) uniform vec3 materialDiffuse;
layout(location = 10) uniform vec3 materialSpecular;
layout(location = 11) uniform float materialShininess;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 texColor;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 ambient, diffuse, specular;
    const vec3 N = normalize(fragNormal);
    vec3 L = normalize(lightPosition - fragPosition);
    
    // Ambient component
    ambient = materialAmbient * lightColor;
    
    // Diffuse component
    float lambertian = max(dot(N, L), 0.0);
    diffuse = materialDiffuse * lambertian * lightColor;
    
    // Specular component
    vec3 R = reflect(-L, N);
    vec3 V = normalize(viewPosition - fragPosition);
    float specularFactor = pow(max(dot(R, V), 1.0), materialShininess);
    specular = materialSpecular * specularFactor * lightColor;
    
    vec3 finalColor = ambient + diffuse + specular;
    vec4 texColor = vec4(1.0);
if (hasTexCoords) {
    texColor = texture(colorMap, fragTexCoord);
} else {
    texColor = vec4(1.0, 1.0, 1.0, 1.0);
}
fragColor = vec4(finalColor * texColor.rgb, 1);

}
