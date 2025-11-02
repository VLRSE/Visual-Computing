#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float time; //neue Uniform für Animation

void main(){

    //time wird pro Frame von der CPU-Seite gesetzt
    float intensity = 0.5 + 0.5 * sin(time); //Wert zwischen 0.0 und 1.0
    vec3 animatedColor = colorVS * intensity;
    color = vec4(animatedColor, 1.0);

}

