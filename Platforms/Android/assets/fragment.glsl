#version 300 es
precision mediump float;
out vec4 fragColor;

//in mediump  vec3 normal;
in lowp     vec3 lightColor;

void main(){
    fragColor = vec4(lightColor, 1.0) + vec4(0.1);
}