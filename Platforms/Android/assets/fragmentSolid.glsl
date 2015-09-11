#version 300 es
precision mediump float;
out vec4 fragColor;

uniform mediump vec3    uLightPos;
uniform lowp vec3       uAmbient;
uniform lowp vec3       uDiffuse;
uniform lowp vec3       uSpecular;
uniform mediump float   uShininess;
uniform lowp    float   uDissolve;
uniform sampler2D       uSamplerDiffuse;

in mediump vec3    position;
in lowp    vec3    normal;



in mediump  vec3 texCoord;

void main(){
    lowp vec4 diffuseColor = texture( uSamplerDiffuse, texCoord.xy );
    
    lowp float alpha = diffuseColor.a;
    mediump vec3 L = normalize(uLightPos - position);
    mediump vec3 E = normalize(-position);
    mediump vec3 R = normalize(-reflect(L, normal));
    mediump vec4 ambient = vec4( uAmbient, 1.0 );
    mediump vec4 diffuse = vec4( uDiffuse * diffuseColor.rgb, 1.0) * max(dot(normal, L), 0.0);
    mediump vec4 specular = vec4( uSpecular, 1.0) * pow(max(dot(R,E), 0.0), uShininess * 0.3 );
    
    diffuseColor = vec4(0.1) + ambient + diffuse + specular;
    diffuseColor.a = alpha;
    
    fragColor = diffuseColor;
}