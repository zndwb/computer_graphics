#include <GL/glut.h>
#include <math.h>
#include <imgui.h>
#include<cmath>
#include<gl/freeglut.h>
#include<vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION


#define M_PI 3.14159265358979323846
using namespace std;

const int numSlices = 12;   // 玉米的片数
const int numStacks = 20;   // 玉米粒的行数
const float radiusTop = 0.08;  // 顶部半径
const float radiusBottom = 0.2; // 底部半径
const float radiusMiddle = 0.3; // 中部半径
const float height = 3.0;  // 玉米的高度
float r = 0.1;
int lastX = 0, lastY = 0;
float rotationX = 0.0, rotationY = 0.0;
float r_0 = 0.5, g_0 = 0.5, b_0 = 0.1;
GLuint texture;
float zoom = 1.0f; // 缩放比例
// 生成Perlin噪声
float PerlinNoise(float x, float y) {
    return (sin(x * 12.9898 + y * 78.233) * 43758.5453 - floor(sin(x * 12.9898 + y * 78.233) * 43758.5453));
}
// 生成纹理数据
std::vector<unsigned char> GenerateTextureData(int width, int height) {
    std::vector<unsigned char> data(width * height * 3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float n = PerlinNoise(x * 0.1f, y * 0.1f);
            unsigned char value = static_cast<unsigned char>((n + 1.0f) * 0.5f * 255);
            data[(y * width + x) * 3 + 0] = value;
            data[(y * width + x) * 3 + 1] = value;
            data[(y * width + x) * 3 + 2] = value;
        }
    }
    return data;
}

std::vector<unsigned char> GenerateTextureData1(int width, int height, float rMult, float gMult, float bMult, float brightness) {
    std::vector<unsigned char> data(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float distance = std::sqrt(std::pow(x - width / 2, 2) + std::pow(y - height / 2, 2));
            float intensity = std::max(0.0f, 1.0f - distance / (width / 2));
            unsigned char r = static_cast<unsigned char>(std::min(255.0f, rMult * intensity * brightness * 255));
            unsigned char g = static_cast<unsigned char>(std::min(255.0f, gMult * intensity * brightness * 255));
            unsigned char b = static_cast<unsigned char>(std::min(255.0f, bMult * intensity * brightness * 255));

            data[(y * width + x) * 3 + 0] = r;
            data[(y * width + x) * 3 + 1] = g;
            data[(y * width + x) * 3 + 2] = b;
        }
    }

    return data;
}

// 初始化纹理
GLuint LoadGeneratedTexture(int width, int height, float rMult, float gMult, float bMult, float brightness) {
    GLuint texture;
    std::vector<unsigned char> data = GenerateTextureData1(width, height, rMult, gMult, bMult, brightness);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

// 初始化函数
void init() {
    glClearColor(0.5f, 0.5f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    // 设置光源
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 加载生成的纹理
    texture = LoadGeneratedTexture(512, 512, 0.5, 0.5, 0.5, 3.0);
}
// 绘制玉米粒
void drawCornKernel(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // 设置材质属性以确保显示黄色
    GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.0f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.0f, 1.0f };
    GLfloat mat_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

// 绘制玉米
void drawCorn(float x00, float y00, float z00, float a) {
    for (int i = 0; i < numStacks; i++) {
        float y = i * (height / numStacks);
        float radius = radiusTop + (radiusMiddle - radiusTop) * sin(M_PI * i / numStacks);

        for (int j = 0; j < numSlices; j++) {
            float angle = j * 2.0f * M_PI / numSlices;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;
            float tempZ = z;
            float tempY = y * cos(a) + x * sin(a);
            float tempX = -y * sin(a) + x * cos(a);
            tempX += x00;
            tempY += y00;
            tempZ += z00;
            drawCornKernel(tempX, tempY, tempZ, r);
        }
    }
}

float texCoords[2][2][2] = {
    {{0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.0f, 1.0f}, {1.0f, 1.0f}}
};


void drawBezierSurface(float controlPoints[4][4][3], int grid) {
    glBindTexture(GL_TEXTURE_2D, texture);  // 绑定生成的纹理
    glEnable(GL_TEXTURE_2D);  // 启用纹理映射

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &controlPoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);

    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texCoords[0][0][0]); // 定义纹理坐标
    glEnable(GL_MAP2_TEXTURE_COORD_2);

    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, grid, 0, grid);

    glDisable(GL_TEXTURE_2D);  // 禁用纹理映射
    glBindTexture(GL_TEXTURE_2D, 0);  // 解绑纹理
}


