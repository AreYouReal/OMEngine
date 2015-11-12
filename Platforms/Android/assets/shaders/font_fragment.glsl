#version 300 es

uniform sampler2D uSamplerDiffuse;
uniform lowp vec4 uColor;

in lowp vec3 vTexCoord;

void main(){
    lowp vec4 color = texture2D( uSamplerDiffuse, vTexCoord );
    color.x = uColor.x;
    color.y = uColor.y;
    color.z = uColor.z;
    color.w *= uColor.w;
    gl_FragColor = color;
}