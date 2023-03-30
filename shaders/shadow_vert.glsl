#version 450
layout(location = 0) in vec3 position;

<<<<<<< Updated upstream
uniform mat4 depthProjectionMatrix;
layout(location = 15) uniform mat4 depthMVP;

void main()
{
    gl_Position = depthMVP * vec4(position, 1.0)
}
=======
// Values that stay constant for the whole mesh.
layout(location=3) uniform mat4 lightmvp;

void main(){
 gl_Position =  lightmvp * vec4(position,1);
}
>>>>>>> Stashed changes
