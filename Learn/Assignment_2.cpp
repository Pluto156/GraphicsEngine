#include "stdafx.h"
bool isInitStage = false;
// 是否启用光照
bool gEnableLighting = true;
void myDisplay(void);
void myTimerFunc(int val);
void SetRC();
void myReshape(int w, int h);
void processKeyboard(unsigned char key, int x, int y);  
void processSpecialKeys(int key, int x, int y);
void processMouse(int button, int state, int x, int y); // 澶勭悊榧犳爣杈撳叆
void processMouseMotion(int x, int y);
// 瀹氭椂鍣ㄥ洖璋冨嚱鏁?
void myTimerFunc(int val)
{
    myDisplay();
    glutTimerFunc(1, myTimerFunc, 0);
}


// 初始化光照的函数
void InitLighting()
{
    if (!gEnableLighting) {
        glDisable(GL_LIGHTING);
        return;
    }

    glEnable(GL_LIGHTING);         // 启用光照系统
    glEnable(GL_LIGHT0);           // 启用第 0 号光源
    glEnable(GL_COLOR_MATERIAL);  // 启用颜色材质绑定

    // 让 glColor 影响材质的 ambient 和 diffuse
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // 光源参数
    GLfloat light_pos[] = { 5.0f, 5.0f, 5.0f, 1.0f };  // 点光源
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_DEPTH_TEST);  // 深度测试，避免遮挡错误
}
// 每帧都更新光源位置
void UpdateLightingPerFrame()
{
    if (!gEnableLighting) return;
    GLfloat light_pos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void SetRC()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    InitLighting(); // 初始化光照
}



