#version 330

// Volumetric keyboard: every key is a rounded box, raymarched as a signed distance field.
// The camera looks from the front and above; at the depth of the line where the notes land
// the scale is exactly the one of the flat keyboard, so the notes still fall onto their keys.
// Light comes from the point where a note touches the keyboard, with soft shadows.
// Distances are expressed in white key widths.

#define SETS_COUNT 12
#define MAJOR_COUNT 75

uniform bool highlightKeys;
uniform float keyboardHeight = 0.25;
uniform float notesCount; // (maxNoteMajor - minNoteMajor + 1)
uniform int minNoteMajor;
uniform int actives[128];
uniform float keyTravel[128]; ///< Animated travel of each key, 0: rest, 1: bottom.
#define MAX_NOTE_LIGHTS 24
uniform int noteLightCount = 0;
uniform vec3 noteLights[MAX_NOTE_LIGHTS]; ///< Lit notes: position along the keyboard, height of the light, distance from the canvas.
uniform vec3 noteLightColors[MAX_NOTE_LIGHTS];
// Baking: the keyboard at rest is rendered once into bakeTex. Each frame, only the parts of the keyboard
// where something happens (lit notes, keys still moving) are computed again, the rest is read back.
#define MAX_DYNAMIC_RANGES 48
uniform bool useBake = false;
uniform sampler2D bakeTex;
uniform int dynamicRangeCount = 0;
uniform vec2 dynamicRanges[MAX_DYNAMIC_RANGES]; ///< Intervals along the keyboard, in white key widths.
uniform vec2 inverseScreenSize;
uniform vec3 edgeColor = vec3(0.0);
uniform vec3 keyColor = vec3(1.0);
uniform vec3 majorColor[SETS_COUNT];
uniform vec3 minorColor[SETS_COUNT];
uniform float minorsWidth = 1.0;

uniform float tilt = 40.0; ///< Camera angle below the horizon, in degrees (90: top view).
uniform float perspective = 91.0; ///< Camera distance to the keyboard, in white key widths.
uniform float fillLight = 0.7; ///< Light from the viewer, so that the key fronts do not merge with the key slip.
uniform float ambientLight = 0.7;
uniform float noteLight = 10.0; ///< Intensity of the light emitted where a note touches its key.
uniform float noteLightRadius = 2.0; ///< Falloff radius of that light.
uniform float keyDip = 0.4; ///< How deep a pressed key goes, at its front.
uniform vec3 feltColor = vec3(0.42, 0.04, 0.06);
uniform float backLight = 60.0; ///< Constant warm bulb standing on the felt strip, behind the keys.
uniform vec3 backLightColor = vec3(1.0, 0.72, 0.38);
uniform float backLightRadius = 1.3; ///< Falloff radius of the bulb, in white key widths.
uniform float backLightPosition = 0.5; ///< Where the bulb stands along the keyboard, 0: left, 1: right.
uniform float backLightHeight = 10.0; ///< Height of the bulb above the white keys, in white key widths.

const int majorIds[MAJOR_COUNT] = int[](0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105, 107, 108, 110, 112, 113, 115, 117, 119, 120, 122, 124, 125, 127);

// Geometry, real proportions (white key width 23.5mm).
const float WHITE_GAP = 0.069;
const float WHITE_DEPTH = 0.894;
const float KEYTOP_H = 0.2; ///< Height of the plastic cap front.
const float KEYTOP_INSET = 0.012; ///< How much the body under the cap is set back (~0.3mm).
const float WHITE_ROUND = 0.08; ///< ~2mm rounded edges, they catch the light.
const float BLACK_H = 0.345; ///< How much black keys rise above white ones.
const float BLACK_ROUND = 0.071;
const float BLACK_LENGTH = 95.0 / 150.0; ///< Relative to white keys.
const float FELT_H = 0.081;
const float FELT_D = 0.183;
const float SLIP_D = 1.22;
const float SLIP_DROP = 0.65; ///< White keys rise 15mm above the key slip.
const float BEHIND = 6.1; ///< Keys continue under the canvas.
const float FRONT_FRACTION = 0.824; ///< Where the front of the keys lands, as a fraction of the keyboard height.

