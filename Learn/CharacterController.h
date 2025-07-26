#pragma once
class CharacterController:public GameScript
{
public:

    void Update()override;

    int cnt = 0;

    void BindUpKey(KeyCode key);
    void BindDownKey(KeyCode key);
    void BindLeftKey(KeyCode key);
    void BindRightKey(KeyCode key);
    void BindFireKey(KeyCode key);

private:
    KeyCode UpKey, DownKey, LeftKey, RightKey, FireKey;
};

