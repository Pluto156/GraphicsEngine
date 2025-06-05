#pragma once
#include "IManager.h"
#include "vector"

class Stage;
class Camera;
class GameObject;
class GameObjectManager : public IManager {
public:
    static GameObjectManager& Instance() {
        static GameObjectManager instance;
        return instance;
    }

    GameObject* Instantiate(GameObject* obj) {
        gameObjects.push_back(obj);
        return obj;
    }

    void Destroy(GameObject* obj) {
        auto it = std::remove(gameObjects.begin(), gameObjects.end(), obj);
        gameObjects.erase(it, gameObjects.end());
    }

    void Update() override;

    Camera* GetCamera() const {
        return camera;
    }

    void SetStage(Stage* stage)  {
        this->stage = stage;
    }

    void SetCamera(Camera* camera) {
        this->camera = camera;
    }

    GameObjectManager(const GameObjectManager&) = delete;
    void operator=(const GameObjectManager&) = delete;

private:
    GameObjectManager() {}

    ~GameObjectManager() {
        for (auto gameObject : gameObjects) {
            delete gameObject;
        }
    }

    std::vector<GameObject*> gameObjects; 

    //TODO 不应在这里
    Camera* camera;
    Stage* stage;
    void drawCoordinateAxes();
};
