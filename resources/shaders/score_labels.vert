#version 330

layout(location = 0) in vec3 v;

uniform vec2 baseOffset;
uniform vec2 nextOffset;
uniform vec2 scale;
uniform bool horizontalMode;
uniform float digitCount;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

out INTERFACE {
	vec2 uv;
	float id;
} Out ;


void main(){

	// We directly output the position.
	vec2 fullOffset = float(gl_InstanceID) * nextOffset + baseOffset;
	vec2 pos = v.xy * scale * 2.0f + flipIfNeeded( fullOffset);
	gl_Position = vec4(pos, 0.1, 1.0);
	// Output the UV coordinates computed from the positions.
	Out.uv = (v.xy + 0.5) * vec2(digitCount, 1.0);
	Out.id = float(gl_InstanceID);

}
