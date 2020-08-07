#include "stdafx.h"
#include "Vertex.h"

//void Vertex::SetColorFromHex(unsigned int hexColor, float alpha) {
//	color.x = ((hexColor & 0xFF0000) >> 16) / (float)255;
//	color.y = ((hexColor & 0x00FF00) >> 8) / (float)255;
//	color.z = ((hexColor & 0x0000FF) >> 0) / (float)255;
//	color.w = alpha;
//}
void Vertex::SetPosition(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}
//void Vertex::SetColor(float r, float g, float b, float a) {
//	color.x = r;
//	color.y = g;
//	color.z = b;
//	color.w = a;
//}
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