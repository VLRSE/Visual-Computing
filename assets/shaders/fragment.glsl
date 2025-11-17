#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float time; //neue Uniform für Animation

in vec3 normal, lightDir;
uniform vec3 lightColor; //Light Color // Farbe der Lichtquelle (z.B. weiß)
//uniform vec3 objectColor; //ObjectColor
uniform bool isLightSource; // True, wenn es die Licht-Box ist

uniform float matShininess = 45.0;// Glanzwert für specular

uniform vec3 viewPos;       // Kameraposition für specular

void main(){

   if (isLightSource)
       {
           // Light cube: show solid color
           color = vec4(lightColor, 1.0);
       }
       else
       {
           // Normal diffuse shading for other objects
           vec3 N = normalize(normalize(normal)); // Normalen normalisieren
           vec3 L = normalize(normalize(lightDir)); // Licht-Richtung normalisieren


            //Diffus Komponente
           float diff = max(dot(N, L), 0.0); // Lambert-Beleuchtung: cos(N,L)
           vec3 result = lightColor * vec3(1.0) * diff; // lightColor = white

           //Ambient Komponente
           float AmbientStrength = 0.2f;
           vec3 ambient = AmbientStrength * colorVS * lightColor;


            //Spekulare Komponente
             vec3 V = normalize(viewPos - (gl_FragCoord.xyz)); // Blickrichtung zum Fragment
             vec3 R = normalize(reflect(-L, N)); // reflektierter Lichtvektor
             float cosBeta = max(dot(R,V),0.0); // Winkel zwischen Reflektion und Kamera
             float spec = pow(cosBeta, matShininess); // Phong-Glanzmodell



             // Farbverlauf basierend auf Zeit
               float r = abs(sin(time * 0.5));
               float g = abs(sin(time * 0.6 + 2.0));
               float b = abs(sin(time * 0.5 + 4.0));

           float intensity = 0.8 + 0.5 * sin(time); //Wert zwischen 0.0 und 1.0
           vec3 animatedColor = vec3(r, g, b) * colorVS ;

           vec3 finalColor = result + ambient + spec + animatedColor;
           color = vec4(finalColor, 1.0);

       }
}

