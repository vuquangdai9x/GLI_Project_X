#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include <vector>
#include "Camera.h"

class SceneManager
{
protected:
	SceneManager() {}
	~SceneManager();
	SceneManager(const SceneManager &) {}
	SceneManager& operator =(const SceneManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == nullptr)
			ms_pInstance = new SceneManager;
	}
	static SceneManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = nullptr;
		}
	}

protected:
	static SceneManager * ms_pInstance;

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
	void RemoveObject(GameObject *object);
	void Init(char * dataSceneFile);
	void SetMainCamera(Camera * camera);
	Camera& GetMainCamera();
	std::vector<GameObject*>& GetListObject();
};