const int MAT_OTHER = 0;
const int MAT_WHITE = 1;
const int MAT_BLACK = 2;
const int MAT_FELT = 3;
const int MAT_CASE = 4;
const int MAT_MASCOT = 5;
const int MAT_BULB = 6;

// A real piano: A0 to C8. When the visualizer shows a wider range, the columns outside it hold the cheek blocks
// of the case instead of keys: notes still land on their keys, and the lowest keys move away from the frame edge.
const int PIANO_LOW = 21;
const int PIANO_HIGH = 108;
const float CHEEK_H = 0.9; ///< Height of the cheek blocks above the white keys (~2cm).
const float CHEEK_FRONT = 0.25; ///< How far they stand out in front of the keys.
const float CHEEK_ROUND = 0.2;
const float CHEEK_GAP = 0.04; ///< Gap between a cheek block and the outermost key.

out vec4 fragColor;

// Visible length of the keys, fitted to the camera so that the keyboard fills its band.
float zBack;
float zBlack;
// Edges of the real piano keys, in white key widths.
float pianoLeft;
float pianoRight;
// Mascot: the point where it stands, and whether it is shown.
vec3 mascotBase;
bool mascotOn;
// Its light can be inside it (the heart of a flower): rays towards that light must not be stopped by the model.
bool shadowIgnoresMascot = false;
// Highest point of the scene: rays start just above it, and shadow rays going higher are lit.
float sceneTop = 0.6;

float sdBox(vec3 p, vec3 b, float r){
	vec3 q = abs(p) - b + r;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - r;
}

int whiteNote(int i){
	return majorIds[clamp(i + minNoteMajor, 0, MAJOR_COUNT - 1)];
}

int activeSet(int note){
	return highlightKeys ? actives[note] : -1;
}

// Black key between white keys i and i+1, placed as in the flat keyboard. Returns its note or -1.
int blackAfter(int i, out float center){
	center = 0.0;
	if(i < 0 || i >= int(notesCount) - 1){
		return -1;
	}
	int n = whiteNote(i);
	int pc = n % 12;
	if(pc == 4 || pc == 11 || n >= 127){
		return -1;
	}
	int b = n + 1;
	int bpc = b % 12;
	float shift = (bpc == 1 || bpc == 6) ? -0.09 : ((bpc == 3 || bpc == 10) ? 0.09 : 0.0);
	center = float(i + 1) + shift * minorsWidth;
	return b;
}

// White key at index i: a plastic cap over a slightly narrower, slightly set back body.
// The cap is rounded on top but its bottom edge is sharp (cut by a plane), and the step is tiny:
// a rounded bottom edge left a deep groove that read as a black underline on every key.
float whiteKey(vec3 p, float i, float swing, float z1){
	int note = whiteNote(int(i));
	if(note < PIANO_LOW || note > PIANO_HIGH){
		return 1e9;
	}
	float dip = keyTravel[note] * swing;
	vec3 q = vec3(p.x - i - 0.5, p.y + dip, p.z);
	float capExtra = 2.0 * WHITE_ROUND;
	float dcap = sdBox(q - vec3(0.0, 0.5 * (capExtra - KEYTOP_H) - capExtra, 0.5 * z1),
					   vec3(0.5 - 0.5 * WHITE_GAP, 0.5 * (KEYTOP_H + capExtra), 0.5 * z1), WHITE_ROUND);
	dcap = max(dcap, -(q.y + KEYTOP_H));
	float bodyH = WHITE_DEPTH - KEYTOP_H;
	float dbody = sdBox(q - vec3(0.0, -KEYTOP_H - 0.5 * bodyH, 0.5 * z1 + 0.5 * KEYTOP_INSET),
						vec3(0.5 - 0.5 * WHITE_GAP - KEYTOP_INSET, 0.5 * bodyH, 0.5 * z1 - 0.5 * KEYTOP_INSET), 0.005);
	return min(dcap, dbody);
}

