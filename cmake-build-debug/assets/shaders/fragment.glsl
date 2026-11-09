#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float time;

void main(){

    float factor = 0.5 + 0.5 * (0.5 + 0.5 * sin(time));

    vec3 animatedColor = colorVS * factor;

    color = vec4(animatedColor.x, animatedColor.y, animatedColor.z, 1.0);

}

