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

// for shadow mapping
layout(location = 13) uniform sampler2D texShadow;
layout(location = 14) uniform mat4 lightMVP;


layout(location = 15) uniform vec3 lightPosition2;
layout(location = 16) uniform vec3 lightColor2;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;


layout(location = 0) out vec4 fragColor;


float CalcShadowFactorPCF(){
    vec4 fragLightCoord = lightMVP * vec4(fragPosition, 1.0);

    // Divide by w because fragLightCoord are homogeneous coordinates
    fragLightCoord.xyz /= fragLightCoord.w;

    // The resulting value is in NDC space (-1 to +1),
    //  we transform them to texture space (0 to 1).
    fragLightCoord.xyz = fragLightCoord.xyz * 0.5 + 0.5;

    float TexelWidth = 1.0/ 1024;
    float TexelHeight = 1.0/ 1024;

    vec2 TexelSize = vec2(TexelWidth, TexelHeight);

    float ShadowSum = 0.0;

    for (int y = -1; y <= 1; y++){
        for (int x = -1; x <= 1; x++){
            vec2 Offset = vec2(x,y) * TexelSize;
            float depth = texture(texShadow, fragLightCoord.xy + Offset).x;

            if (depth + (0.001 / length(fragPosition - lightPosition)) >= fragLightCoord.z){
                ShadowSum += 1.0;
            }
        }
    }

    return ShadowSum / 9.0 * max(0.0, 2 * (0.5 - length(vec2(0.5, 0.5) - fragLightCoord.xy)));
}

vec3 phong(vec3 lightPos, vec3 lightCol){
    vec3 ambient, diffuse, specular;
    const vec3 Normalized = normalize(fragNormal);
    vec3 new_pos = normalize(lightPos - fragPosition);
    
    // Ambient component
    ambient = materialAmbient * lightCol;
    
    // Diffuse component
    float diffuse_intensity = max(dot(Normalized, new_pos), 0.0);
    diffuse = materialDiffuse * diffuse_intensity * lightCol;
    
    // Specular component
    vec3 specular_reflection = reflect(-new_pos, Normalized);
    vec3 specular_new_pos = normalize(viewPosition - fragPosition);
    float specular_intensity = pow(max(dot(specular_reflection, specular_new_pos), 1.0), m_materialShininess);
    specular = materialSpecular * specular_intensity * lightCol;
    
    vec3 finalColor = ambient + diffuse + specular;
    return finalColor;
}


void main()
{

    vec4 texColor = vec4(1.0);
    if (hasTexCoords) {
        texColor = texture(colorMap, fragTexCoord);
    } else {
        texColor = vec4(1.0);
    }
    vec3 finalColor = phong(lightPosition, lightColor);
    vec3 finalColor2 = phong(lightPosition2, lightColor2);
    fragColor = vec4((finalColor* CalcShadowFactorPCF() + finalColor2), 1.0) * texColor;
    

}
