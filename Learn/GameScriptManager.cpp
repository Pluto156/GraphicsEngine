#include "stdafx.h"
#include "GameScriptManager.h"
void GameScriptManager::Update()
{
    TriggerCollisionEvents();
    std::swap(currentFrameContacts, lastFrameContacts);
    currentFrameContacts.clear();
}


