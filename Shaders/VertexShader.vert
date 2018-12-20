#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;
uniform mat4 tm, view, proj;

out vec3 colour;
void main() 
{
	colour = vc;
	gl_Position = proj * view * vec4(vp.x,vp.y,vp.z, 1.0f);
}