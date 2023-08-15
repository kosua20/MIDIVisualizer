#version 330

layout(location = 0) in vec3 v;

uniform vec2 baseOffset;
uniform vec2 nextOffset;
uniform vec2 scale;
uniform bool horizontalMode;
uniform int digitCount;
uniform int firstMeasure;

vec2 flipIfNeeded(vec2 inPos){
	return horizontalMode ? vec2(inPos.y, -inPos.x) : inPos;
}

out INTERFACE {
	vec2 uv;
	float id;
} Out ;


void main(){
	float fullID = float(firstMeasure + gl_InstanceID);

	float currentDigitCount = floor(log(max(1, fullID))/log(10.0)) + 1;

	vec2 size = vec2(currentDigitCount, 1.0);
	vec2 maxSize = vec2(float(digitCount), 1.0);
	vec2 finalScale = 2.f * scale * size;
	vec2 maxFinalScale = 2.f * scale * maxSize;

	// We directly output the position.
	vec2 fullOffset = float(gl_InstanceID) * nextOffset + baseOffset;// + 0.5 * finalScale;

	if(horizontalMode){
		// Nothing to do on Y (which will become X) (already centered)
		// just center on X (which becomes Y)
		//Cheat to avoid digit touching the edge of the screen with offset 0.
		fullOffset.x += 0.6 * maxFinalScale.y;
	} else {
		// We want to right align, so use the max scale.
		fullOffset.x += maxFinalScale.x - 0.5 * finalScale.x;
		// Nothing to do on the Y axis.
	}
	vec2 pos = v.xy * finalScale + flipIfNeeded(fullOffset);
	gl_Position = vec4(pos, 0.1, 1.0);

	// Output the UV coordinates computed from the positions.
	// We flip the UVs so that the integer part corresponds to a power of ten
	Out.uv = (-v.xy + 0.5) * size;
	Out.id = fullID;


}
