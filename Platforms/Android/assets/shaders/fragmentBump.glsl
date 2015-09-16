#version 300 es
precision mediump float;

uniform lowp vec3       uAmbient;
uniform lowp vec3       uDiffuse;
uniform lowp vec3       uSpecular;
uniform mediump float   uShininess;
uniform lowp    float   uDissolve;

uniform mediump sampler2D uSamplerDiffuse;
uniform mediump sampler2D uSamplerBump;

in lowp vec3 texCoord;
in lowp vec3 position;
in lowp vec3 normal;
in lowp vec3 lightDirectionTS;

out vec4 fragColor;

void main(){
    lowp vec3 normal = texture(uSamplerBump, texCoord.xy).rgb * 2.0 - 1.0;
    lowp float intensity = max(dot(lightDirectionTS, normal), 0.0);
    fragColor = vec4(0.1);
    if(intensity > 0.0){
        fragColor += (vec4(uDiffuse, 1.0) * intensity) * texture(uSamplerDiffuse, texCoord.xy);
    }    
}