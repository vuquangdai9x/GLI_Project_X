#pragma once
// particle for circle emitter
struct Particle {
	float angle;
	float radiusOffset;
	float radiusMultiplier;
	//float velocityOffset;
	float sizeOffset;
	Vector4 colorOffset;
};

struct ParticleBox {
	Vector2 initPos;
	Vector2 velocityOffset;
	float velocityMul;
	float timeOffset;
	float lifeTime;
	float size;
	Vector4 colorOffset;
};