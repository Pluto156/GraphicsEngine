#include "stdafx.h"
Camera camera;
Stage stage("stage",0,0,0);
Area Floor("Floor",0, -1 - 0.05, 0);
Area A("A", 0, 6, 0);
Area B1("B1", -(9 + 8 * 0.05) / 2 + 0.5, 0, 0);
Area B2("B2", -(15 + 14 * 0.05) / 2 + 0.5, 1, -4 - 4 * 0.05);
Area C("C", 0, 2.5 + 5.5, 5);
Area E29("E29", -(29 + 28 * 0.05) / 2 + 0.5, 20, 0);
Area E25("E25", -(25 + 24 * 0.05) / 2 + 0.5, 20, 0);
Area Car("Car", 0, 0.5+2.5, 0);

bool isInitStage = false;

void myDisplay(void);
void myTimerFunc(int val);
void SetRC();
void myReshape(int w, int h);
void processKeyboard(unsigned char key, int x, int y);  // 处理键盘输入
void processSpecialKeys(int key, int x, int y);
void processMouse(int button, int state, int x, int y); // 处理鼠标输入
void processMouseMotion(int x, int y);

// 定时器回调函数
void myTimerFunc(int val)
{
    InputManager::instance().Update();
    myDisplay();
    glutTimerFunc(1, myTimerFunc, 0);
}

// 设置渲染环境
void SetRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);  // 启用深度测试，确保物体按正确的顺序渲染
}

// 绘制世界坐标系坐标轴（XYZ）
void drawCoordinateAxes()
{
    // X轴（红色）
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);  // 红色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);  // X轴正方向
    glEnd();

    // Y轴（绿色）
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);  // 绿色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);  // Y轴正方向
    glEnd();

    // Z轴（蓝色）
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);  // 蓝色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);  // Z轴正方向
    glEnd();

    //glBegin(GL_LINES);
    //glColor3f(0.0f, 0.0f, 1.0f);  // 蓝色
    //glVertex3f(origin.x, origin.y, origin.z);
    //CVector end = origin +   direct* 100;
    //glVertex3f(end.x, end.y, end.z);  // Z轴正方向
    //glEnd();
}

void drawFloor()
{
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
}