void InitStage()
{
    if (!isInitStage)
    {
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
        GameObject* Car2 = GameObjectManager::Instance().Instantiate("Car2", CVector3(3, 0.5 + 2.5 + 5, 0));
        GameObject* Sphere = GameObjectManager::Instance().Instantiate("Sphere",CVector3(6, 20, 0));
        Stage* StageCom = stage->AddComponent<Stage>();
        Camera* CameraCom = camera->AddComponent<Camera>();

        Car->AddComponent<MeshFilter>()->LoadModel("E:/sourcecode/GraphicsEngine/Resource/Model/T 90.obj");
        Car->AddComponent<MeshRenderer>("E:/sourcecode/GraphicsEngine/Resource/Model/T 90A.png");
        Car->transform->isShowLocalAxis = true;
        Car->transform->SetLocalScale(CVector3(0.2, 0.2, 0.2));
        Car2->AddComponent<MeshFilter>()->LoadModel("E:/sourcecode/GraphicsEngine/Resource/Model/T 90.obj");
        Car2->AddComponent<MeshRenderer>("E:/sourcecode/GraphicsEngine/Resource/Model/T 90A.png");
        Car2->transform->isShowLocalAxis = true;
        Car2->transform->SetLocalScale(CVector3(0.2, 0.2, 0.2));
        Sphere->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Sphere, 1.0f, 32, 16);
        Sphere->AddComponent<MeshRenderer>();

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

        //// e29鍖猴細2鎺掞紝鍓嶆帓29涓紝鍚庢帓25涓紝澶у皬1x12x0.1锛岄棿璺?.05
        //AreaPos = E29->transform->position;
        //float e29spacing = 0.05f;
        //for (int j = 0; j < 29; ++j) {
        //    float xPos = AreaPos.x + j * (1 + e29spacing); // 璁＄畻x浣嶇疆
        //    float yPos = AreaPos.y;
        //    float zPos = AreaPos.z; // 璁＄畻z浣嶇疆
        //    E29->AddChild(ShapeFactory::CreateBox("E29_" + std::to_string((j + 1)),1, 12, 0.1f, CVector3(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        //}

        //// e25鍖猴細2鎺掞紝鍓嶆帓29涓紝鍚庢帓25涓紝澶у皬1x12x0.1锛岄棿璺?.05
        //AreaPos = E25->transform->position;
        //float e25spacing = 0.05f;
        //for (int j = 0; j < 25; ++j) {
        //    float xPos = AreaPos.x + j * (1 + e25spacing); // 璁＄畻x浣嶇疆
        //    float yPos = AreaPos.y;
        //    float zPos = AreaPos.z + 1 * (2 + e25spacing); // 璁＄畻z浣嶇疆
        //    E25->AddChild(ShapeFactory::CreateBox("E25_" + std::to_string((j + 1)),1, 12, 0.1f, CVector3(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        //}
        //灏忚溅
        //Box* Car_Body = ShapeFactory::CreateBox("Car_Body", 2, 1, 4, CVector3(), 0.9, 0.9, 0.9, false);
        //Box* Car_Wheel_1 = ShapeFactory::CreateBox("Car_Wheel_1", 1, 1, 1, CVector3(), 0, 0, 0, false);
        //Box* Car_Wheel_2 = ShapeFactory::CreateBox("Car_Wheel_2", 1, 1, 1, CVector3(), 0, 0, 0, false);
        //Box* Car_Wheel_3 = ShapeFactory::CreateBox("Car_Wheel_3", 1, 1, 1, CVector3(), 0, 0, 0, false);
        //Box* Car_Wheel_4 = ShapeFactory::CreateBox("Car_Wheel_4", 1, 1, 1, CVector3(), 0, 0, 0, false);
        //Car->AddChild(Car_Body);
        //Car->AddChild(Car_Wheel_1);
        //Car->AddChild(Car_Wheel_2);
        //Car->AddChild(Car_Wheel_3);
        //Car->AddChild(Car_Wheel_4);

        //Car_Body->transform->SetLocalPosition(CVector3(0, 1, 0));
        //Car_Wheel_1->transform->SetLocalPosition(CVector3(-0.5, 0, -1.5));
        //Car_Wheel_2->transform->SetLocalPosition(CVector3(0.5, 0, -1.5));
        //Car_Wheel_3->transform->SetLocalPosition(CVector3(-0.5, 0, 1.5));
        //Car_Wheel_4->transform->SetLocalPosition(CVector3(0.5, 0, 1.5));
        
        //Box* Car2_Body = ShapeFactory::CreateBox("Car2_Body", 2, 1, 4, CVector3(), 0, 0, 0, false);

        //Car2->AddChild(Car2_Body);
        //Car2_Body->transform->SetLocalPosition(CVector3(0, 0, 0));


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

        
        
        auto rigidBody4 = Sphere->AddComponent<RigidBody>();
        rigidBody4->rigidBodyPrimitive->SetMass(1000);
        auto sphereCollider = Sphere->AddComponent<SphereCollider>(1);
        sphereCollider->mFriction = 10;
        sphereCollider->mBounciness = 0.5;
        sphereCollider->SynchronizeData();

        sphereCollider->mCollider->rigidBodyPrimitive = rigidBody4->rigidBodyPrimitive;
        rigidBody4->rigidBodyPrimitive->mCollisionVolume = sphereCollider->mCollider;
        rigidBody4->rigidBodyPrimitive->SetInertiaTensor(sphereCollider->mCollider->GetInertiaTensor(rigidBody4->rigidBodyPrimitive->GetMass()));
        //rigidBody4->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
        Sphere->transform->UpdateColliderTransform();
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Sphere);


        Car->AddComponent<CharacterController>();
        Car2->AddComponent<Unit>();
        //Sphere->AddComponent<Bullet>();
        
        isInitStage = true;
    }

}

// 娓叉煋鍦烘櫙
void myDisplay(void) {
    InitStage();
    TimeManager::Instance().Update();  
    InputManager::Instance().Update();
    GameObjectManager::Instance().Update();
    GameScriptManager::Instance().Update();
    PhysicsLit::PhysicsManager::Instance().Update();
    //UpdateLightingPerFrame(); // 每帧刷新光照位置
    InputManager::Instance().LateUpdate();

}


// 澶勭悊绐楀彛澶у皬鍙樺寲
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, GLfloat(w) / h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void processKeyboard(unsigned char key, int x, int y)
{
    if (key == 'l' || key == 'L') {
        gEnableLighting = !gEnableLighting;
        InitLighting();  // 切换光照时重新初始化
    }
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}

void processMouse(int button, int state, int x, int y)
{
}

void processMouseMotion(int x, int y)
{
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
    glutKeyboardFunc(&processKeyboard);
    glutSpecialFunc(&processSpecialKeys);
    glutMouseFunc(&processMouse);
    glutMotionFunc(&processMouseMotion);

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




