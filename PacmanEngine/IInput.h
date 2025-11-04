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

	virtual bool isQuitRequested() const = 0;

	virtual bool isKeyDown(int key) const = 0;
	virtual bool isPressed(int key) const = 0;
	virtual bool isLeftMouseDown() const = 0;
	virtual bool isLeftMousePressed() const = 0;
	virtual bool isRightMouseDown() const = 0;
	virtual bool isRightMousePressed() const = 0;
	virtual int getMouseX() const = 0;
	virtual int getMouseY() const = 0;
	virtual bool isPressedTwice(int button) const = 0;

	virtual void setKeyDown(int key) = 0;
	virtual void setKeyUp(int key) = 0;
	virtual void setMouseX(int lx) = 0;
	virtual void setMouseY(int ly) = 0;

	// Functions are mouse input, including getting and setting current position and mouse button presses.
	virtual void setLeftMouse(MouseState state) = 0;
	virtual void setRightMouse(MouseState state) = 0;
	virtual void setMousePosition(int lx, int ly) = 0;

	virtual void update(float dt) = 0;

};
