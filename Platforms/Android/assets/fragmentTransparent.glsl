#version 300 es
precision mediump float;
out vec4 fragColor;

uniform sampler2D Diffuse;

//in mediump  vec3 normal;
in lowp     vec3 lightColor;
in mediump  vec3 texCoord;

void main(){
    lowp vec4 diffuseColor = texture( Diffuse, texCoord.xy );

    fragColor = diffuseColor;
    fragColor.a = 0.65;
    
    fragColor *= vec4(lightColor, 1.0);
}