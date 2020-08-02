#version 330
#define CHANNELS_COUNT 8

layout(location = 0) in vec2 v;

uniform float time;
uniform float scale;
uniform vec3 baseColor[CHANNELS_COUNT];
uniform vec2 inverseScreenSize;
uniform sampler2D textureParticles;
uniform vec2 inverseTextureSize;

uniform int globalId;
uniform float duration;
uniform int channel;

uniform int texCount;
uniform float colorScale;

uniform float expansionFactor = 1.0;
uniform float speedScaling = 0.2;
uniform float keyboardHeight = 0.25;

uniform int minNote;
uniform float notesCount;

const float shifts[128] = float[](
0,0.5,1,1.5,2,3,3.5,4,4.5,5,5.5,6,7,7.5,8,8.5,9,10,10.5,11,11.5,12,12.5,13,14,14.5,15,15.5,16,17,17.5,18,18.5,19,19.5,20,21,21.5,22,22.5,23,24,24.5,25,25.5,26,26.5,27,28,28.5,29,29.5,30,31,31.5,32,32.5,33,33.5,34,35,35.5,36,36.5,37,38,38.5,39,39.5,40,40.5,41,42,42.5,43,43.5,44,45,45.5,46,46.5,47,47.5,48,49,49.5,50,50.5,51,52,52.5,53,53.5,54,54.5,55,56,56.5,57,57.5,58,59,59.5,60,60.5,61,61.5,62,63,63.5,64,64.5,65,66,66.5,67,67.5,68,68.5,69,70,70.5,71,71.5,72,73,73.5,74
);

out INTERFACE {
	vec4 color;
	vec2 uv;
	float id;
} Out;


float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main(){
	Out.id = float(gl_InstanceID % texCount);
	Out.uv = v + 0.5;
	// Fade color based on time.
	Out.color = vec4(colorScale * baseColor[channel], 1.0-time*time);
	
	float localTime = speedScaling * time * duration;
	float particlesCount = 1.0/inverseTextureSize.y;
	
	// Pick particle id at random.
	float particleId = float(gl_InstanceID) + floor(particlesCount * 10.0 * rand(vec2(globalId,globalId)));
	float textureId = mod(particleId,particlesCount);
	float particleShift = floor(particleId/particlesCount);
	
	// Particle uv, in pixels.
	vec2 particleUV = vec2(localTime / inverseTextureSize.x + 10.0 * particleShift, textureId);
	// UV in [0,1]
	particleUV = (particleUV+0.5)*vec2(1.0,-1.0)*inverseTextureSize;
	// Avoid wrapping.
	particleUV.x = clamp(particleUV.x,0.0,1.0);
	// We want to skip reading from the very beginning of the trajectories because they are identical.
	// particleUV.x = 0.95 * particleUV.x + 0.05;
	// Read corresponding trajectory to get particle current position.
	vec3 position = texture(textureParticles, particleUV).xyz;
	// Center position (from [0,1] to [-0.5,0.5] on x axis.
	position.x -= 0.5;
	
	// Compute shift, randomly disturb it.
	vec2 shift = 0.5*position.xy;
	float random = rand(vec2(particleId + float(globalId),time*0.000002+100.0*float(globalId)));
	shift += vec2(0.0,0.1*random);
	
	// Scale shift with time (expansion effect).
	shift = shift*time*expansionFactor;
	// and with altitude of the particle (ditto).
	shift.x *= max(0.5, pow(shift.y,0.3));
	
	// Horizontal shift is based on the note ID.
	float xshift = -1.0 + ((shifts[globalId] - shifts[int(minNote)]) * 2.0 + 1.0) / notesCount;
	//  Combine global shift (due to note id) and local shift (based on read position).
	vec2 globalShift = vec2(xshift, (2.0 * keyboardHeight - 1.0)-0.02);
	vec2 localShift = 0.003 * scale * v + shift * duration * vec2(1.0,0.5);
	vec2 screenScaling = vec2(1.0,inverseScreenSize.y/inverseScreenSize.x);
	vec2 finalPos = globalShift + screenScaling * localShift;
	
	// Discard particles that reached the end of their trajectories by putting them off-screen.
	finalPos = mix(vec2(-200.0),finalPos, position.z);
	// Output final particle position.
	gl_Position = vec4(finalPos,0.0,1.0);
	
	
}