// Cheek block of the case between x0 and x1: higher than the black keys, standing out at the front,
// going down to the floor and back under the canvas.
float cheek(vec3 p, float x0, float x1, float z1){
	float bottom = -WHITE_DEPTH - 0.6;
	float z0 = -CHEEK_FRONT;
	return sdBox(vec3(p.x - 0.5 * (x0 + x1), p.y - 0.5 * (CHEEK_H + bottom), p.z - 0.5 * (z0 + z1)),
				 vec3(0.5 * (x1 - x0), 0.5 * (CHEEK_H - bottom), 0.5 * (z1 - z0)), CHEEK_ROUND);
}

float sdEllipsoid(vec3 p, vec3 r){
	float k0 = length(p / r);
	float k1 = length(p / (r * r));
	return k0 * (k0 - 1.0) / k1;
}

float sdCapsule(vec3 p, vec3 a, vec3 b, float r){
	vec3 pa = p - a, ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
	return length(pa - ba * h) - r;
}

// Mascot: a 3D model baked into a distance field (a thin shell around its surface), standing on the left cheek
// block or hovering anywhere around it. The model can be turned, leaned and scaled.
uniform bool mascotEnabled = false;
uniform sampler3D mascotDist; ///< Distance to the surface, mapped to mascotDistRange.
uniform sampler3D mascotColors; ///< rgb albedo, a: glow.
uniform vec3 mascotHalf; ///< Half size of the baked box, in model space.
uniform vec3 mascotCenter; ///< Center of the box, from the point where the model stands.
uniform vec2 mascotDistRange = vec2(0.0, 1.0);
uniform vec3 mascotOffset = vec3(0.0); ///< From the middle of the left cheek block.
uniform mat3 mascotToModel = mat3(1.0); ///< Rotation and scale from the scene to the model.
uniform float mascotSize = 1.0;
uniform float mascotTop = 1.0; ///< Highest point of the mascot and its bulb, above its base.
uniform float mascotGlow = 1.0; ///< Glow of its emissive parts.
uniform float mascotLight = 0.0; ///< Light it casts on the keyboard.
uniform vec3 mascotLightOffset = vec3(0.0, 1.0, -0.5); ///< Where its light stands, from its base (scene space).
uniform float mascotBulb = 0.0; ///< Radius of a small glowing bulb at the light, 0: none.
uniform vec3 mascotColor = vec3(0.72, 0.86, 1.0); ///< Glow and light color.

vec3 mascotLocal(vec3 p){
	return mascotToModel * (p - mascotBase);
}

vec3 mascotUVW(vec3 q){
	return (q - mascotCenter + mascotHalf) / (2.0 * mascotHalf);
}

// Returns (model, bulb).
vec2 mascotMap(vec3 p){
	float bulb = mascotBulb > 0.0 ? length(p - mascotBase - mascotLightOffset) - mascotBulb : 1e9;
	vec3 q = mascotLocal(p);
	float box = sdBox(q - mascotCenter, mascotHalf, 0.0);
	if(box > 0.02){
		return vec2((box + 0.01) * mascotSize, bulb);
	}
	float d = mix(mascotDistRange.x, mascotDistRange.y, texture(mascotDist, mascotUVW(q)).r);
	return vec2((d + max(box, 0.0)) * mascotSize, bulb);
}

