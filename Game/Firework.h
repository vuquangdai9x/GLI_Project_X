#pragma once
#include "Effect.h"
#include "CurveFunction.h"
#include <random>
#include <time.h>


class Firework : public Effect {
private:
	enum RENDER_SHAPE { CIRCLE, RECT, RECT_LINE, LINE };
	enum ANIMATION {LINEAR, EASE_OUT_QUINT, EASE_OUT_CIRC};
	struct Particle {
		RENDER_SHAPE m_shape;
		float m_initX,m_initY;
		float m_x, m_y;
		float m_startSize, m_endSize;
		float m_size;
		float m_startVx, m_startVy, m_endVx, m_endVy;
		float m_vx, m_vy;
		float m_delay;
		unsigned int m_uiColor;
	};
private:
	int m_iNumOfParticles;
	float m_lifeTime;
	float m_initPosXRange = 0, m_initPosYRange = 0, m_offsetX = 0, m_offsetY = 0;
	float m_startMinSpeed, m_startMaxSpeed, m_endSpeed;
	float m_startMinSize, m_startMaxSize, m_endSize;
	float m_startMinDirection, m_startMaxDirection;
	float m_startMinDelay, m_startMaxDelay;
	unsigned int m_uiStartColor, m_uiEndColor;
	RENDER_SHAPE m_shape;
	ANIMATION m_animationType;
	Particle * m_aParticle;

	float m_timeCounter;
private:
	 Firework(Vector2 position, int loopCount=1) :
		Effect(position, loopCount) {}

	 void Init() {
		 srand(time(NULL));
		 int iRandomShapeType;
		 float randomInitSpeed;
		 float randomInitDirection; //radian
		 for (int i = 0; i < m_iNumOfParticles; i++) {
			 Particle& p = m_aParticle[i];
			 p.m_initX = p.m_x = m_offsetX + m_position.x + (((float)rand() / (float)RAND_MAX)*m_initPosXRange - m_initPosXRange / 2);
			 p.m_initY = p.m_y = m_offsetY + m_position.y + (((float)rand() / (float)RAND_MAX)*m_initPosYRange - m_initPosYRange / 2);

			 p.m_shape = m_shape;

			 p.m_startSize = ((float)rand() / (float)RAND_MAX)*(m_startMaxSize - m_startMinSize) + m_startMinSize;
			 p.m_endSize = p.m_startSize * m_endSize;

			 randomInitDirection = ((float)rand() / (float)RAND_MAX)*(m_startMaxDirection - m_startMinDirection) + m_startMinDirection;
			 randomInitSpeed = ((float)rand() / (float)RAND_MAX)*(m_startMaxSpeed - m_startMinSpeed) + m_startMinSpeed;
			 p.m_startVx = randomInitSpeed * cosf(randomInitDirection);
			 p.m_endVx = p.m_startVx * m_endSpeed;
			 p.m_startVy = randomInitSpeed * sinf(randomInitDirection);
			 p.m_endVy = p.m_startVy * m_endSpeed;

			 p.m_delay = ((float)rand() / (float)RAND_MAX)*(m_startMaxDelay - m_startMinDelay) + m_startMinDelay;

			 p.m_uiColor = CurveFunction::GetColorFromGradient(m_uiStartColor,m_uiEndColor, ((float)rand() / (float)RAND_MAX));
		 }
		 m_timeCounter = 0;
	 }

	 
public:
	static Firework * CreateFirework(Vector2 position, int loopCount, int typeId=-1) {
		Firework * fw = new Firework(position, loopCount);
		switch (typeId) {
		case 0:
			fw->m_shape = RENDER_SHAPE::LINE;
			fw->m_animationType = ANIMATION::EASE_OUT_CIRC;
			fw->m_iNumOfParticles = 20;
			fw->m_lifeTime = 2;

			fw->m_initPosXRange = 0;
			fw->m_initPosYRange = 0;
			fw->m_startMinDelay = 0;
			fw->m_startMaxDelay = 0;

			fw->m_startMinSpeed = 200;
			fw->m_startMaxSpeed = 300;
			fw->m_endSpeed = 0.2 ;

			fw->m_startMinSize = 20;
			fw->m_startMaxSize = 30;
			fw->m_endSize = 0;

			fw->m_startMinDirection = 0;
			fw->m_startMaxDirection = 2*M_PI;

			fw->m_uiStartColor = 0xcc2b5e;
			fw->m_uiEndColor = 0x753a88;
			break;
		case -1:
		default: // circle break
			fw->m_shape = RENDER_SHAPE::CIRCLE;
			fw->m_animationType = ANIMATION::EASE_OUT_QUINT;
			fw->m_iNumOfParticles = 20;
			fw->m_lifeTime = 1;

			fw->m_initPosXRange = 5;
			fw->m_initPosYRange = 5;
			fw->m_startMinDelay = 0;
			fw->m_startMaxDelay = 0;

			fw->m_startMinSpeed = 150;
			fw->m_startMaxSpeed = 200;
			fw->m_endSpeed = 0.5;

			fw->m_startMinSize = 3;
			fw->m_startMaxSize = 10;
			fw->m_endSize = 0;

			fw->m_startMinDirection = 0;
			fw->m_startMaxDirection = 2 * M_PI;

			fw->m_uiStartColor = 0xeb3349;
			fw->m_uiEndColor = 0xf45c43;
			break;
		}
		fw->m_aParticle = new Particle[fw->m_iNumOfParticles];
		fw->Init();
		return fw;
	}
public:
	~Firework(){
		delete[] m_aParticle;
	}
	
