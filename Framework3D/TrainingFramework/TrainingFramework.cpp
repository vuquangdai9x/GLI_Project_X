 //TrainingFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>
#include <stddef.h>
#include "Globals.h"
#include "InputManager.h"

#include "ResourceManager.h"
#include "SceneManager.h"

#include "ResourcesManager2D.h"
#include "SceneManager2D.h"

// detect memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
// ---


int Init ( ESContext *esContext )
{
	InputManager::CreateInstance();
	
	// 2D
	ResourceManager2D::CreateInstance();
	ResourceManager2D::GetInstance()->LoadResources("Datas/resources2d.txt");
	SceneManager2D::CreateInstance();
	if (!SceneManager2D::GetInstance()->LoadScene("Datas/scene2d.txt")) {
		printf("[ERR] Entry point: Failed to init scene");
		return false;
	}

	//// 3D
	//ResourceManager::CreateInstance();
	//ResourceManager::GetInstance()->LoadResources("Datas/resources.txt");
	//SceneManager::CreateInstance();
	//if (!SceneManager::GetInstance()->LoadScene("Datas/scene.txt")) {
	//	printf("[ERR] Entry point: Failed to init scene");
	//	return false;
	//}

	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	SceneManager2D::GetInstance()->Render();
	//SceneManager::GetInstance()->Render();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{
	InputManager::GetInstance()->Update(deltaTime);
	SceneManager2D::GetInstance()->Update(deltaTime);
	//SceneManager::GetInstance()->Update(deltaTime);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	InputManager::GetInstance()->KeyPressed(key, bIsPressed);
}

void CleanUp()
{
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	int iInitResult;
	if ((iInitResult = Init(&esContext)) != 0) {
		printf("Oop! Error happen\n");
	}
	else {
		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);

		esRegisterKeyFunc(&esContext, Key);

		esMainLoop(&esContext);
	}

	//releasing OpenGL resources
	CleanUp();

	ResourceManager2D::DestroyInstance();
	SceneManager2D::DestroyInstance();

	//ResourceManager::DestroyInstance();
	//SceneManager::DestroyInstance();

	InputManager::DestroyInstance();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	_CrtDumpMemoryLeaks();
}

