#pragma once
#include "IWindow.h"

class IInput
{
public:
	virtual ~IInput() = default;
	virtual void pumpEvents( IWindow&) = 0;

	enum class MouseState { UP, DOWN, PRESSED };

	// Mouse structure, store mouse relevant information
	struct Mouse
	{
		int x, y;
		MouseState left, right;
	};



	virtual bool isKeyDown(int key) = 0;
	virtual bool isPressed(int key) = 0;
	virtual bool isLeftMouseDown() = 0;
	virtual bool isLeftMousePressed() = 0;
	virtual bool isRightMouseDown() = 0;
	virtual bool isRightMousePressed() = 0;
	virtual int getMouseX() = 0;
	virtual int getMouseY() = 0;

	virtual void setKeyDown(int key) = 0;
	virtual void setKeyUp(int key) = 0;
	virtual void setMouseX(int lx) = 0;
	virtual void setMouseY(int ly) = 0;

	// Functions are mouse input, including getting and setting current position and mouse button presses.
	virtual void setLeftMouse(MouseState state) = 0;
	virtual void setRightMouse(MouseState state) = 0;
	virtual void setMousePosition(int lx, int ly) = 0;

	virtual void update(float dt) = 0;
	virtual bool isPressedTwice(int button) = 0;
	virtual bool isQuitRequested() = 0;

};
