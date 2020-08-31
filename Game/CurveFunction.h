#pragma once
#include "stdafx.h"

// Class contain functions to get interpolated value between start & end. The input value must be in range [0,1]
// Usually use in animation
// Reference: https://easings.net/
// 
typedef float (*CurveFnPtr)(float,float,float);
class CurveFunction {
public:
	static float EaseInOut(float start, float end, float value);
	static float Linear(float start, float end, float value);
	static float EaseOutCirc(float start, float end, float value);
	static float EaseOutQuint(float start, float end, float value);
	static float GetColorFromGradient(unsigned int v1, unsigned int v2, float value);
	static CurveFnPtr GetFunctionPtr(char name[]);
};