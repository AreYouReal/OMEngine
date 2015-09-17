#version 300 es
precision mediump float;
out vec4 fragColor;

uniform sampler2D       uSamplerDiffuse;

in mediump  vec3        texCoord;
in lowp vec4            ambientColor;
in lowp vec4            diffuseColor;
in lowp vec4            specularColor;

void main(){
    lowp vec4 diffuseColorTexture = texture( uSamplerDiffuse, texCoord.xy );
    if(diffuseColorTexture.a < 0.1) discard;
    fragColor = ambientColor + diffuseColor * diffuseColorTexture + specularColor;
}