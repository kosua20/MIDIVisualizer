#version 330

layout(location = 0) in vec3 v;

uniform vec2 baseOffset;
uniform vec2 nextOffset;
uniform vec2 scale;
uniform bool horizontalMode;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

out INTERFACE {
	float uv;
} Out ;


void main(){
	
	// We directly output the position.
	vec2 pos = v.xy * scale * 2.0f + float(gl_InstanceID) * nextOffset + baseOffset;
	gl_Position = vec4(flipIfNeeded(pos), 0.1, 1.0);
	// Output the UV coordinates computed from the positions.
	Out.uv = 2.0 * (scale.x > 0.99 ? v.y : v.x);
}
