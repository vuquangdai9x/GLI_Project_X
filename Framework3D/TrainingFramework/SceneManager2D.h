#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include <vector>
#include "Camera2D.h"

class SceneManager2D
{
protected:
	SceneManager2D() {}
	~SceneManager2D();
	SceneManager2D(const SceneManager2D&) {}
	SceneManager2D& operator =(const SceneManager2D&) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == nullptr)
			ms_pInstance = new SceneManager2D;
	}
	static SceneManager2D* GetInstance()
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
	static SceneManager2D* ms_pInstance;

private:
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
	void SetMainCamera(Camera2D * camera);
	Camera2D& GetMainCamera();
	std::vector<Sprite*>& GetListObject();
};