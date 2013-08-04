#version 330 core  

precision highp float;
precision highp int;

uniform mat4 g_ModelviewProjection;
in vec4 iPosition;

void main()
{
	gl_Position = g_ModelviewProjection * iPosition;
}
