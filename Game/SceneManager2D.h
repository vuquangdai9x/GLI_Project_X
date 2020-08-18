#pragma once
#pragma warning(disable: 4996)
#include "stdafx.h"
#include "Sprite.h"
#include <vector>
#include "../Framework3D/TrainingFramework/Camera2D.h"

class SceneManager2D
{

public:
	SceneManager2D() {}
	~SceneManager2D();

private:
	Vector2 return2D;
	Vector3 return3D;
	std::vector<Sprite*> m_listObject;
	Camera2D* m_mainCamera;
public:
	float m_time = 0;

	void Update(float frameTime);
	void Render();
	void AddObject(Sprite* object);
	Sprite& GetObjectByID(int id);
	void RemoveObject(Sprite* object);
	bool LoadScene(char* dataSceneFile);
	bool LoadMenuScene(char* dataSceneFile);
	void SetMainCamera(Camera2D * camera);
	Camera2D& GetMainCamera();
	std::vector<Sprite*>& GetListObject();
	Vector2& get2Dpos(float x, float y, float z);
	Vector3& get3Dpos(float x, float y);
};