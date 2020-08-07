#include "stdafx.h"
#include <windows.h>
#include "esUtil.h"
#define MOUSE_CLICK     1
#define MOUSE_RELEASE   2
#define MOUSE_MOVE    3


static int mouseX = 0, mouseY = 0;


// Main window procedure
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;
	int mev, butnum;
	mev = butnum = 0;
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
		ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

		if (esContext && esContext->drawFunc)
			esContext->drawFunc(esContext);

		ValidateRect(esContext->hWnd, NULL);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	{
		ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

		if (esContext && esContext->keyFunc)
			esContext->keyFunc(esContext, (unsigned char)wParam, true);
	}
	break;

	case WM_KEYUP:
	{
		ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

		if (esContext && esContext->keyFunc)
			esContext->keyFunc(esContext, (unsigned char)wParam, false);
	}
	break;
	case WM_LBUTTONDOWN:
		butnum = 1;
		mev = MOUSE_CLICK;
		break;
	case WM_MBUTTONDOWN:
		butnum = 2;
		mev = MOUSE_CLICK;
		break;
	case WM_RBUTTONDOWN:
		butnum = 3;
		mev = MOUSE_CLICK;
		break;
	case WM_LBUTTONUP:
		butnum = 1;
		mev = MOUSE_RELEASE;
		break;
	case WM_MBUTTONUP:
		butnum = 2;
		mev = MOUSE_RELEASE;
		break;
	case WM_RBUTTONUP:
		butnum = 3;
		mev = MOUSE_RELEASE;
		break;
	case WM_MOUSEMOVE:
	{
		POINTS pz;
		pz = MAKEPOINTS(lParam);
		mouseX = pz.x;
		mouseY = pz.y;
	}
	mev = MOUSE_MOVE;
	break;
	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	if (mev)
	{
		POINT point;
		ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
		//GetCursorPos( &point );
		point.x = mouseX;
		point.y = mouseY;
		if (esContext && esContext->mouseFunc)
			esContext->mouseFunc(esContext, mev, butnum,
				(int)point.x, (int)point.y);
	}

	return lRet;
}

//      Create Win32 instance and window
GLboolean WinCreate(ESContext* esContext, const char* title)
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = "opengles2.0";

	if (!RegisterClass(&wndclass))
		return FALSE;

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->hWnd = CreateWindow(
		"opengles2.0",
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the 
	// ESWindowProc
	SetWindowLongPtr(esContext->hWnd, GWL_USERDATA, (LONG)(LONG_PTR)esContext);

	if (esContext->hWnd == NULL)
		return GL_FALSE;

	ShowWindow(esContext->hWnd, TRUE);

	return GL_TRUE;
}



//      Start main windows loop
void WinLoop(ESContext* esContext)
{
	MSG msg = { 0 };
	int done = 0;
	DWORD lastTime = GetTickCount();

	while (!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = (float)(curTime - lastTime) / 1000.0f;
		lastTime = curTime;

		if (gotMsg)
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			SendMessage(esContext->hWnd, WM_PAINT, 0, 0);

		// Call update function if registered
		if (esContext->updateFunc != NULL)
			esContext->updateFunc(esContext, deltaTime);
	}
}
