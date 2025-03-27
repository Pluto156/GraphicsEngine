#include "stdafx.h"
Camera camera;
Stage stage("stage",0,0,0);
Area Floor("Floor",0, -1 - 0.05, 0);
Area A("A", 0, 0, 0);
Area B1("B1", -(9 + 8 * 0.05) / 2 + 0.5, 0, 0);
Area B2("B2", -(15 + 14 * 0.05) / 2 + 0.5, 1, -4 - 4 * 0.05);
Area C("C", 0, 2.5 + 5.5, 0);
Area E29("E29", -(29 + 28 * 0.05) / 2 + 0.5, 20, 0);
Area E25("E25", -(25 + 24 * 0.05) / 2 + 0.5, 20, 0);
bool isRotate = false;
float angle = 0;
bool isInitStage = false;
double modelViewMatrix[16];


CVector origin;
CVector direct;
void myDisplay(void);
void myTimerFunc(int val);
void SetRC();
void myReshape(int w, int h);
void processKeyboard(unsigned char key, int x, int y);  // �����������
void processMouse(int button, int state, int x, int y); // �����������
void processMouseMotion(int x, int y);

// ��ӡ 4x4 ����
void printMatrix(const GLfloat* matrix, const char* name) {
    std::cout << name << " Matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[j * 4 + i] << "\t";  // OpenGL �������ȴ洢����
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}// ��������ƶ�
void display() {

    static GLfloat lastModelViewMatrix[16] = { 0 };  // �洢�ϴεľ��󣬳�ʼΪ 0

    // ��ȡ��ǰ ģ����ͼ����
    GLfloat modelViewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

    // �����������仯ʱ�Ŵ�ӡ
    if (memcmp(lastModelViewMatrix, modelViewMatrix, sizeof(modelViewMatrix)) != 0) {
        printMatrix(modelViewMatrix, "ModelView");
        memcpy(lastModelViewMatrix, modelViewMatrix, sizeof(modelViewMatrix));  // ���»���
    }

    //// ��ȡ����ӡ ͶӰ����
    //GLfloat projectionMatrix[16];
    //glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
    //printMatrix(projectionMatrix, "Projection");

    //// ��ȡ����ӡ �������
    //GLfloat textureMatrix[16];
    //glGetFloatv(GL_TEXTURE_MATRIX, textureMatrix);
    //printMatrix(textureMatrix, "Texture");

    //// ��ȡ����ӡ �ӿڣ�Viewport��
    //GLint viewport[4];
    //glGetIntegerv(GL_VIEWPORT, viewport);
    //std::cout << "Viewport: x=" << viewport[0] << ", y=" << viewport[1]
    //    << ", width=" << viewport[2] << ", height=" << viewport[3] << std::endl;

}
// ��ʱ���ص�����
void myTimerFunc(int val)
{
    myDisplay();
    glutTimerFunc(1, myTimerFunc, 0);
}

// ������Ⱦ����
void SetRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);  // ������Ȳ��ԣ�ȷ�����尴��ȷ��˳����Ⱦ
}

