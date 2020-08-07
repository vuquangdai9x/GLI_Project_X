#pragma once
#include "Camera.h"
#include "Model3D.h"
#include "Material.h"
#include "Texture.h"

class GameObject {
private:
	int m_id;
public:
	GameObject(int id);
	int GetId();
	void Init();
	void Init(Vector3 position, Vector3 rotation, Vector3 scale, int iModelId, int iMaterialId, int iMainTextureId=-1);
	void Update(float deltaTime);
	void Render(Camera * mainCamera);
	void OnDestroy();
	
	// transform
private:
	Vector3 m_position, m_rotation, m_scale;
	Matrix m_WVP;
	Matrix m_transformMat, m_T, m_R, m_S;
	void UpdateRotationMatrix();
	void UpdateTranslateMatrix();
	void UpdateScaleMatrix();

	Model3D* m_model;
	Material* m_material;
	Texture* m_mainTexture;
	bool m_canRender = false;
public:
	void Rotate(Vector3 deltaRotation);
	void SetRotation(Vector3 rotation);
	Vector3 & GetRotation();
	void MoveByWorldAxis(Vector3 deltaMove);
	void SetPosition(Vector3 position);
	Vector3 & GetPosition();
	void SetScale(Vector3 scale);
	Vector3 & GetScale();
};