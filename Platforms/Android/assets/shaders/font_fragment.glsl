#version 300 es

precision mediump float;

uniform sampler2D uSamplerDiffuse;
uniform lowp vec4 uColor;

in lowp vec2 vTexCoord;

out vec4 fragColor;

void main(){
    lowp vec4 color = texture( uSamplerDiffuse, vTexCoord );
    color.x = uColor.x;
    color.y = uColor.y;
    color.z = uColor.z;
    color.w *= uColor.w;
    fragColor = color;
}