void drawStage()
{
    if (!isInitStage)
    {
        stage.isShowLocalAxis = true;
        stage.camera = &camera;
        //stage.AddArea(&Floor);
        stage.AddArea(&A);
        stage.AddArea(&B1);
        stage.AddArea(&B2);
        stage.AddArea(&C);
        stage.AddArea(&E29);
        stage.AddArea(&E25);
        stage.AddArea(&Car);
        stage.position=CVector(0, 0, 0);
        // 地板区
        CVector AreaPos = Floor.position;
        Floor.AddShape(ShapeFactory::CreateBox("floor",100, 0.1, 100, AreaPos, 0.5f, 0.5f, 0.5f));

        //B1.isShowLocalAxis = true;
        AreaPos = B1.position;
        // B1区：10排9列1x5x1的舞台块，间距0.05
        float b1Spacing = 0.05f; // 方块间隔
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                float xPos = AreaPos.x + j * (1 + b1Spacing); // 计算x位置
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b1Spacing); // 计算z位置
                B1.AddShape(ShapeFactory::CreateBox("B1_"+ std::to_string((i+1))+'_' + std::to_string((j + 1)), 1, 5, 1, CVector(xPos, yPos, zPos), 0.117f, 0.506f, 0.69f));
            }
        }

        // a区：22个1x12x1的方块，弧形排列，计算每个box的位置
        float radius = 10.0f; // 假设弧形的半径
        A.position.z= radius-10;
        float anglestep = 0.15; // 将22个块平均分布在弧形上
        AreaPos = A.position;

        for (int i = 0; i < 11; ++i) {
            float angle = i * anglestep;  // 每个方块的角度
            float xPos = AreaPos.x - radius * sin(angle);  // 计算x位置
            float yPos = AreaPos.y;  // y位置保持不变
            float zPos = AreaPos.z - radius * cos(angle);  // 计算z位置
            A.AddShape(ShapeFactory::CreateBox("A_" + std::to_string(i+1),1, 12, 1, CVector(xPos, yPos, zPos),CMatrix::CreateRotationMatrix(angle * 180 / M_PI,CVector::Up()),CEuler(angle*180/M_PI,0,0), 0.117f, 0.506f, 0.69f));
            A.AddShape(ShapeFactory::CreateBox("A_" + std::to_string(2*(i+i)),1, 12, 1, CVector(-xPos, yPos, zPos), CMatrix::CreateRotationMatrix(-angle * 180 / M_PI, CVector::Up()), CEuler(-angle * 180 / M_PI, 0, 0), 0.117f, 0.506f, 0.69f));
            
        }


        // b2区：4排15列1x3x1的舞台块，间距0.05
        AreaPos = B2.position;
        float b2spacing = 0.05f; // 方块间隔
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 15; ++j) {
                float xPos = AreaPos.x + j * (1 + b2spacing); // 计算x位置
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b2spacing); // 计算z位置
                B2.AddShape(ShapeFactory::CreateBox("B2_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 1, 3, 1, CVector(xPos, yPos, zPos), 0.08, 0.3, 0.6));
            }
        }

        // c区：8块3x11x0.5的舞台块，左右各4块，间距0.05
        AreaPos = C.position;
        float cspacing = 0.05f;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                float xPos = AreaPos.x + (i * 2 - 1) * (10 + cspacing); // 计算x位置
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + j * (2 + cspacing); // 计算z位置
                C.AddShape(ShapeFactory::CreateBox("C_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 3, 11, 0.5f, CVector(xPos, yPos, zPos), 1, 0.89, 0));
            }
        }

        // e29区：2排，前排29个，后排25个，大小1x12x0.1，间距0.05
        AreaPos = E29.position;
        float e29spacing = 0.05f;
        for (int j = 0; j < 29; ++j) {
            float xPos = AreaPos.x + j * (1 + e29spacing); // 计算x位置
            float yPos = AreaPos.y;
            float zPos = AreaPos.z; // 计算z位置
            E29.AddShape(ShapeFactory::CreateBox("E29_" + std::to_string((j + 1)),1, 12, 0.1f, CVector(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        }

        // e25区：2排，前排29个，后排25个，大小1x12x0.1，间距0.05
        AreaPos = E25.position;
        float e25spacing = 0.05f;
        for (int j = 0; j < 25; ++j) {
            float xPos = AreaPos.x + j * (1 + e25spacing); // 计算x位置
            float yPos = AreaPos.y;
            float zPos = AreaPos.z + 1 * (2 + e25spacing); // 计算z位置
            E25.AddShape(ShapeFactory::CreateBox("E25_" + std::to_string((j + 1)),1, 12, 0.1f, CVector(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        }
        Car.AddShape(ShapeFactory::CreateBox("Car_Body", 2, 1, 4, CVector(0,2.5+0.5,0), 0.9, 0.9, 0.9,true));
        isInitStage = true;
    }
    Floor.Update();
    stage.Update();
    camera.Update();

}

// 渲染场景
void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    camera.LookAt();
    glGetDoublev(GL_MODELVIEW_MATRIX,stage.modelViewMatrix);
    drawCoordinateAxes();
    drawStage();
    glPopMatrix();
    glutSwapBuffers();
}


// 处理窗口大小变化
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, GLfloat(w) / h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// 处理键盘输入
void processKeyboard(unsigned char key, int x, int y)
{
    InputManager::instance().enqueueKeyboardEvent(key, x, y);
    glutPostRedisplay();
}

// 处理键盘输入
void processSpecialKeys(int key, int x, int y)
{
    InputManager::instance().enqueueSpecialKeyEvent(key, x, y);
    glutPostRedisplay();
}

// 处理鼠标按键输入
void processMouse(int button, int state, int x, int y)
{
    InputManager::instance().enqueueMouseEvent(button, state, x, y);
}

// 处理鼠标移动
void processMouseMotion(int x, int y)
{
    InputManager::instance().enqueueMouseMotionEvent(x, y);
}

// 主程序
int main(int argc, char* argv[])
{
    Calculate();
    // 初始化 GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("作业一");

    // 设置 GLUT 回调函数
    glutDisplayFunc(&myDisplay);
    glutTimerFunc(1, myTimerFunc, 0);
    glutReshapeFunc(&myReshape);
    glutKeyboardFunc(&processKeyboard);   // 注册键盘输入
    glutSpecialFunc(&processSpecialKeys);
    glutMouseFunc(&processMouse);         // 注册鼠标按键输入
    glutMotionFunc(&processMouseMotion);  // 注册鼠标拖动

    // 设置渲染环境
    SetRC();

    // 进入 GLUT 主循环
    glutMainLoop();
    return 0;
}




