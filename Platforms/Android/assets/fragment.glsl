//varying lowp vec3 normal;
//
//void main(void){
//    gl_FragColor = vec4(normal, 1.0);
//}

#version 300 es
precision mediump float;
out vec4 fragColor;

in mediump vec3 normal;
void main(){ fragColor = vec4(normal, 1.0); }