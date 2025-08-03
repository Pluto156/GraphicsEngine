#pragma once
class Unit : public GameScript
{
public:
    void OnTriggerEnter(Collider* other);


    void Start()override;
    void Update()override;
    Transform* HealthBar;

    void ReSet();
    void AddHealth(int addnum);
    void RefreshHealthBar();


private:

    int FullHealth = 10;
    int CurHealth = 10;
    bool isDead = false;

};

