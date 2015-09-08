#version 300 es
precision mediump float;
out vec4 fragColor;

uniform sampler2D Diffuse;

//in mediump  vec3 normal;
in lowp     vec3 lightColor;
in mediump  vec3 texCoord;

void main(){
    fragColor = texture( Diffuse, texCoord.xy) + vec4(0.3);
}