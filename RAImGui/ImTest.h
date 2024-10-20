#pragma once

bool ImTest_SetUp();
bool ImTest_MainLoop();
void ImTest_CleanUp();
bool ImTest_ShouldClose();
int ImTest_Full(int, char**);

void ImTest_UpdateCheck();
void ImTest_DrawOnGameUI();
void ImTest_TryToDraw();