void* rotatePoints(float p[4][4][3], float cosa, float sina) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float x = p[i][j][0];
            float z = p[i][j][2];
            p[i][j][0] = x * cosa - z * sina; // 新的x坐标
            p[i][j][2] = x * sina + z * cosa; // 新的z坐标
        }
    }
    return 0;
}


void drawCornLeaf(float x, float y, float z, float a, float l) {
    glPushMatrix();

    GLfloat mat_ambient[] = { 0.0f, 0.3f, 0.0f, 1.0f };
    GLfloat mat_diffuse[] = { 0.0f, 0.5f, 0.0f, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_shininess[] = { 20.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    float p[4][4][3] = {
        {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}},
        {{0.5,1.5,-0.5},{1.0,1.5,-0.3},{1.0,1.5,0.3},{0.5,1.5,0.5}},
        {{1.5,2.5,-0.5},{2.0,2.5,-0.3},{2.0,2.5,0.3},{1.5,2.5,0.5}},
        {{2.5,3.0,0.0},{3.0,3.0,0.0},{3.0,3.0,0.0},{2.5,3.0,0.0}}
    };
    rotatePoints(p, cos(a), sin(a));

    float controlPoints[4][4][3] = {
        { { x + p[0][0][0] * l, y + 0.0 * l, z + p[0][0][2] * l}, { x + p[0][1][0] * l, y + 0.0 * l, z + p[0][1][2] * l}, { x + p[0][2][0] * l, y + 0.0 * l, z + p[0][2][2] * l}, { x + p[0][3][0] * l, y + 0.0 * l, z + p[0][3][2] * l}},
        { { x + p[1][0][0] * l, y + 1.5 * l, z + p[1][0][2] * l}, { x + p[1][1][0] * l, y + 1.5 * l, z + p[1][1][2] * l}, { x + p[1][2][0] * l, y + 1.5 * l, z + p[1][2][2] * l}, { x + p[1][3][0] * l, y + 1.5 * l, z + p[1][3][2] * l}},
        { { x + p[2][0][0] * l, y + 2.5 * l, z + p[2][0][2] * l}, { x + p[2][1][0] * l, y + 2.5 * l, z + p[2][1][2] * l}, { x + p[2][2][0] * l, y + 2.5 * l, z + p[2][2][2] * l}, { x + p[2][3][0] * l, y + 2.5 * l, z + p[2][3][2] * l} },
        { { x + p[3][0][0] * l, y + 3.0 * l, z + p[3][0][2] * l}, { x + p[3][1][0] * l, y + 3.0 * l, z + p[3][1][2] * l}, { x + p[3][2][0] * l, y + 3.0 * l, z + p[3][2][2] * l}, { x + p[3][3][0] * l, y + 3.0 * l, z + p[3][3][2] * l} }
    };

    drawBezierSurface(controlPoints, 20);

    glPopMatrix();
}

void drawjinggan(float x, float y, float z, float r, float l) {
    glPushMatrix();

    glTranslatef(x, y, z); // 将圆柱底部移动到 xyz 位置

    GLfloat mat_ambient[] = { 0.0f, 0.3f, 0.0f, 1.0f };
    GLfloat mat_diffuse[] = { 0.0f, 0.5f, 0.0f, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_shininess[] = { 20.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 绕 x 轴旋转 90 度
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    // 创建一个新的二次方程对象
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH); // 启用平滑法线
    gluQuadricTexture(quadric, GL_TRUE); // 启用纹理坐标生成

    // 绘制圆柱体
    gluCylinder(quadric, r, r, l, 32, 32);

    // 绘制上端盖
    glPushMatrix();
    glTranslatef(0, 0, l);
    gluDisk(quadric, 0, r, 32, 32);
    glPopMatrix();

    // 绘制底端盖
    gluDisk(quadric, 0, r, 32, 32);

    // 删除二次方程对象
    gluDeleteQuadric(quadric);
    glPopMatrix();
}


void drawLine(GLfloat controlPoints[3][3], float xx, float yy, float zz, float a) {

    // 应用旋转变换
    for (int i = 0; i < 3; i++) {
        GLfloat x = controlPoints[i][0];
        GLfloat z = controlPoints[i][2];
        controlPoints[i][0] = cos(a) * x - sin(a) * z;
        controlPoints[i][2] = sin(a) * x + cos(a) * z;
    }

    // 设置线宽
    glLineWidth(5.0f);

    GLfloat mat_ambient[] = { 0.94f, 0.86f, 0.51f, 1.0f };
    GLfloat mat_diffuse[] = { 0.94f, 0.86f, 0.51f, 1.0f };
    GLfloat mat_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };


    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 绘制贝塞尔曲线
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 100; ++i) {
        float t = i / 100.0f;
        float x = (1 - t) * (1 - t) * controlPoints[0][0] + 2 * (1 - t) * t * controlPoints[1][0] + t * t * controlPoints[2][0];
        float y = (1 - t) * (1 - t) * controlPoints[0][1] + 2 * (1 - t) * t * controlPoints[1][1] + t * t * controlPoints[2][1];
        float z = (1 - t) * (1 - t) * controlPoints[0][2] + 2 * (1 - t) * t * controlPoints[1][2] + t * t * controlPoints[2][2];
        glVertex3f(xx + x, yy + y, zz + z);
    }
    glEnd();
}
// 显示回调函数
GLfloat controlPoints1[3][3] = {
        { 0.0, 0.0, 0.0 },
        {  1.0, 2.0, 0.0 },
        {  2.0, 1.0, 0.0 }
};
GLfloat controlPoints2[3][3] = {
        { 0.0, 0.0, 0.0 },
        {  1.2, 4.0, 0.0 },
        {  3.1, 2.0, 0.0 }
};
GLfloat controlPoints3[3][3] = {
        { 0.0, 0.0, 0.0 },
        {  -1.5, 3.5, 0.0 },
        {  -3.5, 1.5, 0.0 }
};
GLfloat controlPoints4[3][3] = {
        { 0.0, 0.0, 0.0 },
        {  0.0, 1.5, 0.0 },
        {  0.0, 4.0, 0.0 }
};
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置观察点
    gluLookAt(0.0, 0.0, 10.0 * zoom,  // 乘以缩放比例
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);

    // 绘制玉米
    drawCorn(-0.1, 1.5, 0.0, M_PI / 6);
    drawCorn(0.1, 0.0, 0.0, -M_PI / 6);

    // 绘制玉米叶子
    drawCornLeaf(0.0, 0.4, 0.0, -M_PI / 3, 1.0);
    drawCornLeaf(0.0, -0.7, 0.0, 0.0, 1.0);
    drawCornLeaf(0.0, -2.7, 0.0, M_PI, 1.0);
    drawCornLeaf(0.0, -2.7, 0.0, 0.0, 1.0);
    drawCornLeaf(0.0, -1.3, 0.0, 2 * M_PI / 3, 1.0);
    drawCornLeaf(0.0, -1.3, 0.0, -M_PI / 3, 1.0);
    drawCornLeaf(0.0, 0.8, 0.0, M_PI, 1.0);
    drawCornLeaf(0.0, -4.0, 0.0, 0, 2.0);
    drawCornLeaf(0.0, -3.7, 0.0, 8* M_PI / 7, 2.0);
    drawCornLeaf(0.0, -4.8, 0.0, -1 * M_PI / 10, 1.8);
    drawCornLeaf(0.0, -5.5, 0.0, 6*M_PI / 7, 1.8);
    drawCornLeaf(0.0, 2.0, 0.0, -M_PI / 9, 0.8);
    drawCornLeaf(0.0, 2.3, 0.0, 1 * M_PI / 7, 0.8);

    //绘制玉米穗
    drawLine(controlPoints1, 0.0, 5.0, 0.0, 0.0);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, 0.1);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, 0.2);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, 0.3);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, -0.1);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, -0.2);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, -0.3);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, -0.45);
    drawLine(controlPoints1, 0.0, 5.0, 0.0, 0.45);

    drawLine(controlPoints2, 0.0, 5.0, 0.0, 0.0);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, 0.15);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, 0.25);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, 0.35);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, -0.15);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, -0.25);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, -0.35);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, -0.45);
    drawLine(controlPoints2, 0.0, 5.0, 0.0, 0.45);

    drawLine(controlPoints3, 0.0, 5.0, 0.0, 0.0);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, 0.1);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, 0.2);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, 0.3);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, -0.1);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, -0.2);
    drawLine(controlPoints3, 0.0, 5.0, 0.0, -0.3);
    
    drawLine(controlPoints4, 0.0, 5.0, 0.0, 0.0);




    //绘制茎秆
    drawjinggan(0.0, 5.0, 0.0, 0.1, 12.0);

    glFlush();
    glutSwapBuffers();
}

// 窗口重塑回调函数
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// 鼠标按下事件回调函数
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
    }
}

// 鼠标移动事件回调函数
void motion(int x, int y) {
    rotationY += (x - lastX) * 0.1f;
    rotationX += (y - lastY) * 0.1f;

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

// 鼠标滚轮事件回调函数
void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        zoom *= 1.1f; // 放大
    }
    else {
        zoom /= 1.1f; // 缩小
    }
    glutPostRedisplay();
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("OpenGL Corn");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMouseWheelFunc(mouseWheel); // 添加鼠标滚轮事件回调函数
    glutMainLoop();
    return 0;
}


