#version 330

layout(location = 0) in vec2 v;

uniform float time;
uniform float scale;
uniform vec3 baseColor;
uniform vec2 inverseScreenSize;
uniform sampler2D textureParticles;
uniform vec2 inverseTextureSize;
uniform int globalId;
uniform float duration;

uniform float expansionFactor = 1.0;
uniform float speedScaling = 0.2;

#define notesCount 52.0

const float shifts[88] = float[](0, 0.5, 1, 2, 2.5, 3, 3.5, 4, 5, 5.5, 6, 6.5, 7, 7.5, 8, 9, 9.5, 10, 10.5, 11, 12, 12.5, 13, 13.5, 14, 14.5, 15, 16, 16.5, 17, 17.5, 18, 19, 19.5, 20, 20.5, 21, 21.5, 22, 23, 23.5, 24, 24.5, 25, 26, 26.5, 27, 27.5, 28, 28.5, 29, 30, 30.5, 31, 31.5, 32, 33, 33.5, 34, 34.5, 35, 35.5, 36, 37, 37.5, 38, 38.5, 39, 40, 40.5, 41, 41.5, 42, 42.5, 43, 44, 44.5, 45, 45.5, 46, 47, 47.5, 48, 48.5, 49, 49.5, 50, 51);

out INTERFACE {
	vec4 color;
	vec2 uv;
	float id;
} Out;


float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main(){
	Out.id = float(gl_InstanceID % 4);
	Out.uv = v + 0.5;
	// Fade color based on time.
	Out.color = vec4(baseColor, 1.0-time*time);
	
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
	shift.x *= pow(shift.y,0.3);
	
	// Combine global shift (due to note id) and local shift (based on read position).
	vec2 globalShift = vec2(-1.0 + (shifts[globalId] * 2.0 + 1.0) / notesCount,-0.58);
	vec2 localShift = 0.003 * scale * v + shift * duration * vec2(1.0,0.5);
	vec2 screenScaling = vec2(1.0,inverseScreenSize.y/inverseScreenSize.x);
	vec2 finalPos = globalShift + screenScaling * localShift;
	
	// Discard particles that reached the end of their trajectories by putting them off-screen.
	finalPos = mix(vec2(-200.0),finalPos, position.z);
	// Output final particle position.
	gl_Position = vec4(finalPos,0.0,1.0);
	
	
}
