#include "stdafx.h"
bool isInitStage = false;
void myDisplay(void);
void myTimerFunc(int val);
void SetRC();
void myReshape(int w, int h);
void processKeyboard(unsigned char key, int x, int y);  // 澶勭悊閿洏杈撳叆
void processSpecialKeys(int key, int x, int y);
void processMouse(int button, int state, int x, int y); // 澶勭悊榧犳爣杈撳叆
void processMouseMotion(int x, int y);
// 瀹氭椂鍣ㄥ洖璋冨嚱鏁?
void myTimerFunc(int val)
{
    myDisplay();
    glutTimerFunc(1, myTimerFunc, 0);
}

// 璁剧疆娓叉煋鐜
void SetRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);  // 鍚敤娣卞害娴嬭瘯锛岀‘淇濈墿浣撴寜姝ｇ‘鐨勯『搴忔覆鏌?
}



void InitStage()
{
    if (!isInitStage)
    {
        Camera* camera = new Camera();
        Stage* stage = new Stage("stage", 0, 0, 0);
        GameObject* Floor = new GameObject("Floor", CVector3(0, -1 - 0.05, 0));
        GameObject* A = new GameObject("A", CVector3(0, 6, 0));
        GameObject* B1 = new GameObject("B1", CVector3(-(9 + 8 * 0.05) / 2 + 0.5, 0, 0));
        GameObject* B2 = new GameObject("B2", CVector3(-(15 + 14 * 0.05) / 2 + 0.5, 1, -4 - 4 * 0.05));
        GameObject* C = new GameObject("C", CVector3(0, 2.5 + 5.5, 5));
        GameObject* E29 = new GameObject("E29", CVector3(-(29 + 28 * 0.05) / 2 + 0.5, 20, 0));
        GameObject* E25 = new GameObject("E25", CVector3(-(25 + 24 * 0.05) / 2 + 0.5, 20, 0));
        GameObject* Car = new GameObject("Car", CVector3(0, 0.5 + 2.5+5, 0));
        GameObject* Car2 = new GameObject("Car2", CVector3(3, 0.5 + 2.5 + 5, 0));

        GameObject* Sphere = ShapeFactory::CreateSphere("Sphere",1, CVector3(3, 0.5 + 2.5 + 7.5, 0));

        Car->AddComponent<MeshRenderer>("E:/sourcecode/GraphicsEngine/Resource/Model/T 90.obj","E:/sourcecode/GraphicsEngine/Resource/Model/T 90A.png");
        Car->transform->isShowLocalAxis = true;
        Car->transform->SetLocalScale(CVector3(0.2, 0.2, 0.2));
        GameObjectManager::Instance().SetCamera(camera);
        GameObjectManager::Instance().SetStage(stage);
        

        stage->transform->isShowLocalAxis = true;
        stage->camera = GameObjectManager::Instance().GetCamera();
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
        // 鍦版澘鍖?
        CVector3 AreaPos = Floor->transform->position;
        Floor->AddChild(ShapeFactory::CreateBox("floor",100, 0.1, 100, AreaPos, 0.5f, 0.5f, 0.5f));

        //B1.isShowLocalAxis = true;
        AreaPos = B1->transform->position;
        // B1鍖猴細10鎺?鍒?x5x1鐨勮垶鍙板潡锛岄棿璺?.05
        float b1Spacing = 0.05f; // 鏂瑰潡闂撮殧
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                float xPos = AreaPos.x + j * (1 + b1Spacing); // 璁＄畻x浣嶇疆
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b1Spacing); // 璁＄畻z浣嶇疆
                B1->AddChild(ShapeFactory::CreateBox("B1_"+ std::to_string((i+1))+'_' + std::to_string((j + 1)), 1, 5, 1, CVector3(xPos, yPos, zPos), 0.117f, 0.506f, 0.69f));
            }
        }

        // a鍖猴細22涓?x12x1鐨勬柟鍧楋紝寮у舰鎺掑垪锛岃绠楁瘡涓猙ox鐨勪綅缃?
        float radius = 10.0f; // 鍋囪寮у舰鐨勫崐寰?
        A->transform->position.z= radius-10;
        float anglestep = 0.15; // 灏?2涓潡骞冲潎鍒嗗竷鍦ㄥ姬褰笂
        AreaPos = A->transform->position;

        for (int i = 0; i < 11; ++i) {
            float angle = i * anglestep;  // 姣忎釜鏂瑰潡鐨勮搴?
            float xPos = AreaPos.x - radius * sin(angle);  // 璁＄畻x浣嶇疆
            float yPos = AreaPos.y;  // y浣嶇疆淇濇寔涓嶅彉
            float zPos = AreaPos.z - radius * cos(angle);  // 璁＄畻z浣嶇疆
            A->AddChild(ShapeFactory::CreateBox("A_" + std::to_string(i+1),1, 12, 1, CVector3(xPos, yPos, zPos),CMatrix4::CreateRotationMatrix(angle * 180 / M_PI,CVector3::Up()),CEuler(angle*180/M_PI,0,0), 0.117f, 0.506f, 0.69f));
            A->AddChild(ShapeFactory::CreateBox("A_" + std::to_string(2*(i+i)),1, 12, 1, CVector3(-xPos, yPos, zPos), CMatrix4::CreateRotationMatrix(-angle * 180 / M_PI, CVector3::Up()), CEuler(-angle * 180 / M_PI, 0, 0), 0.117f, 0.506f, 0.69f));
            
        }


        // b2鍖猴細4鎺?5鍒?x3x1鐨勮垶鍙板潡锛岄棿璺?.05
        AreaPos = B2->transform->position;
        float b2spacing = 0.05f; // 鏂瑰潡闂撮殧
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 15; ++j) {
                float xPos = AreaPos.x + j * (1 + b2spacing); // 璁＄畻x浣嶇疆
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b2spacing); // 璁＄畻z浣嶇疆
                B2->AddChild(ShapeFactory::CreateBox("B2_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 1, 3, 1, CVector3(xPos, yPos, zPos), 0.08, 0.3, 0.6));
            }
        }

        // c鍖猴細8鍧?x11x0.5鐨勮垶鍙板潡锛屽乏鍙冲悇4鍧楋紝闂磋窛0.05
        AreaPos = C->transform->position;
        float cspacing = 0.05f;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                float xPos = AreaPos.x + (i * 2 - 1) * (10 + cspacing); // 璁＄畻x浣嶇疆
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + j * (2 + cspacing); // 璁＄畻z浣嶇疆
                C->AddChild(ShapeFactory::CreateBox("C_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 3, 11, 0.5f, CVector3(xPos, yPos, zPos), 1, 0.89, 0));
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
        
        Box* Car2_Body = ShapeFactory::CreateBox("Car2_Body", 2, 1, 4, CVector3(), 0, 0, 0, false);

        Car2->AddChild(Car2_Body);
        Car2_Body->transform->SetLocalPosition(CVector3(0, 0, 0));


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

        auto rigidBody2 = Car->AddComponent<RigidBody>();
        rigidBody2->rigidBodyPrimitive->SetMass(100);
        auto BoxCollider2 = Car->AddComponent<BoxCollider>(CVector3(1, 0.5, 2));
        BoxCollider2->mFriction = 10;
        BoxCollider2->mBounciness = 0.5;
        BoxCollider2->SynchronizeData();

        BoxCollider2->mCollider->rigidBodyPrimitive = rigidBody2->rigidBodyPrimitive;
        rigidBody2->rigidBodyPrimitive->mCollisionVolume = BoxCollider2->mCollider;
        rigidBody2->rigidBodyPrimitive->SetInertiaTensor(BoxCollider2->mCollider->GetInertiaTensor(rigidBody2->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Car);
        rigidBody2->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));


        auto rigidBody3 = Car2->AddComponent<RigidBody>();
        rigidBody3->rigidBodyPrimitive->SetMass(1000);
        auto BoxCollider3 = Car2->AddComponent<BoxCollider>(CVector3(1, 0.5, 2));
        BoxCollider3->mFriction = 10;
        BoxCollider3->mBounciness = 0.5;
        BoxCollider3->SynchronizeData();

        BoxCollider3->mCollider->rigidBodyPrimitive = rigidBody3->rigidBodyPrimitive;
        rigidBody3->rigidBodyPrimitive->mCollisionVolume = BoxCollider3->mCollider;
        rigidBody3->rigidBodyPrimitive->SetInertiaTensor(BoxCollider3->mCollider->GetInertiaTensor(rigidBody3->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Car2);
        rigidBody3->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
        
        
        
        auto rigidBody4 = Sphere->AddComponent<RigidBody>();
        rigidBody4->rigidBodyPrimitive->SetMass(1000);
        auto sphereCollider = Sphere->GetComponent<SphereCollider>();
        sphereCollider->mFriction = 10;
        sphereCollider->mBounciness = 0.5;
        sphereCollider->SynchronizeData();

        sphereCollider->mCollider->rigidBodyPrimitive = rigidBody4->rigidBodyPrimitive;
        rigidBody4->rigidBodyPrimitive->mCollisionVolume = sphereCollider->mCollider;
        rigidBody4->rigidBodyPrimitive->SetInertiaTensor(sphereCollider->mCollider->GetInertiaTensor(rigidBody4->rigidBodyPrimitive->GetMass()));
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Sphere);
        rigidBody4->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
        
        GameScriptManager::Instance().registerScript(Car->AddComponent<CharacterController>());
        
        isInitStage = true;
    }

}

