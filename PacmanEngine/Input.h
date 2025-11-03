#pragma once
#include <vector>
#include <iostream>
#include "IInput.h"

// Input class
// Class stores the current state on keyboard and mouse input.
// Store which keys are pressed or released, mouse position and mouse button presses.
class Input : public IInput
{
public:
	int lastKeyPressed;
	int lastKeyReleased;
	bool doublePress;
	bool isQuitRequsted = false;

	Input();

	virtual void pumpEvents(IWindow& win) override;

	//--- GETTERS ---
	virtual bool isKeyDown(int key) override;
	virtual bool isPressed(int key) override;
	virtual bool isLeftMouseDown() override;
	virtual bool isLeftMousePressed() override;
	virtual bool isRightMouseDown() override;
	virtual bool isRightMousePressed() override;
	virtual int getMouseX() override;
	virtual int getMouseY() override;
	virtual bool isPressedTwice(int button) override;
	virtual bool isQuitRequested() override;

	//--- SETTERS ---
	virtual void setKeyDown(int key) override;
	virtual void setKeyUp(int key) override;
	virtual void setMouseX(int lx) override;
	virtual void setMouseY(int ly) override;

	// Functions are mouse input, including getting and setting current position and mouse button presses.
	virtual void setLeftMouse(MouseState state) override;
	virtual void setRightMouse(MouseState state) override;
	virtual void setMousePosition(int lx, int ly) override;

	virtual void update(float dt) override;
private:
	// Array of booleans representing keys (pressed = true, released = false)
	bool keys[256]{ false };
	std::vector<int> pressed;
	Mouse mouse;
};
