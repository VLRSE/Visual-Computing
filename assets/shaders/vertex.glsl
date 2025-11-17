#version 330 core

layout (location = 0) in vec3 pos;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_color;

out vec3 colorVS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



//Beleuchtung
uniform vec3 lightPos;
out vec3 normal, lightDir;

void main(){
    colorVS = vertex_color;
    vec4 posVS =  model * vec4(pos, 1.0); // Position in Modell-Space → Welt-Space
    gl_Position = projection * view * posVS ;  // Projektionsmatrix transformiert ins Clip-Space



    // compute normal in view space //
    vec4 n = vec4(vertex_normal, 0.0); // Normalen als Vektor (w=0)
    mat4 normalMat = transpose(inverse(view * model)); // Normalen müssen invers-transponiert transformiert werden
    normal = (normalMat * n).xyz; // Normalen in View-Space umwandeln

    // compute light direction in view space //
    vec4 lp = vec4(view * vec4(lightPos, 1.0)); // Lichtposition in View-Space
    vec4 P = vec4(view * model * vec4(pos, 1.0));  // Fragmentposition in View-Space
    lightDir = (lp - P).xyz;  // Richtungsvektor vom Fragment zum Licht



}
