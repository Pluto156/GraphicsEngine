#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameObject.h"               // assumed definitions
#include "PhysicsManager.h"           // for PhysicsLit::PhysicsManager

// ------------------------- dtor -------------------------
GameObjectManager::~GameObjectManager()
{
    // Delete all remaining objects (if any)
    for (auto* obj : gameObjects) {
        delete obj;
    }
}

// ------------------------- Destroy ----------------------
// Mark for deferred destruction
void GameObjectManager::Destroy(GameObject* obj) {
    if (!obj || obj->IsPendingDestroy()) return;

    obj->SetIsPendingDestroy(true);

    // ✅ 首先递归销毁其所有子 GameObject
    auto transform = obj->GetComponent<Transform>();
    if (transform) {
        for (auto* child : transform->children) {
            if (child && child->gameObject) {
                Destroy(child->gameObject);
            }
        }
    }

    // 再加入待销毁列表
    if (std::find(pendingDestroy.begin(), pendingDestroy.end(), obj) == pendingDestroy.end())
        pendingDestroy.push_back(obj);
}
static float angle = 0.0f;

void GameObjectManager::Update()
{
    // ----- 1. Render preparation -------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    if (camera) camera->LookAt();

    angle += 0.01f;
    float radius = 5.0f;
    GLfloat light_pos[] = {
        radius * cos(angle),
        5.0f,
        radius * sin(angle),
        1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    if (stage) glGetDoublev(GL_MODELVIEW_MATRIX, stage->modelViewMatrix);
    drawCoordinateAxes();

    // ----- 2. Update all active objects -----------------------------------------
    for (auto* obj : gameObjects) {
        if (obj)
        {
            obj->Update();
        }
    }

    // ----- 3. Deferred creation --------------------------------------------------
    if (!pendingCreate.empty()) {
        gameObjects.insert(gameObjects.end(), pendingCreate.begin(), pendingCreate.end());
        pendingCreate.clear();
    }

    // ----- 4. Deferred destruction ----------------------------------------------
    for (auto* obj : pendingDestroy) {
        if (!obj) continue;
        reallyDestroy(obj);
    }
    pendingDestroy.clear();

    // ----- 5. Finish frame -------------------------------------------------------
    glPopMatrix();
    glutSwapBuffers();
}


// -------------------- reallyDestroy (private) -----------------------------------
void GameObjectManager::reallyDestroy(GameObject* obj)
{
    // Remove from active list
    auto it = std::remove(gameObjects.begin(), gameObjects.end(), obj);
    gameObjects.erase(it, gameObjects.end());
    // 父Transform需要销毁其引用
    if (obj->transform) {
        obj->transform->DetachFromParent();
    }
    // Inform physics manager (if any)
    if (obj && obj->GetComponent<RigidBody>() != nullptr)
    {
        PhysicsLit::PhysicsManager::Instance().RemoveGameObject(obj);
    }
    delete obj;
}

// ------------------- drawCoordinateAxes -----------------------------------------
void GameObjectManager::drawCoordinateAxes()
{
    // X axis (red)
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);
    glEnd();

    // Y axis (green)
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glEnd();

    // Z axis (blue)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();
}
