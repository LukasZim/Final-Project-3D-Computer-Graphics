#version 450
layout(location = 0) in vec3 position;
uniform mat4 depthProjectionMatrix;
layout(location = 15) uniform mat4 depthMVP;

void main()
{
    gl_Position = depthMVP * vec4(position, 1.0)
}
