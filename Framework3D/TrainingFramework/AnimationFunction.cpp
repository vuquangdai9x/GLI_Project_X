#include "stdafx.h"
#include "AnimationFunction.h"

float AnimationFunction::EaseInOut(float start, float end, float value) {
	float t = (value < 0.5) ? (8 * value * value * value * value) : (1 - pow(-2 * value + 2, 4) / 2);
	return (t*(end - start) + start);
}
float AnimationFunction::Linear(float start, float end, float value) {
	return (start + value*(end - start));
}
float AnimationFunction::EaseOutCirc(float start, float end, float value) {
	float t = sqrt(1 - pow(value - 1, 2));
	return (t*(end - start) + start);
}

float AnimationFunction::GetColorFromGradient(unsigned int v1, unsigned int v2, float value) {
	unsigned int RedV1 = (v1 & 0xFFFFFF) >> 4,
		GreenV1 = (v1 & 0x00FFFF) >> 2,
		BlueV1 = v1 & 0x0000FF;
	unsigned int RedV2 = (v2 & 0xFFFFFF) >> 4,
		GreenV2 = (v2 & 0x00FFFF) >> 2,
		BlueV2 = v2 & 0x0000FF;
	unsigned int RedP = (RedV2 * value + RedV1 * (1 - value)),
		GreenP = (GreenV2 * value + GreenV1 * (1 - value)),
		BlueP = (BlueV2 * value + BlueV1 * (1 - value));
	return ((RedP << 4) & 0xFF0000) | ((GreenP << 2) & 0x00FF00) | (BlueP & 0x0000FF);
}

float AnimationFunction::EaseOutQuint(float start, float end, float value) {
	float t = 1 - pow(1 - value, 5);
	return (t*(end - start) + start);
}