// 娓叉煋鍦烘櫙
void myDisplay(void) {
    InitStage();
    TimeManager::Instance().Update();  
    InputManager::Instance().Update();
    PhysicsLit::PhysicsManager::Instance().Update();
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

// 澶勭悊閿洏杈撳叆
void processKeyboard(unsigned char key, int x, int y)
{
    InputManager::Instance().enqueueKeyboardEvent(key, x, y);
    glutPostRedisplay();
}

// 澶勭悊閿洏杈撳叆
void processSpecialKeys(int key, int x, int y)
{
    InputManager::Instance().enqueueSpecialKeyEvent(key, x, y);
    glutPostRedisplay();
}

// 澶勭悊榧犳爣鎸夐敭杈撳叆
void processMouse(int button, int state, int x, int y)
{
    InputManager::Instance().enqueueMouseEvent(button, state, x, y);
}

// 澶勭悊榧犳爣绉诲姩
void processMouseMotion(int x, int y)
{
    InputManager::Instance().enqueueMouseMotionEvent(x, y);
}

// 涓荤▼搴?
int main(int argc, char* argv[])
{
    Calculate();
    // 鍒濆鍖?GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("av");

    // 璁剧疆 GLUT 鍥炶皟鍑芥暟
    glutDisplayFunc(&myDisplay);
    glutTimerFunc(1, myTimerFunc, 0);
    glutReshapeFunc(&myReshape);
    glutKeyboardFunc(&processKeyboard);   // 娉ㄥ唽閿洏杈撳叆
    glutSpecialFunc(&processSpecialKeys);
    glutMouseFunc(&processMouse);         // 娉ㄥ唽榧犳爣鎸夐敭杈撳叆
    glutMotionFunc(&processMouseMotion);  // 娉ㄥ唽榧犳爣鎷栧姩

    // 璁剧疆娓叉煋鐜
    SetRC();

    // 杩涘叆 GLUT 涓诲惊鐜?
    glutMainLoop();
    return 0;
}




