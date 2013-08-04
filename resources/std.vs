#version 150 core  

precision highp float;
precision highp int;

uniform mat4 g_ModelviewProjection;
in vec2 iPosition;

void main()
{
	gl_Position    = g_ModelviewProjection * iPosition;
}
