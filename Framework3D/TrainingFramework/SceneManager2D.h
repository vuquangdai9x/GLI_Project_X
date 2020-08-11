#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include <vector>
#include "Camera2D.h"

class SceneManager2D
{

public:
	SceneManager2D() {}
	~SceneManager2D();

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