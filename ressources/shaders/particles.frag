#version 330

in INTERFACE {
	vec4 color;
	vec2 uv;
} In;

uniform sampler2D lookParticles;

out vec4 fragColor;


void main(){
	float alpha = texture(lookParticles, In.uv).r;
	fragColor = In.color;
	fragColor.a *= alpha;
	
}
