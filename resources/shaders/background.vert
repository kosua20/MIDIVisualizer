#version 330

layout(location = 0) in vec3 v;

uniform bool horizontalMode = false;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

out INTERFACE {
	vec2 uv;
} Out ;


void main(){
	
	// We directly output the position.
	gl_Position = vec4(flipIfNeeded(v.xy), v.z, 1.0);
	// Output the UV coordinates computed from the positions.
	Out.uv = (v.xy) * 0.5 + 0.5;
	
}
