#include "stdafx.h"
#include "GameObjectManager.h"
void GameObjectManager::Update() {
    for (auto obj : gameObjects) {
        if (obj) obj->Update();
    }
}