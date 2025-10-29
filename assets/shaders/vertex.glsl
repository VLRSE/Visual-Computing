#version 330 core
/*layout ist Attribut Pointer
**location ist vertex position
**in ist input
*/

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colorRGB;

out vec4 colorVS; //FRAGE: warum vec4?
uniform mat4 model; // 4-Dimensional Matrix

void main()
{
    gl_Position = model * vec4(pos, 1.0);
    colorVS = vec4(colorRGB, 1.0);
}