// Returns distance, material, note.
vec3 map(vec3 p){
	// A pressed key pivots far behind: it goes deeper at the front.
	float swing = keyDip * (0.2 + 0.8 * clamp((zBack - p.z) / zBack, 0.0, 1.0));
	float z1 = zBack + BEHIND;

	// White keys: repetition along x. Pressed keys sit lower than their neighbours, so the closest surface
	// can belong to the next key: evaluate the key under the point and its nearest neighbour.
	float wi = clamp(floor(p.x), 0.0, notesCount - 1.0);
	float wn = clamp(wi + (fract(p.x) > 0.5 ? 1.0 : -1.0), 0.0, notesCount - 1.0);
	float dw = whiteKey(p, wi, swing, z1);
	float dwn = whiteKey(p, wn, swing, z1);
	if(dwn < dw){
		dw = dwn;
		wi = wn;
	}
	int whiteN = whiteNote(int(wi));

	// Black keys: only the closest boundary between two white keys can hold one.
	float db = 1e9;
	float center;
	int blackN = blackAfter(int(floor(p.x + 0.5)) - 1, center);
	if(blackN < PIANO_LOW || blackN > PIANO_HIGH){
		blackN = -1;
	}
	if(blackN >= 0){
		float y = p.y + keyTravel[blackN] * swing;
		// Narrower at the top.
		float taper = 1.0 - 0.18 * clamp(y / BLACK_H, 0.0, 1.0);
		float hz = 0.5 * (z1 - zBlack);
		db = sdBox(vec3((p.x - center) / taper, y - 0.5 * BLACK_H, p.z - zBlack - hz),
				   vec3(0.45 * minorsWidth, 0.5 * BLACK_H + 0.122, hz), BLACK_ROUND) * taper;
		// Slanted front face.
		float cut = (p.z - zBlack) * 0.9 - (y - BLACK_H) - 0.03;
		db = max(db, -cut * 0.7);
	}

	// Felt strip lying on the white keys against the canvas, black keys go through it.
	float df = sdBox(vec3(p.x - 0.5 * (pianoLeft + pianoRight), p.y - 0.5 * FELT_H, p.z - zBack + 0.5 * FELT_D),
					 vec3(0.5 * (pianoRight - pianoLeft), 0.5 * FELT_H, 0.5 * FELT_D), 0.02);
	// Cheek blocks, only if the visualizer range leaves room for them.
	float dc = 1e9;
	if(pianoLeft > 0.5){
		dc = cheek(p, -2.0, pianoLeft - CHEEK_GAP, z1);
	}
	if(pianoRight < notesCount - 0.5){
		dc = min(dc, cheek(p, pianoRight + CHEEK_GAP, notesCount + 2.0, z1));
	}
	// Bed under the keys and key slip in front of them.
	float dbed = p.y + WHITE_DEPTH + 0.081;
	float dslip = sdBox(vec3(p.x - 0.5 * notesCount, p.y + SLIP_DROP + 0.61, p.z + 0.5 * SLIP_D + 0.061),
						vec3(notesCount, 0.61, 0.5 * SLIP_D), 0.061);
	dbed = min(dbed, dslip);

	float dm = 1e9;
	float dbulb = 1e9;
	if(mascotOn && !shadowIgnoresMascot){
		vec2 m = mascotMap(p);
		dm = m.x;
		dbulb = m.y;
	}

	float d = min(min(min(dw, db), min(df, dbed)), min(dc, min(dm, dbulb)));
	int mat = d == dw ? MAT_WHITE : (d == db ? MAT_BLACK : (d == df ? MAT_FELT : (d == dc ? MAT_CASE :
			  (d == dm ? MAT_MASCOT : (d == dbulb ? MAT_BULB : MAT_OTHER)))));
	int note = mat == MAT_WHITE ? whiteN : (mat == MAT_BLACK ? blackN : -1);
	// The canvas is a wall standing on the white keys: everything behind it is cut.
	d = max(d, p.z - zBack);
	return vec3(d, float(mat), float(note));
}

float dist(vec3 p){
	return map(p).x;
}

vec3 normalAt(vec3 p, float e){
	return normalize(vec3(
		dist(p + vec3(e, 0.0, 0.0)) - dist(p - vec3(e, 0.0, 0.0)),
		dist(p + vec3(0.0, e, 0.0)) - dist(p - vec3(0.0, e, 0.0)),
		dist(p + vec3(0.0, 0.0, e)) - dist(p - vec3(0.0, 0.0, e))));
}

