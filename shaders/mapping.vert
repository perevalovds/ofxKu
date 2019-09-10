#version 120

//Shader for mapping quads, see details in ofxKuVideoMapping.h


varying vec2 texCoordVarying;

void main()
{
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}