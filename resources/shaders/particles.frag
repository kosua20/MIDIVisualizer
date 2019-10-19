#version 330

in INTERFACE {
	vec4 color;
	vec2 uv;
	float id;
} In;

uniform sampler2DArray lookParticles;

out vec4 fragColor;


void main(){
	float alpha = texture(lookParticles, vec3(In.uv, In.id)).r;
	fragColor = In.color;
	fragColor.a *= alpha;
}
