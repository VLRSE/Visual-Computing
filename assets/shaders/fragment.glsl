#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float time; //neue Uniform für Animation

void main(){

    float intensity = 0.5 + 0.5 * sin(time); //Wert zwischen 0.0 und 1.0
    vec3 animatedColor = colorVS * intensity;
    color = vec4(colorVS.x, colorVS.y, colorVS.z, 1.0);

}

