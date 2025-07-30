#pragma once
#include "IManager.h"
#include <vector>
#include <algorithm>

class Stage;
class Camera;
class GameObject;

class GameObjectManager : public IManager {
public:
    /*--------------------------------------------------
     *  Singleton accessor
     *--------------------------------------------------*/
    static GameObjectManager& Instance() {
        static GameObjectManager instance;
        return instance;
    }

    /*--------------------------------------------------
     *  Object life‑cycle
     *--------------------------------------------------*/
    template<typename... Args>
    GameObject* Instantiate(Args&&... args) {
        GameObject* obj = new GameObject(std::forward<Args>(args)...);
        pendingCreate.push_back(obj);
        return obj;
    }



    /**
     * Mark an object to be destroyed at the end of the current Update()
     * call.  This prevents iterator invalidation while we are traversing
     * the gameObjects vector.
     */
    void Destroy(GameObject* obj);

    /*--------------------------------------------------
     *  Frame update
     *--------------------------------------------------*/
    void Update() override;

    /*--------------------------------------------------
     *  Camera / Stage accessors
     *--------------------------------------------------*/
    Camera* GetCamera() const { return camera; }
    void    SetStage(Stage* stage) { this->stage = stage; }
    void    SetCamera(Camera* camera) { this->camera = camera; }

    /*--------------------------------------------------
     *  Non‑copyable / non‑movable
     *--------------------------------------------------*/
    GameObjectManager(const GameObjectManager&) = delete;
    void operator=(const GameObjectManager&) = delete;

private:
    GameObjectManager() = default;
    ~GameObjectManager();

    /*--------------------------------------------------
     *  Helpers
     *--------------------------------------------------*/
    void drawCoordinateAxes();
    void reallyDestroy(GameObject* obj);   // actual deletion logic

private:
    std::vector<GameObject*> gameObjects;      // active objects
    std::vector<GameObject*> pendingCreate;
    std::vector<GameObject*> pendingDestroy;   // objects waiting to die

    Camera* camera = nullptr;                // owned elsewhere
    Stage* stage = nullptr;                // owned elsewhere

};