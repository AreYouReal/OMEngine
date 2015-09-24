#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;
uniform mediump mat4 uNormalM;
uniform mediump vec3 uLightPos;

uniform lowp vec3       uAmbient;
uniform lowp vec3       uDiffuse;
uniform lowp vec3       uSpecular;
uniform mediump float   uShininess;
uniform lowp    float   uDissolve;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec3 aTexCoord;

out mediump vec3 texCoord;
out lowp    vec4 ambientColor;
out lowp    vec4 diffuseColor;
out lowp    vec4 specularColor;

void main(){
    lowp vec3 normal = normalize(vec3(uNormalM * aNormal));
    mediump vec3 position = (uModelViewM * aPosition).xyz;
    lowp vec3 L = normalize(uLightPos - position);
    
    mediump vec3 E = normalize(-position);
    mediump vec3 R = normalize(-reflect(L, normal));

    gl_Position = uProjectionM * uModelViewM * aPosition;
    
    texCoord = aTexCoord;
    ambientColor = vec4(uAmbient, 1.0);
    float intensity = max(dot(normal, L), 0.0);
    if(intensity > 0.0){
        diffuseColor = vec4( uDiffuse, 1.0) * intensity;
        specularColor = vec4( uSpecular, 1.0) * pow(max(dot(R,E), 0.0), uShininess * 0.3 );
    }else{
        diffuseColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        specularColor = vec4(0.0f, 0.0f, .0f, 1.0f);
    }
}