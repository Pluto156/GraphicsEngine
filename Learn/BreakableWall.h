#pragma once
#pragma once
class BreakableWall : public GameScript
{
    REGISTER_COMPONENT_DERIVED(BreakableWall, ComponentType::BreakableWall, GameScript)
public:
    void OnTriggerEnter(Collider* other);


    void Start()override;
    void Update()override;

    void ReSet();
    void AddHealth(int addnum);


private:

    int FullHealth = 10;
    int CurHealth = 10;
    bool isDead = false;

};

