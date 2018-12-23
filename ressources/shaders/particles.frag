#version 330

in INTERFACE {
	vec4 color;
	vec2 uv;
	float id;
} In;

uniform sampler2D lookParticles[4];

out vec4 fragColor;


void main(){
	float alpha = texture(lookParticles[int(In.id)], In.uv).r;
	fragColor = In.color;
	fragColor.a *= alpha;
	
}