	void Render() {
		Effect::Render();
		/*for (int i = 0; i < m_iNumOfParticles; i++) {
			VideoDriver::GetInstance()->SetColor(m_aParticle[i].m_uiColor);
			Particle& p = m_aParticle[i];
			if (p.m_delay > 0) continue;
			switch (m_aParticle[i].m_shape)
			{
			case RENDER_SHAPE::CIRCLE:
				VideoDriver::GetInstance()->DrawCircle(p.m_x, p.m_y, p.m_size);
				break;
			case RENDER_SHAPE::RECT:
				VideoDriver::GetInstance()->FillRect(p.m_x - p.m_size, p.m_y - p.m_size, p.m_size * 2, p.m_size * 2);
				break;
			case RENDER_SHAPE::RECT_LINE:
				VideoDriver::GetInstance()->DrawRect(p.m_x - p.m_size, p.m_y - p.m_size, p.m_size * 2, p.m_size * 2);
				break;
			case RENDER_SHAPE::LINE:
				float dx = p.m_initX - p.m_x;
				float dy = p.m_initY - p.m_y;
				float t = 1;
				if (sqrtf(dx*dx + dy*dy)>p.m_size)
					t = p.m_size / sqrtf(dx*dx + dy*dy);
				VideoDriver::GetInstance()->DrawLine(p.m_x + dx*t, p.m_y + dy*t, p.m_x, p.m_y);
				break;
			}
		}
		VideoDriver::GetInstance()->SetColor(DEFAULT_COLOR);*/
	}
	void Update(float deltaTime) {
		Effect::Update(deltaTime);
		m_timeCounter += deltaTime;
		if (m_timeCounter > m_lifeTime) {
			m_iLoopCount--;
			Init();
		}
		float t = m_timeCounter / m_lifeTime;
		float(*animation_fun)(float, float, float);
		switch (m_animationType)
		{
		case ANIMATION::EASE_OUT_CIRC:
			animation_fun = CurveFunction::EaseOutCirc;
			break;
		case ANIMATION::EASE_OUT_QUINT:
			animation_fun = CurveFunction::EaseOutQuint;
			break;
		case ANIMATION::LINEAR:
		default:
			animation_fun = CurveFunction::Linear;
			break;
		}

		for (int i = 0; i < m_iNumOfParticles; i++) {
			Particle& p = m_aParticle[i];
			if (p.m_delay > 0) {
				p.m_delay -= deltaTime;
			}
			else {
				p.m_size = animation_fun(p.m_startSize, p.m_endSize, t);
				p.m_vx = animation_fun(p.m_startVx, p.m_endVx, t);
				p.m_vy = animation_fun(p.m_startVy, p.m_endVy, t);

				p.m_x += p.m_vx * deltaTime;
				p.m_y += p.m_vy * deltaTime;
			}
		}
	}
};