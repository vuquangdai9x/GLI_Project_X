#include "DifficultyCurve.h"
#include "stdafx.h"

float DifficultyCurve::Linear(float height)
{
	return height*1.5;
}
DifficultyCurveFnPtr DifficultyCurve::GetFunctionPtr(char curveName[]) {
	if (strcmp("Linear", curveName) == 0) {
		return DifficultyCurve::Linear;
	}
	else {
		printf("[ERR] CurveFunction::GetFunctionPtr: Invalid curveNamed : %s\n", curveName);
		return NULL;
	}
}
