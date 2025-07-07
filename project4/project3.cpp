#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// 光照模型类型
enum LightModel {
    GOURAUD,
    PHONG
};

LightModel currentModel = GOURAUD;

// 光照模型参数
float size = 10.0f; // 缩放
void myinit(void);
void myReshape(int width, int height);
void display(void);
static GLfloat spin = 0.0;
static GLfloat i = 0.0;

// 初始化材质特性、光源、光照模型和深度缓冲区
float x1 = -3.0, y1 = -1.0, z1 = 9.0; // 点光源位置
float x2 = 1.0, y2 = 0.0, z2 = -6.0;    // 球的位置

// 记录鼠标状态
int mouseDown = 0;
float xrot = 0.0f, yrot = 0.0f; // 旋转角度
float xdiff = 0.0f, ydiff = 0.0f; // 记录最后一次鼠标位置

float amb = 0.8, dif = 0.2, spe = 1.0;
float ambl = 1.0, difl = 2.0, spel = 0.2;
float rr = 0.0, gg = 0.0, bb = 0.0;

void myinit(void)
{
    glClearColor(rr, gg, bb, 0.0); // 设置背景颜色为浅蓝色
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    GLfloat mat_ambient[] = { amb, amb, amb, 1.0 };
    GLfloat mat_diffuse[] = { dif, dif, dif, 1.0 };
    GLfloat mat_specular[] = { spe, spe, spe, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    GLfloat light0_diffuse[] = { ambl, ambl, ambl, 1.0 }; // 漫反射
    GLfloat light0_position[] = { x1, y1, z1, 0.0 };   // 光源位置
    GLfloat light0_specular[] = { difl, difl, difl, 1.0 }; // 镜面反射
    GLfloat light0_ambient[] = { spel, spel, spel, 1.0 }; // 环境光

    GLfloat spot_direction[] = { -3.0, -1.0, -9.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 120.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

void Draw_Ball()
{
    glPushMatrix();
    glScalef(1.0f, 1.0f, 3.0f);
    glutSolidSphere(0.3f * size, 100, 100);
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置观察点
    gluLookAt(0.0, 0.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // 旋转
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);

    // 根据当前的光照模型设置光照模型
    if (currentModel == GOURAUD) {
        glShadeModel(GL_SMOOTH);
    }
    else if (currentModel == PHONG) {
        glShadeModel(GL_FLAT);
    }

    // 绘制白色球体
    GLfloat white_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat white_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);

    // 将旋转中心移动到圆球的位置
    glTranslatef(x2, y2, z2);

    // 旋转光源
    glPushMatrix();
    glRotatef(spin, 0.0, 0.0, 1.0);
    GLfloat position[] = { x1, y1, z1, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // 绘制光源球
    glPushMatrix();
    glTranslatef(x1, y1, z1); // 光源的位置
    glScalef(0.4, 0.4, 0.4);
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(0.2f, 50.0f, 50.0f);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPopMatrix();

    // 绘制圆球
    glPushMatrix();
    Draw_Ball();
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

void myReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
        exit(0);
        break;
    case 'g': // 切换到Gouraud光照模型
        currentModel = GOURAUD;
        glutPostRedisplay();
        break;
    case 'p': // 切换到Phong光照模型
        currentModel = PHONG;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        y1 += 0.1;
        break;
    case GLUT_KEY_DOWN:
        y1 -= 0.1;
        break;
    case GLUT_KEY_LEFT:
        x1 -= 0.1;
        break;
    case GLUT_KEY_RIGHT:
        x1 += 0.1;
        break;
    case GLUT_KEY_F1: // Increase z-coordinate
        z1 += 0.1;
        break;
    case GLUT_KEY_F2: // Decrease z-coordinate
        z1 -= 0.1;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 使用双缓存模式
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("光照模型");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}