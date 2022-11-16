#version 330 core

// layouts
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec3 Color;
out vec2 TextureUV;

in INFO {
	vec3 Normal;
	vec3 Color;
	vec2 TextureUV;
	mat4 projection;
} info[];


void main(){
	for(int x = 0; x < 3; x++){
		gl_Position = info[x].projection * gl_in[x].gl_Position;
		Normal = info[x].Normal;
		Color = info[x].Color;
		TextureUV = info[x].TextureUV;
		EmitVertex();
	}
	EndPrimitive();
}