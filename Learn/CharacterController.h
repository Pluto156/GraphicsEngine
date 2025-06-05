#pragma once
class CharacterController:public Component, public InputManager::IInputControl
{
public:
	static ComponentType GetType();

    void processKeyboard(unsigned char key, int x, int y)override;
    void processSpecialKeys(int key, int x, int y) override;
    void processMouse(int button, int state, int x, int y)override;
    void processMouseMotion(int x, int y)override;
};

