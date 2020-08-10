#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include <vector>
#include "Camera.h"

class SceneManager
{
public:
	SceneManager() {}
	~SceneManager();
	
private:
	std::vector<GameObject*> m_listObject;
	Camera * m_mainCamera;
public:
	float m_fogStart, m_fogLength;
	Vector4 m_fogColor;
	float m_time = 0;

	void Update(float frameTime);
	void Render();
	void AddObject(GameObject *object);
	GameObject& GetObject(int id);
	bool LoadScene(char * dataSceneFile);
	void SetMainCamera(Camera * camera);
	Camera& GetMainCamera();
	std::vector<GameObject*>& GetListObject();
};