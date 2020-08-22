#pragma once
#pragma warning(disable: 4996)
#include "stdafx.h"
#include "Sprite.h"
#include <vector>
#include "../Framework3D/TrainingFramework/Camera2D.h"
#include "CombatController.h"
#include "Singleton.h"

#include "UIComponent.h"

#define PLAY_OBJECT 0
#define MENU_OBJECT 1

class SceneManager2D
{

public:
	SceneManager2D() {}
	~SceneManager2D();

private:
	Vector2 return2D;
	Vector3 return3D;
	std::vector<Sprite*> m_listObject,m_menuObject;
	Camera2D* m_mainCamera,*m_menuCamera;

	CombatController* m_combatController = NULL;
public:
	float m_time = 0;

	void Update(float frameTime, int listObjet = PLAY_OBJECT);
	void Render(int listObjet = PLAY_OBJECT);
	void AddObject(Sprite* object,int listObjet = PLAY_OBJECT);
	Sprite& GetObjectByID(int id);
	void RemoveObject(Sprite* object);
	bool LoadScene(char* dataSceneFile);
	bool LoadMenuScene(char* dataSceneFile);
	void SetMainCamera(Camera2D * camera, int listObjet = PLAY_OBJECT);
	Camera2D& GetMainCamera(int listObjet = PLAY_OBJECT);
	std::vector<Sprite*>& GetListObject();
	Vector2& get2Dpos(float x, float y, float z, int listObjet = PLAY_OBJECT);
	Vector3& get3Dpos(float x, float y, int listObjet = PLAY_OBJECT);
};