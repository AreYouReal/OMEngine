#version 300 es
uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;
uniform mediump mat4 uNormalM;

layout(location = 0) in vec2 aPosition;
layout(location = 2) in vec2 aTexCoord;

out lowp vec2 vTexCoord;


void main(){
    vTexCoord = aTexCoord;
    gl_Position = uProjectionM * mat4(1.0f)* vec4(aPosition.x, aPosition.y, 0.0f, 1.0f);
}