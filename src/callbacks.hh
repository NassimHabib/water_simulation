#pragma once

void callbackIdle();
void callbackDisplay();
void callbackReshape(int width, int height);

void callbackKeyboard(unsigned char key, int x, int y);
void callbackKeyboardUp(unsigned char key, int x, int y);

void callbackSpecial(int key, int x, int y);
void callbackSpecialUp(int key, int x, int y);

void callbackMouseWheel(int wheel, int direction, int x, int y);