// ������������ϵ�����ᣨXYZ��
void drawCoordinateAxes()
{
    // X�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);  // X��������
    glEnd();

    // Y�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);  // Y��������
    glEnd();

    // Z�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);  // Z��������
    glEnd();


    //glBegin(GL_LINES);
    //glColor3f(0.0f, 0.0f, 1.0f);  // ��ɫ
    //glVertex3f(origin.x, origin.y, origin.z);
    //CVector end = origin +   direct* 100;
    //glVertex3f(end.x, end.y, end.z);  // Z��������
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
        //stage.AddArea(&Floor);
        stage.AddArea(&A);
        stage.AddArea(&B1);
        stage.AddArea(&B2);
        stage.AddArea(&C);
        stage.AddArea(&E29);
        stage.AddArea(&E25);
        stage.position=CVector(0, 0, 0);
        // �ذ���
        CVector AreaPos = Floor.position;
        Floor.AddBox(Box::CreateBox("floor",100, 0.1, 100, AreaPos, 0.5f, 0.5f, 0.5f));


        AreaPos = B1.position;
        // B1����10��9��1x5x1����̨�飬���0.05
        float b1Spacing = 0.05f; // ������
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                float xPos = AreaPos.x + j * (1 + b1Spacing); // ����xλ��
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b1Spacing); // ����zλ��
                B1.AddBox(Box::CreateBox("B1_"+ std::to_string((i+1))+'_' + std::to_string((j + 1)), 1, 5, 1, CVector(xPos, yPos, zPos), 0.117f, 0.506f, 0.69f));
            }
        }

        // a����22��1x12x1�ķ��飬�������У�����ÿ��box��λ��
        float radius = 10.0f; // ���軡�εİ뾶
        A.position = CVector(0, 6, radius-10);
        float anglestep = 0.15; // ��22����ƽ���ֲ��ڻ�����
        AreaPos = A.position;

        for (int i = 0; i < 11; ++i) {
            float angle = i * anglestep;  // ÿ������ĽǶ�
            float xPos = AreaPos.x - radius * sin(angle);  // ����xλ��
            float yPos = AreaPos.y;  // yλ�ñ��ֲ���
            float zPos = AreaPos.z - radius * cos(angle);  // ����zλ��
            A.AddBox(Box::CreateBox("A_" + std::to_string(i+1),1, 12, 1, CVector(xPos, yPos, zPos),CMatrix::CreateRotationMatrixY(angle),CEuler(angle*180/M_PI,0,0), 0.117f, 0.506f, 0.69f,true));
            A.AddBox(Box::CreateBox("A_" + std::to_string(2*(i+i)),1, 12, 1, CVector(-xPos, yPos, zPos), CMatrix::CreateRotationMatrixY(-angle), CEuler(-angle * 180 / M_PI, 0, 0), 0.117f, 0.506f, 0.69f, true));
            
        }


        // b2����4��15��1x3x1����̨�飬���0.05
        AreaPos = B2.position;
        float b2spacing = 0.05f; // ������
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 15; ++j) {
                float xPos = AreaPos.x + j * (1 + b2spacing); // ����xλ��
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + i * (1 + b2spacing); // ����zλ��
                B2.AddBox(Box::CreateBox("B2_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 1, 3, 1, CVector(xPos, yPos, zPos), 0.08, 0.3, 0.6));
            }
        }

        // c����4��3x11x0.5����̨�飬���Ҹ�2�飬���0.05
        AreaPos = C.position;
        float cspacing = 0.05f;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                float xPos = AreaPos.x + (i * 2 - 1) * (10 + cspacing); // ����xλ��
                float yPos = AreaPos.y;
                float zPos = AreaPos.z + j * (2 + cspacing); // ����zλ��
                C.AddBox(Box::CreateBox("C_" + std::to_string((i + 1)) + '_' + std::to_string((j + 1)), 3, 11, 0.5f, CVector(xPos, yPos, zPos), 1, 0.89, 0));
            }
        }

        // e29����2�ţ�ǰ��29��������25������С1x12x0.1�����0.05
        AreaPos = E29.position;
        float e29spacing = 0.05f;
        for (int j = 0; j < 29; ++j) {
            float xPos = AreaPos.x + j * (1 + e29spacing); // ����xλ��
            float yPos = AreaPos.y;
            float zPos = AreaPos.z; // ����zλ��
            E29.AddBox(Box::CreateBox("E29_" + std::to_string((j + 1)),1, 12, 0.1f, CVector(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        }

        // e25����2�ţ�ǰ��29��������25������С1x12x0.1�����0.05
        AreaPos = E25.position;
        float e25spacing = 0.05f;
        for (int j = 0; j < 25; ++j) {
            float xPos = AreaPos.x + j * (1 + e25spacing); // ����xλ��
            float yPos = AreaPos.y;
            float zPos = AreaPos.z + 1 * (2 + e25spacing); // ����zλ��
            E25.AddBox(Box::CreateBox("E25_" + std::to_string((j + 1)),1, 12, 0.1f, CVector(xPos, yPos, zPos), 0.9, 0.57, 0.12));
        }
        isInitStage = true;
    }

    if (isRotate)
    {
        angle = (angle + 0.1) / 1;
        angle = angle >= 360 ? angle - 360 : angle;
        stage.SetRotation(CMatrix::CreateRotationMatrixY((angle)*M_PI / 180));
    }
    Floor.Draw();
    stage.Draw();

}

// ��Ⱦ����
void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    camera.LookAt();
    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
    //display();
    //camera.CameraDebug();
    // ������������ϵ
    //drawCoordinateAxes();
    //if (isRotate)
    //{
    //    glRotatef(angle++ / 100, 0, 1, 0);
    //}
    //else
    //{
    //    glRotatef(angle / 100, 0, 1, 0);
    //}
    drawStage();
    glPopMatrix();
    glutSwapBuffers();
}


// �����ڴ�С�仯
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, GLfloat(w) / h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// �����������
void processKeyboard(unsigned char key, int x, int y)
{
    if (key == '2')
    {
        isRotate = !isRotate;
    }
    camera.processKeyboard(key, x, y);  // ʹ�� Camera �����������������
    glutPostRedisplay();
}

// ������갴������
void processMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            float val;
            double modelview[16], project[16], pos[3];
            int viewport[4];
            //glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, project);
            glGetIntegerv(GL_VIEWPORT, viewport);
            y = viewport[3] - y;
            glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
            gluUnProject(x, y, val, modelViewMatrix, project, viewport, &pos[0], &pos[1], &pos[2]);

            //printf("%d:%d\t(%d:%d)\t%f\t(%.2f,%.2f,%.2f)\n", button, state, x, y, val, pos[0], pos[1], pos[2]);
            origin= camera.position;
            direct = CVector(pos[0], pos[1], pos[2])- camera.position;
            direct.Normalize();
            PointCollision PointCollision;
            stage.IntersectWithRay(origin, direct,100);
        }
    }
    camera.processMouse(button, state, x, y);  // ʹ�� Camera �����������������
}

// ��������ƶ�
void processMouseMotion(int x, int y)
{
    camera.processMouseMotion(x, y);  // ʹ�� Camera ��������������ƶ�
    //camera.CameraDebug();
}

// ������
int main(int argc, char* argv[])
{
    Calculate();
    // ��ʼ�� GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("��ҵһ");

    // ���� GLUT �ص�����
    glutDisplayFunc(&myDisplay);
    glutTimerFunc(1, myTimerFunc, 0);
    glutReshapeFunc(&myReshape);
    glutKeyboardFunc(&processKeyboard);   // ע���������
    glutMouseFunc(&processMouse);         // ע����갴������
    glutMotionFunc(&processMouseMotion);  // ע������϶�

    // ������Ⱦ����
    SetRC();

    // ���� GLUT ��ѭ��
    glutMainLoop();
    return 0;
}




