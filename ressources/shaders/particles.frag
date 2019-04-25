#version 330

in INTERFACE {
	vec4 color;
	vec2 uv;
	float id;
} In;

uniform sampler2D lookParticles0;
uniform sampler2D lookParticles1;
uniform sampler2D lookParticles2;
uniform sampler2D lookParticles3;

out vec4 fragColor;


void main(){
	int lid = int(In.id);
	float alpha = texture((lid == 0 ? lookParticles0 : (lid == 1 ? lookParticles1 : (lid == 2 ? lookParticles2 : lookParticles3))), In.uv).r;
	fragColor = In.color;
	fragColor.a *= alpha;
	
}
