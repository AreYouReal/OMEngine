#version 300 es
precision mediump float;

struct Material{
    lowp vec4 ambient;
    lowp vec4 diffuse;
    lowp vec4 specular;
    mediump float shininess;
};

uniform Material uMaterial;
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
    lowp float intensity = max(dot(normal, lightDirectionTS), 0.0);
    fragColor = uMaterial.ambient;
    if(intensity > 0.0){
        lowp vec3 reflectionVec = normalize(-reflect(lightDirectionTS, normal));
        vec4 reflectiveComponent = uMaterial.specular * pow(max(dot(reflectionVec, position), 0.0), uMaterial.shininess);
        fragColor += uMaterial.diffuse * intensity * texture(uSamplerDiffuse, texCoord.xy) + reflectiveComponent;
    }
}