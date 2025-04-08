#pragma once
#include "IManager.h"
#include "vector"
class GameObject;
class GameObjectManager :public IManager {
public:
    static GameObjectManager& Instance() {
        static GameObjectManager instance;  
        return instance;
    }

    void Register(GameObject* obj) {
        gameObjects.push_back(obj);
    }

    void Unregister(GameObject* obj) {
        auto it = std::remove(gameObjects.begin(), gameObjects.end(), obj);
        gameObjects.erase(it, gameObjects.end());
    }

    void Update()override;

    GameObjectManager(const GameObjectManager&) = delete;
    void operator=(const GameObjectManager&) = delete;

private:
    GameObjectManager() = default;  
    ~GameObjectManager() = default;

    std::vector<GameObject*> gameObjects; 
};

