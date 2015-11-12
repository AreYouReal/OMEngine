#version 300 es
uniform mediump uModelViewM;
uniform mediump uProjectionM;
uniform mediump uNormalM;

layout(location = 0) in vec4 aPosition;
layout(locatino = 2) in vec3 aTexCoord;

out lowp vec2 vTexCoord;


void main(){
    vTexCoord = aTexCoord;
    gl_Position = uProjectionM * uModelViewM * vec4(aPosition.x, aPosition.y, 0.0f, 1.0f);
}