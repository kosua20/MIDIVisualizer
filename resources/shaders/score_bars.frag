#version 330

in INTERFACE {
	float uv;
} In ;

uniform vec3 color;
out vec4 fragColor;


void main(){
	float uv = 1.0 - abs(In.uv);
	float gradU = fwidth(uv);
	float alpha = smoothstep(0.1 - gradU, 0.1 + gradU, uv);
	fragColor = vec4(color, alpha);
}
