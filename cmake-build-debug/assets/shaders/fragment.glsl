#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;


void main(){

    //time wird pro Frame von der CPU-Seite gesetzt

          // Farbverlauf basierend auf Zeit
            float r = abs(sin(time * 0.5));
            float g = abs(sin(time * 0.6 + 2.0));
            float b = abs(sin(time * 0.5 + 4.0));

        float intensity = 0.8 + 0.5 * sin(time); //Wert zwischen 0.0 und 1.0
        vec3 animatedColor = vec3(r, g, b) * colorVS ;
        color = vec4(animatedColor, 1.0);

}

