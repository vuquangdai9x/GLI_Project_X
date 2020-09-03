#pragma once
typedef float (*DifficultyCurveFnPtr)(float);
class DifficultyCurve {
public:
	static float Linear(float height);
	static DifficultyCurveFnPtr GetFunctionPtr(char name[]);
};