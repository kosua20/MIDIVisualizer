#version 330

layout(location = 0) in vec2 v;

uniform float keyboardHeight = 0.25;
uniform float notesCount; // (maxNoteMajor - minNoteMajor + 1)
uniform vec2 inverseScreenSize;
uniform float topMargin = 2.0; ///< In white key widths: room for what rises above the line (black keys, the mascot).

void main(){
	// Input are in -0.5,0.5.
	// The quad covers the keyboard band, plus a margin above it:
	// black keys and the felt strip rise above the line where the notes land.
	float keyWidthPx = 1.0 / (inverseScreenSize.x * notesCount);
	float margin = topMargin * keyWidthPx * inverseScreenSize.y;
	float top = 2.0 * keyboardHeight + margin;
	gl_Position = vec4(v.x * 2.0, (v.y + 0.5) * top - 1.0, 0.0, 1.0);
}
