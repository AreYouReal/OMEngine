#version 300 es
precision mediump float;
out vec4 fragColor;

struct Material{
    lowp    vec4    ambient;
    lowp    vec4    diffuse;
    lowp    vec4    specular;
    mediump float   shininess;
};

struct Light{
    lowp    int     type;
    lowp    vec3    direction;
    lowp    vec3    position;
    lowp    vec4    color;
};

uniform Light uLight;
uniform Material uMaterial;
uniform sampler2D       uSamplerDiffuse;

in mediump  vec3    position;
in lowp     vec3    normal;
in mediump  vec3    texCoord;

void main(){
    lowp vec4 diffuseColor = texture( uSamplerDiffuse, texCoord.xy );
    mediump vec3 L;
    
    if(uLight.type > 0){
        L = normalize(uLight.position - position);
    }else{
        L = uLight.direction;
    }
    float intensity = max(dot(normal, L), 0.0);
    mediump vec4 specular;
    mediump vec4 diffuse;
    specular = diffuse = vec4(0, 0, 0, 1);
    if(intensity > 0.0){
        mediump vec3 E = normalize(-position);
        mediump vec3 R = normalize(-reflect(L, normal));
        diffuse = uMaterial.diffuse * diffuseColor * intensity * uLight.color;
        specular = uMaterial.specular * pow(max(dot(R,E), 0.0), uMaterial.shininess * 0.3 ) * uLight.color;
    }
    
    vec4 resultColor = uMaterial.ambient + diffuse + specular;
    
    fragColor = resultColor;
}