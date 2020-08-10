#include "stdafx.h"
#include "Vertex.h"

void Vertex::SetPosition(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Vertex::SetUV(float u, float v) {
	uv.x = u;
	uv.y = v;
}
void Vertex::SetNorm(float x, float y, float z) {
	norm.x = x;
	norm.y = y;
	norm.z = z;
}
void Vertex::SetBinorm(float x, float y, float z) {
	binorm.x = x;
	binorm.y = y;
	binorm.z = z;
}
void Vertex::SetTangent(float x, float y, float z) {
	tangent.x = x;
	tangent.y = y;
	tangent.z = z;
}

void Vertex::PrintInfo() {
	printf("Position: %f, %f, %f\n", pos.x, pos.y, pos.z);
	/*printf("Norm: %f, %f, %f\n", norm.x, norm.y, norm.z);
	printf("Binorm: %f, %f, %f\n", binorm.x, binorm.y, binorm.z);
	printf("Tangent: %f, %f, %f\n", tangent.x, tangent.y, tangent.z);
	printf("UV: %f, %f\n", uv.x, uv.y);*/
}