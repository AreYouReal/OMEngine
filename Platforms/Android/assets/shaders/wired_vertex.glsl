#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;

struct Material{
    lowp vec4 ambient;
};

uniform Material uMaterial;

layout(location = 0) in vec4 aPosition;

out vec4 color;

void main(){
    gl_Position = uProjectionM * uModelViewM * aPosition;
    color = uMaterial.ambient;
}