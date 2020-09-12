//TrainingFramework.cpp : Defines the entry point for the console application.
#pragma warning(disable: 4996)

#include "../Framework3D/TrainingFramework/framework3d.h"
#include "../Utilities/utilities.h"
#include "Box2D/Box2D.h"
#include <conio.h>
#include <stddef.h>
#include "Singleton.h"
#include "ResourcesManager2D.h"
#include "SceneManager2D.h"
#include "State/GameStateManager.h"
#include "WorldManager.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "PostProcessingManager.h"
#include "DecorateObjectManager.h"

#include <stdlib.h> 
#include <time.h>

ESContext esContext;

int Init(ESContext* esContext)
{
	srand(time(NULL));
	Singleton<EffectManager>::CreateInstance();
	Singleton<SoundManager>::CreateInstance();
	Singleton<InputManager>::CreateInstance();
	Singleton<ResourceManager2D>::CreateInstance();
	Singleton<SceneManager2D>::CreateInstance();
	Singleton<WorldManager>::CreateInstance();
	Singleton<GameStateManager>::CreateInstance();
	Singleton<PostProcessingManager>::CreateInstance();
	Singleton<DecorateObjectManager>::CreateInstance();

	Singleton<PostProcessingManager>::GetInstance()->Load("Datas/post-processing.txt");
	Singleton<PostProcessingManager>::GetInstance()->SetMaterial(0);
	Singleton<DecorateObjectManager>::GetInstance()->Load("Datas/decorate-object.txt");
	
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Singleton<WorldManager>::GetInstance()->createRectagle(PLAYER, 1.0f, 0.0f, 1.0f, 1.0f);
	

	return 0;
}

void Draw(ESContext* esContext)
{
	//DWORD start, end;
	//start = GetTickCount();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Singleton<SceneManager2D>::GetInstance()->Render();
	//SceneManager::GetInstance()->Render();
	Singleton<GameStateManager>::GetInstance()->render();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	//end = GetTickCount();
	//DWORD deltaTime = end - start;
	//if(deltaTime!=0.0f)
	
	//if (deltaTime < 1000.0 / 60.0)
	//Sleep(1000 / 60.0 - deltaTime);
}

void Update(ESContext * esContext, float deltaTime)
{
	//printf("%f \n", deltaTime);
	Singleton<GameStateManager>::GetInstance()->update(deltaTime);
	//printf("%f \n", 1 / deltaTime);
}

void Key(ESContext * esContext, unsigned char key, bool bIsPressed)
{
	Singleton<InputManager>::GetInstance()->KeyPressed(key, bIsPressed);
	Singleton<GameStateManager>::GetInstance()->KeyPress();
}
void Mouse(ESContext* esContext, int typeOfService, int button, int x, int y) {
	Singleton<InputManager>::GetInstance()->MouseEvent(typeOfService, button, x, y);
	/*float xx = x, yy = y;
	printf("2D pos: %f %f \n", xx, yy);
	Vector3 pos3D;
	pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(xx, yy);
	printf("3D pos: %f %f \n", pos3D.x, pos3D.y);*/
}
void CleanUp() 
{

}

int _tmain(int argc, _TCHAR * argv[])
{
	
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

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
		esRegisterMouseFunc(&esContext, Mouse);
		esMainLoop(&esContext);
		
	}

	//releasing OpenGL resources
	CleanUp();

	Singleton<ResourceManager2D>::DestroyInstance();
	Singleton<SceneManager2D>::DestroyInstance();
	Singleton<InputManager>::DestroyInstance();
	Singleton<WorldManager>::DestroyInstance();
	Singleton<SoundManager>::DestroyInstance();
	Singleton<PostProcessingManager>::DestroyInstance();
	Singleton<EffectManager>::DestroyInstance();
	Singleton<DecorateObjectManager>::DestroyInstance();
	//identifying memory leaks
	//MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

