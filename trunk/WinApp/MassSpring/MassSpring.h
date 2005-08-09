#pragma once

#include "resource.h"

#define SCREEN_WIDTH 800								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 600								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel


void DeInit();
void Init();
void RenderScene(void);
void InitializeOpenGL(int Width, int Height);
void SizeOpenGLScreen(int Width, int Height);
bool SetupPixelFormat(HDC hdc);
void RenderScene();
void UpdateScene(float dt);
void BuildScene();