float softShadow(vec3 p, vec3 dir, float maxT, float k, float jitter){
	// Low lights send rays almost parallel to the key tops: the start is pushed off the surface,
	// and steps are long enough to cross the keyboard, so that far keys do not flicker.
	float res = 1.0;
	float t = 0.06 + 0.02 * jitter;
	for(int i = 0; i < 64 && t < maxT; ++i){
		vec3 q = p + dir * t;
		// Nothing above the scene can cast a shadow: a ray going up past it is lit.
		if(q.y > sceneTop && dir.y >= 0.0){
			break;
		}
		float d = dist(q);
		res = min(res, k * d / t);
		// Already fully in shadow: no need to go further.
		if(res < 0.004){
			break;
		}
		t += clamp(d, 0.02, 1.0);
	}
	return clamp(res, 0.0, 1.0);
}

vec3 shade(vec3 n, vec3 v, vec3 l, vec3 albedo, float gloss, float spec, vec3 color, float amount){
	float ndl = max(dot(n, l), 0.0);
	float sp = pow(max(dot(n, normalize(l + v)), 0.0), gloss) * spec;
	return (albedo * ndl + sp) * color * amount;
}

void main(){
	vec2 screenSize = 1.0 / inverseScreenSize;
	float keyWidthPx = screenSize.x / notesCount;
	float lineY = keyboardHeight * screenSize.y;

	// Camera, scale 1:1 at the depth of the line.
	float phi = radians(tilt);
	vec3 forward = vec3(0.0, -sin(phi), cos(phi));
	vec3 up = vec3(0.0, cos(phi), sin(phi));
	float frontDelta = FRONT_FRACTION * keyboardHeight * screenSize.y / keyWidthPx;
	zBack = frontDelta * perspective / (perspective * sin(phi) + frontDelta * cos(phi));
	zBlack = zBack * (1.0 - BLACK_LENGTH);
	pianoLeft = 0.0;
	pianoRight = notesCount;
	bool leftFound = false;
	for(int i = 0; i < int(notesCount); ++i){
		int note = whiteNote(i);
		if(!leftFound && note >= PIANO_LOW){
			pianoLeft = float(i);
			leftFound = true;
		}
		if(note <= PIANO_HIGH){
			pianoRight = float(i + 1);
		}
	}
	// On the middle of the left cheek block, halfway towards the canvas.
	mascotOn = mascotEnabled;
	mascotBase = vec3(pianoLeft - CHEEK_GAP - 1.45, CHEEK_H - 0.01, 0.5 * zBack) + mascotOffset;
	bool cheeks = pianoLeft > 0.5 || pianoRight < notesCount - 0.5;
	sceneTop = cheeks ? CHEEK_H + 0.05 : 0.6;
	if(mascotOn){
		sceneTop = max(sceneTop, CHEEK_H + mascotTop + 0.05);
	}
	vec3 eye = vec3(0.5 * notesCount, 0.0, zBack) - forward * perspective;

	vec2 uv = (gl_FragCoord.xy - vec2(0.5 * screenSize.x, lineY)) / (keyWidthPx * perspective);
	vec3 dir = normalize(vec3(uv.x, 0.0, 0.0) + uv.y * up + forward);
	if(dir.y >= 0.0){
		discard;
	}

	if(useBake){
		// Where the ray meets the top of the white keys: close enough to know which keys this pixel shows,
		// the ranges have margins for what stands above (black keys) or below (pressed keys).
		float xOnKeys = eye.x + dir.x * (eye.y / -dir.y);
		bool dynamic = false;
		for(int i = 0; i < dynamicRangeCount; ++i){
			if(xOnKeys >= dynamicRanges[i].x && xOnKeys <= dynamicRanges[i].y){
				dynamic = true;
				break;
			}
		}
		if(!dynamic){
			vec4 baked = texelFetch(bakeTex, ivec2(gl_FragCoord.xy), 0);
			if(baked.a < 0.5){
				discard;
			}
			fragColor = baked;
			return;
		}
	}

	// March from just above the keyboard.
	float t = (eye.y - sceneTop) / -dir.y;
	float tEnd = t + 50.0;
	bool escaped = false;
	vec3 hit;
	for(int i = 0; i < 160; ++i){
		hit = map(eye + dir * t);
		if(hit.x < 0.001){
			break;
		}
		t += hit.x * 0.85;
		if(t > tEnd){
			escaped = true;
			break;
		}
	}
	// Rays still marching in a narrow gap did reach the keyboard; only those flying away are transparent.
	if(escaped){
		discard;
	}
	vec3 p = eye + dir * t;
	hit = map(p);
	int mat = int(hit.y + 0.5);
	// The mascot is sampled from a grid: a step about half its cell smooths the bumps between samples.
	vec3 n = normalAt(p, mat == MAT_MASCOT ? 0.012 * mascotSize : 0.006);
	vec3 v = -dir;

	float ao = 1.0;
	const float aoSteps[5] = float[](0.04, 0.10, 0.18, 0.30, 0.49);
	for(int i = 0; i < 5; ++i){
		float k = aoSteps[i];
		ao -= (k - min(dist(p + n * k), k)) / k * pow(0.5, float(i)) * 0.55;
	}
	ao = clamp(ao, 0.0, 1.0);

	// Materials.
	vec3 albedo = vec3(0.012, 0.010, 0.010); // Bed and key slip: dark lacquer.
	float gloss = 6.0;
	float spec = 0.02;
	if(mat == MAT_WHITE){
		float back = clamp((p.z - zBack + 2.44) / 2.44, 0.0, 1.0);
		// Inside a gap, a side wall faces its neighbour a few tenths of a millimeter away: almost no light
		// gets there. When the neighbour is pressed, the wall is exposed and lit like the rest of the key.
		// Probe the free space in front of the surface to tell the two apart.
		float sideness = clamp((abs(n.x) - 0.35) / 0.5, 0.0, 1.0);
		float open = clamp(dist(p + n * 0.12) / 0.1, 0.0, 1.0);
		float gapShade = mix(1.0, mix(0.05, 1.0, open), sideness);
		albedo = 0.88 * keyColor * (1.0 - 0.3 * back) * gapShade;
		// Satin plastic: a soft sheen everywhere, including the rounded edges.
		gloss = 36.0;
		spec = 0.28 * gapShade;
	} else if(mat == MAT_BLACK){
		albedo = 0.5 * edgeColor;
		gloss = 70.0;
		spec = 1.0;
	} else if(mat == MAT_FELT){
		albedo = feltColor;
	} else if(mat == MAT_MASCOT){
		albedo = texture(mascotColors, mascotUVW(mascotLocal(p))).rgb;
		gloss = 40.0;
		spec = 0.4;
	} else if(mat == MAT_BULB){
		albedo = vec3(0.02);
		gloss = 60.0;
		spec = 0.5;
	} else if(mat == MAT_CASE){
		// Black lacquer of the case.
		albedo = vec3(0.03);
		gloss = 40.0;
		spec = 1.0;
	} else if(p.z < 0.0){
		gloss = 50.0;
		spec = 0.6;
	}

	float jitter = fract(sin(dot(gl_FragCoord.xy, vec2(12.9898, 78.233))) * 43758.5453);
	vec3 po = p + n * 0.03;

	// Ambient light takes the color of the bulb: the whole scene is bathed in its warmth.
	vec3 color = albedo * 0.085 * backLightColor * ambientLight * ao;
	vec3 fillDir = normalize(vec3(0.0, 0.25, -1.0));
	color += shade(n, v, fillDir, albedo, gloss, spec * 0.2, vec3(0.30, 0.31, 0.34), fillLight * ao);

	// Back light: one warm bulb on the felt strip, a bit higher than the note lights,
	// so that some of its light reaches the whole keyboard. Black keys cast long shadows away from it.
	if(backLight > 0.0){
		vec3 lp = vec3(backLightPosition * notesCount, backLightHeight, zBack - FELT_D - 0.061);
		vec3 l = lp - p;
		float dl = length(l);
		l /= dl;
		float fall = backLight / (1.0 + pow(dl / max(backLightRadius, 0.01), 2.0));
		float sh = softShadow(po, l, dl, 6.0, jitter);
		color += shade(n, v, l, albedo, gloss, spec, backLightColor, fall * sh * (0.4 + 0.6 * ao));
	}

	// Light of the mascot, on the cheek block and the lowest keys.
	if(mascotOn && mascotLight > 0.0 && mat != MAT_BULB){
		vec3 lp = mascotBase + mascotLightOffset;
		vec3 l = lp - p;
		float dl = length(l);
		l /= dl;
		float fall = mascotLight / (1.0 + pow(dl / 1.6, 2.0));
		// A hidden light (in the heart of a flower) is too close to the model: it would blow it out to white.
		// A visible bulb lights it fully.
		fall *= mat == MAT_MASCOT && mascotBulb == 0.0 ? 0.15 : 1.0;
		// Fades out towards the middle of the keyboard: it is the light of the lowest octaves.
		fall *= clamp((12.0 - dl) / 4.0, 0.0, 1.0);
		if(fall * max(dot(n, l), 0.0) > 0.002 || fall * spec > 0.002){
			shadowIgnoresMascot = true;
			float sh = softShadow(po, l, dl, 6.0, jitter);
			shadowIgnoresMascot = false;
			color += shade(n, v, l, albedo, gloss, spec, mascotColor, fall * sh * (0.4 + 0.6 * ao));
		}
	}

	// Light from the point where each played note touches the keyboard.
	// The list of lit notes is built once per frame on the CPU: scanning all the keys here, in every pixel,
	// cost most of the frame even when nothing was played.
	float reach = 4.0 * noteLightRadius; // Beyond, the light of a note does not show; keep in sync with Renderer.
	for(int i = 0; i < noteLightCount; ++i){
		float x = noteLights[i].x;
		if(abs(x - p.x) > reach){
			continue;
		}
		vec3 lightColor = noteLightColors[i];
		// The bottom of the note is a short segment, not a point: two samples for softer penumbras.
		for(int s = 0; s < 2; ++s){
			vec3 lp = vec3(x, noteLights[i].y + float(s) * 3.0 * 0.142, zBack - noteLights[i].z);
			vec3 l = lp - p;
			float dl = length(l);
			l /= dl;
			float fall = noteLight * (s == 0 ? 0.7 : 0.3) / (1.0 + pow(dl / noteLightRadius, 2.0));
			fall *= clamp((reach - dl) / (2.0 * noteLightRadius), 0.0, 1.0);
			// Facing away, or too faint to show: the shadow ray is the expensive part, skip it.
			if(fall * max(dot(n, l), 0.0) <= 0.002 && fall * spec <= 0.002){
				continue;
			}
			float sh = softShadow(po, l, dl, 5.0, jitter);
			color += shade(n, v, l, albedo, gloss, spec * 1.3, lightColor, fall * sh * (0.4 + 0.6 * ao));
		}
	}

	// Emissive parts of the mascot, and its bulb.
	if(mat == MAT_MASCOT){
		color += mascotColor * mascotGlow * texture(mascotColors, mascotUVW(mascotLocal(p))).a;
	} else if(mat == MAT_BULB){
		color += mascotColor * 3.0;
	}

	// Lacquered case: at grazing angles it mirrors the warm room, which outlines its rounded edges.
	if(mat == MAT_CASE){
		float fresnel = pow(1.0 - max(dot(n, v), 0.0), 4.0);
		color += (0.04 + 0.5 * fresnel) * backLightColor * ambientLight * ao;
	}

	// Soft tone mapping, so that light does not clip.
	color = (1.0 - exp(-color * 1.15)) / (1.0 - exp(-1.15));
	fragColor = vec4(color, 1.0);
}
