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
    const vec3 normalized_fragNormal = normalize(fragNormal);
    vec3 new_pos = normalize(lightPosition - fragPosition);
    
    // Ambient
    ambient = materialAmbient * lightColor;
    
    // Diffuse 
    float diffuse_reflection = dot(normalized_fragNormal, new_pos);
    float diffuse_intensity = 0.0;
    if(diffuse_reflection>=0.5){
        diffuse_intensity = 1.0;
    }
    else if (diffuse_reflection>=0.4){
		diffuse_intensity = 0.8;
	}
	else if (diffuse_reflection>=0.3){
		diffuse_intensity = 0.5;
	}
	else{
		diffuse_intensity = 0.4;
	}
    diffuse = materialDiffuse * diffuse_intensity * lightColor;
    
    // Specular 
    vec3 specular_reflection = reflect(-new_pos, normalized_fragNormal);
    vec3 specular_view = normalize(viewPosition - fragPosition);
    float specularFactor = pow(max(dot(specular_reflection, specular_view), 0.0), m_materialShininess);
    float specular_intensity = 0.0;
    if(specularFactor>0.5){
        specular_intensity = 1.0;
    }
    else if(specularFactor>0.2){
        specular_intensity = 0.8;
    }
    else{
        specular_intensity = 0.6;
    }
    specular = materialSpecular * specular_intensity * lightColor;
    
    vec3 finalColor = ambient + diffuse + specular; // Adding three components, making the final color
    vec4 texColor = vec4(1.0);
    if (hasTexCoords) {
        texColor = texture(colorMap, fragTexCoord);
    }
    fragColor = vec4(finalColor * texColor.rgb, 1.0);
}
