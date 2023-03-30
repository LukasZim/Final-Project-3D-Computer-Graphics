#version 450
<<<<<<< Updated upstream

layout(location = 0) out float fragmentDepth;
void main(){
	fragmentDepth = gl_FragCoord.z;
=======
layout(location = 0) out float fragmentdepth;

void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
>>>>>>> Stashed changes
}