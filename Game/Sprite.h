#pragma once
#include "stdafx.h"
#include "../Framework3D/TrainingFramework/Material2D.h"
#include "../Framework3D/TrainingFramework/Camera2D.h"
#include "../Framework3D/TrainingFramework/Model3D.h"
#include "../Framework3D/TrainingFramework/Texture.h"

class Sprite{
public:
	Sprite(int id);
	int GetId();
	void Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId);
	void Init(Sprite& sprite);
	//void SetSpriteAnimation(int iTextureId, int iNumOfCols, int iNumOfRows, float interval);
	virtual void Update(float deltaTime);
	virtual void Render(Camera2D* mainCamera);
	virtual void OnDestroy();
	
protected:
	int m_id;
	bool m_isActive;
	Vector3 m_position;
	Vector2 m_originSize;
	Vector2 m_scale;
	float m_rotation;
	Vector4 m_color;

	Matrix m_WVP;
	Matrix m_transformMat, m_T, m_R, m_S;
	void UpdateRotationMatrix();
	void UpdateTranslateMatrix();
	void UpdateScaleMatrix();

	Model3D* m_model;
	Material2D* m_material2d;
	Texture* m_mainTexture;
	bool m_canRender = false;
public:
	void SetRotation(float rotation);
	float GetRotation();
	void SetPosition(Vector3 position);
	Vector3 GetPosition();
	void SetScale(Vector2 scale);
	Vector2 GetScale();
	void SetColor(unsigned int hexColor, float alpha);
	void SetColor(Vector4 color);
	Vector4 GetColor();
	void SetActiveSprite(bool value);
	bool CheckIsActiveSprite();
};
int glhProjectf(float objx, float objy, float objz, Matrix modelview, Matrix projection, int* viewport, float* windowCoordinate);
int glhUnProjectf(float winx, float winy, float winz, Matrix modelview, Matrix projection, int* viewport, float* objectCoordinate);