#include "stdafx.h"
bool isInitStage = false;

void myDisplay(void);
void myTimerFunc(int val);
void SetRC();
void myReshape(int w, int h);
void myTimerFunc(int val)
{
    myDisplay();
    glutTimerFunc(1, myTimerFunc, 0);
}

void SetRC()
{
    glEnable(GL_DEPTH_TEST);  // 深度测试，避免遮挡错误
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    LightManager::Instance().InitLighting(); // 初始化光照
}

void InitStage()
{
    if (!isInitStage)
    {
        GameObject* Light0 = GameObjectManager::Instance().Instantiate("Light0", CVector3(0, 20, 20));
        GameObject* camera = GameObjectManager::Instance().Instantiate("Camera",CVector3(0,20,20));
        GameObject* stage = GameObjectManager::Instance().Instantiate("stage", CVector3());
        GameObject* Floor = GameObjectManager::Instance().Instantiate("Floor", CVector3(0, -1 - 0.05, 0));
        GameObject* A = GameObjectManager::Instance().Instantiate("A", CVector3(0, 6, 0));
        GameObject* B1 = GameObjectManager::Instance().Instantiate("B1", CVector3(-(9 + 8 * 0.05) / 2 + 0.5, 0, 0));
        GameObject* B2 = GameObjectManager::Instance().Instantiate("B2", CVector3(-(15 + 14 * 0.05) / 2 + 0.5, 1, -4 - 4 * 0.05));
        GameObject* C = GameObjectManager::Instance().Instantiate("C", CVector3(0, 2.5 + 5.5, 5));
        GameObject* E29 = GameObjectManager::Instance().Instantiate("E29", CVector3(-(29 + 28 * 0.05) / 2 + 0.5, 20, 0));
        GameObject* E25 = GameObjectManager::Instance().Instantiate("E25", CVector3(-(25 + 24 * 0.05) / 2 + 0.5, 20, 0));
        GameObject* Car = GameObjectManager::Instance().Instantiate("Car", CVector3(0, 0.5 + 2.5+5, 0));
        GameObject* CarHealthBar = GameObjectManager::Instance().Instantiate("CarCarHealthBar");
        GameObject* Car2HealthBar = GameObjectManager::Instance().Instantiate("Car2CarHealthBar");
        GameObject* Car2 = GameObjectManager::Instance().Instantiate("Car2", CVector3(3, 0.5 + 2.5 + 5, 0));
        GameObject* HealthPack = GameObjectManager::Instance().Instantiate("HealthPack", CVector3(0, 0.5 + 2.5 + 5, 0));
        GameObject* HealthPackPart1 = GameObjectManager::Instance().Instantiate("HealthPackPart1", CVector3(0, 0.5 + 2.5 + 5, 0));
        GameObject* HealthPackPart2 = GameObjectManager::Instance().Instantiate("HealthPackPart2", CVector3(0, 0.5 + 2.5 + 5, 0));

        Stage* StageCom = stage->AddComponent<Stage>();
        Camera* CameraCom = camera->AddComponent<Camera>();
        Light* light = Light0->AddComponent<Light>();
        Light0->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Sphere, 0.1f, 32, 16);
        Light0->AddComponent<MeshRenderer>();
        light->color = CVector3(1.0f,1,1);   // 白色光
        light->intensity = 0.1f;
        light->range = 15.0f;
        Light0->AddComponent<LightController>()->Light = light;

        Car->AddComponent<MeshFilter>()->LoadModel("E:/sourcecode/GraphicsEngine/Resource/Model/T 90.obj");
        Car->AddComponent<MeshRenderer>("E:/sourcecode/GraphicsEngine/Resource/Model/T 90A.png");
        Car->transform->isShowLocalAxis = true;
        Car->transform->SetLocalScale(CVector3(0.2, 0.2, 0.2));
        CarHealthBar->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
        auto renderer = CarHealthBar->AddComponent<MeshRenderer>();
        Material mat;
        mat.diffuseColor = CVector3(1, 0,0);
        mat.specularColor = CVector3(0,0,0);
        mat.shininess = 0;
        renderer->SetMaterial(mat);

        Car->AddChild(CarHealthBar);
        CarHealthBar->transform->localPosition = CVector3(0, 1, 0);
        CarHealthBar->transform->localScale = CVector3(1.5, 0.05, 0.05);

        Car2->AddComponent<MeshFilter>()->LoadModel("E:/sourcecode/GraphicsEngine/Resource/Model/T 90.obj");
        Car2->AddComponent<MeshRenderer>("E:/sourcecode/GraphicsEngine/Resource/Model/T 90A.png");
        Car2->transform->isShowLocalAxis = true;
        Car2->transform->SetLocalScale(CVector3(0.2, 0.2, 0.2));
        Car2->AddChild(Car2HealthBar);
        Car2HealthBar->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
        renderer = Car2HealthBar->AddComponent<MeshRenderer>();
        renderer->SetMaterial(mat);
        Car2HealthBar->transform->localPosition = CVector3(0, 1, 0);
        Car2HealthBar->transform->localScale = CVector3(1.5, 0.05, 0.05);

        HealthPack->AddChild(HealthPackPart1);
        HealthPack->AddChild(HealthPackPart2);
        HealthPackPart1->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
        renderer = HealthPackPart1->AddComponent<MeshRenderer>();
        renderer->SetMaterial(mat);
        HealthPackPart1->transform->localPosition = CVector3(0, 0, 0);
        HealthPackPart1->transform->localScale = CVector3(0.5, 1.5, 0.5);
        HealthPackPart2->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
        renderer = HealthPackPart2->AddComponent<MeshRenderer>();
        renderer->SetMaterial(mat);
        HealthPackPart2->transform->localPosition = CVector3(0, 0, 0);
        HealthPackPart2->transform->SetEulerAngles(CEuler(0, 90, 0));
        HealthPackPart2->transform->localScale = CVector3(0.5, 1.5, 0.5);



        Floor->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
        Floor->AddComponent<MeshRenderer>();

        GameObjectManager::Instance().SetCamera(CameraCom);
        GameObjectManager::Instance().SetStage(StageCom);
        StageCom->Car = Car;
        StageCom->Car2 = Car2;

        stage->transform->isShowLocalAxis = true;
        StageCom->camera = GameObjectManager::Instance().GetCamera();
        //stage.AddArea(&Floor);
        stage->AddChild(A);
        stage->AddChild(B1);
        stage->AddChild(B2);
        stage->AddChild(C);
        stage->AddChild(E29);
        stage->AddChild(E25);
        stage->AddChild(Car);
        stage->AddChild(Car2);

        stage->transform->position=CVector3(0, 0, 0);
        CVector3 AreaPos = Floor->transform->position;

        AreaPos = B1->transform->position;
        float b1Spacing = 0.05f; 
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                float xPos = AreaPos.x + j * (1 + b1Spacing); 
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b1Spacing); 

                GameObject* t = GameObjectManager::Instance().Instantiate("B1_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), CVector3(xPos, yPos, zPos));
                t->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
                auto renderer = t->AddComponent<MeshRenderer>();
                t->transform->localScale = CVector3(1,5,1);
                Material mat;
                mat.diffuseColor = CVector3(0.117f, 0.506f, 0.69f);
                mat.specularColor = CVector3(1.0f, 1.0f, 1.0f);
                mat.shininess = 64.0f;
                renderer->SetMaterial(mat);
                B1->AddChild(t);
            }
        }

        float radius = 10.0f; 
        A->transform->position.z= radius-10;
        float anglestep = 0.15; 
        AreaPos = A->transform->position;

        for (int i = 0; i < 11; ++i) {
            float angle = i * anglestep;  
            float xPos = AreaPos.x - radius * sin(angle); 
            float yPos = AreaPos.y;  
            float zPos = AreaPos.z - radius * cos(angle); 



            GameObject* t1 = GameObjectManager::Instance().Instantiate("A_" + std::to_string(i + 1), CVector3(xPos, yPos, zPos),CMatrix4::CreateRotationMatrix(angle * 180 / M_PI, CVector3::Up()), CEuler(angle * 180 / M_PI, 0, 0));
            t1->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
            auto renderer1 = t1->AddComponent<MeshRenderer>();
            t1->transform->localScale = CVector3(1, 12, 1);
            Material mat1;
            mat1.diffuseColor = CVector3(0.117f, 0.506f, 0.69f);
            mat1.specularColor = CVector3(1.0f, 1.0f, 1.0f);
            mat1.shininess = 64.0f;
            renderer1->SetMaterial(mat1);

            GameObject* t2 = GameObjectManager::Instance().Instantiate("A_" + std::to_string(i + 1), CVector3(-xPos, yPos, zPos), CMatrix4::CreateRotationMatrix(-angle * 180 / M_PI, CVector3::Up()), CEuler(-angle * 180 / M_PI, 0, 0));
            t2->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
            auto renderer2 = t2->AddComponent<MeshRenderer>();
            t2->transform->localScale = CVector3(1, 12, 1);
            Material mat2;
            mat2.diffuseColor = CVector3(0.117f, 0.506f, 0.69f);
            mat2.specularColor = CVector3(1.0f, 1.0f, 1.0f);
            mat2.shininess = 64.0f;
            renderer2->SetMaterial(mat2);

            A->AddChild(t1);
            A->AddChild(t2);
            
        }


        AreaPos = B2->transform->position;
        float b2spacing = 0.05f;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 15; ++j) {
                float xPos = AreaPos.x + j * (1 + b2spacing); 
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b2spacing); 

                GameObject* t = GameObjectManager::Instance().Instantiate("B2_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), CVector3(xPos, yPos, zPos));
                t->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
                auto renderer = t->AddComponent<MeshRenderer>();
                t->transform->localScale = CVector3(1, 3, 1);
                Material mat;
                mat.diffuseColor = CVector3(0.08, 0.3, 0.6);
                mat.specularColor = CVector3(1.0f, 1.0f, 1.0f);
                mat.shininess = 64.0f;
                renderer->SetMaterial(mat);
                B2->AddChild(t);
            }
        }

        AreaPos = C->transform->position;
        float cspacing = 0.05f;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                float xPos = AreaPos.x + (i * 2 - 1) * (10 + cspacing); 
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + j * (2 + cspacing); 
                GameObject* t = GameObjectManager::Instance().Instantiate("C_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), CVector3(xPos, yPos, zPos));
                t->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Cube, 1.0f, 32, 16);
                auto renderer = t->AddComponent<MeshRenderer>();
                t->transform->localScale = CVector3(3, 11, 0.5f);
                Material mat;
                mat.diffuseColor = CVector3(1, 0.89, 0);
                mat.specularColor = CVector3(1.0f, 1.0f, 1.0f);
                mat.shininess = 64.0f;
                renderer->SetMaterial(mat);
                C->AddChild(t);
            }
        }

        auto rigidBody1 = B1->AddComponent<RigidBody>();
        //rigidBody1->rigidBodyPrimitive->SetMass(100);
        auto planeCollider = B1->AddComponent<PlaneCollider>();
        planeCollider->mFriction = 10;
        planeCollider->mBounciness = 0;

        planeCollider->mCollider->mLocalNormal = CVector3(0,1,0);
        planeCollider->mCollider->mDistance = 2.5;

        planeCollider->SynchronizeData();
        planeCollider->mCollider->rigidBodyPrimitive = rigidBody1->rigidBodyPrimitive;
        rigidBody1->rigidBodyPrimitive->mCollisionVolume = planeCollider->mCollider;
        rigidBody1->rigidBodyPrimitive->SetInertiaTensor(planeCollider->mCollider->GetInertiaTensor(rigidBody1->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(B1);
        B1->transform->UpdateColliderTransform();


        auto rigidBody2 = Car->AddComponent<RigidBody>();
        rigidBody2->rigidBodyPrimitive->SetMass(100);
        auto BoxCollider2 = Car->AddComponent<BoxCollider>(CVector3(0.5, 0.5, 0.5));
        BoxCollider2->mFriction = 10;
        BoxCollider2->mBounciness = 0.5;
        BoxCollider2->SynchronizeData();

        BoxCollider2->mCollider->rigidBodyPrimitive = rigidBody2->rigidBodyPrimitive;
        rigidBody2->rigidBodyPrimitive->mCollisionVolume = BoxCollider2->mCollider;
        rigidBody2->rigidBodyPrimitive->SetInertiaTensor(BoxCollider2->mCollider->GetInertiaTensor(rigidBody2->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Car);
        rigidBody2->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
        Car->transform->UpdateColliderTransform();
        rigidBody2->rigidBodyPrimitive->SetGameObjectName("Car");

        auto rigidBody3 = Car2->AddComponent<RigidBody>();
        rigidBody3->rigidBodyPrimitive->SetMass(1000);
        auto BoxCollider3 = Car2->AddComponent<BoxCollider>(CVector3(0.5, 0.5, 0.5));
        BoxCollider3->mFriction = 10;
        BoxCollider3->mBounciness = 0.5;
        BoxCollider3->SynchronizeData();
        BoxCollider3->mCollider->SetLayer(PhysicsLit::Layer::ENEMY);
        BoxCollider3->mCollider->rigidBodyPrimitive = rigidBody3->rigidBodyPrimitive;
        rigidBody3->rigidBodyPrimitive->mCollisionVolume = BoxCollider3->mCollider;
        rigidBody3->rigidBodyPrimitive->SetInertiaTensor(BoxCollider3->mCollider->GetInertiaTensor(rigidBody3->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Car2);
        rigidBody3->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
        Car2->transform->UpdateColliderTransform();
        rigidBody3->rigidBodyPrimitive->SetGameObjectName("Car2");


        auto CarController =  Car->AddComponent<CharacterController>();
        CarController->BindUpKey(KeyCode::T);
        CarController->BindDownKey(KeyCode::G);
        CarController->BindLeftKey(KeyCode::F);
        CarController->BindRightKey(KeyCode::H);
        CarController->BindFireKey(KeyCode::J);

        auto Car2Controller = Car2->AddComponent<CharacterController>();
        Car2Controller->BindUpKey(KeyCode::UpArrow);
        Car2Controller->BindDownKey(KeyCode::DownArrow);
        Car2Controller->BindLeftKey(KeyCode::LeftArrow);
        Car2Controller->BindRightKey(KeyCode::RightArrow);
        Car2Controller->BindFireKey(KeyCode::Alpha1);

        Car->AddComponent<Unit>()->HealthBar = CarHealthBar->transform;
        Car2->AddComponent<Unit>()->HealthBar = Car2HealthBar->transform;
        
        
        isInitStage = true;
    }

}

void myDisplay(void) {
    InitStage();
    TimeManager::Instance().Update(); 
    CountDownTimerManager::Instance().Update();
    InputManager::Instance().Update();
    GameObjectManager::Instance().Update();
    GameScriptManager::Instance().Update();
    PhysicsLit::PhysicsManager::Instance().Update();
    InputManager::Instance().LateUpdate();
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, GLfloat(w) / h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}







int main(int argc, char* argv[])
{
    Calculate();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Lighting Toggle Example");

    glutDisplayFunc(&myDisplay);
    glutTimerFunc(1, myTimerFunc, 0);
    glutReshapeFunc(&myReshape);

    // 注册键盘事件处理
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        InputManager::Instance().onKeyDown(key, x, y);
        });

    glutKeyboardUpFunc([](unsigned char key, int x, int y) {
        InputManager::Instance().onKeyUp(key, x, y);
        });

    glutSpecialFunc([](int key, int x, int y) {
        InputManager::Instance().onSpecialKeyDown(key, x, y);
        });

    glutSpecialUpFunc([](int key, int x, int y) {
        InputManager::Instance().onSpecialKeyUp(key, x, y);
        });

    glutMotionFunc([](int x, int y) {
        InputManager::Instance().onMouseMove(x, y);
        });

    glutPassiveMotionFunc([](int x, int y) {
        InputManager::Instance().onMouseMove(x, y);
        });
    glutMouseFunc([](int button, int state, int x, int y) {
        InputManager::Instance().onMouseButton(button, state, x, y);
        });


    SetRC(); // 初始化光照和其他渲染设置
    glutMainLoop();

    return 0;
}




