#version 450
layout(location = 0) in vec3 position;
uniform mat4 depthProjectionMatrix;
layout(location = 12) uniform mat4 lightmvp;

void main()
{
    gl_Position = lightmvp * vec4(position, 1.0)
}
