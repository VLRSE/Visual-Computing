#version 330 core
//layout ist Attribut Pointer
//location ist vertex position
//in ist input
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colorRGB;

out vec3 colorVS;
uniform mat4 model; // 4-Dimensional Matrix

void main(){
    colorVS = colorRGB;
    gl_Position = model * vec4(pos, 1.0);


}
