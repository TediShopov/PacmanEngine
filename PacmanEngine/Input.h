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
	virtual bool isKeyDown(int key) const override;
	virtual bool isPressed(int key) const override;
	virtual bool isLeftMouseDown() const override;
	virtual bool isLeftMousePressed() const override;
	virtual bool isRightMouseDown() const override;
	virtual bool isRightMousePressed() const override;
	virtual int getMouseX() const override;
	virtual int getMouseY() const override;
	virtual bool isPressedTwice(int button) const override;
	virtual bool isQuitRequested() const override;

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
	mutable Mouse mouse;
};
