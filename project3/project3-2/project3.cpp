#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

// 控制点
#define N 8
float controlPoints1[N][N][3] = {
    {{-150, -150, 100}, {-100, -150, 0}, {-50, -150, 0}, {0, -150, 50}, {50, -150, 0}, {100, -150, 0}, {150, -150, -100}, {200, -150, 0}},
    {{-150, -100, 0}, {-100, -100, 50}, {-50, -100, -100}, {0, -100, 0}, {50, -100, 0}, {100, -100, 100}, {150, -100, 0}, {200, -100, -50}},
    {{-150, -50, 0}, {-100, -50, 0}, {-50, -50, 50}, {0, -50, 0}, {50, -50, 0}, {100, -50, 0}, {150, -50, -50}, {200, -50, 0}},
    {{-150, 0, -50}, {-100, 0, -100}, {-50, 0, 0}, {0, 0, 150}, {50, 0, 0}, {100, 0, -100}, {150, 0, -50}, {200, 0, -50}},
    {{-150, 50, 0}, {-100, 50, 0}, {-50, 50, -100}, {0, 50, 0}, {50, 50, 50}, {100, 50, 0}, {150, 50, 0}, {200, 50, 50}},
    {{-150, 100, 50}, {-100, 100, 0}, {-50, 100, 0}, {0, 100, -50}, {50, 100, -50}, {100, 100, 50}, {150, 100, 0}, {200, 100, 0}},
    {{-150, 150, 0}, {-100, 150, 100}, {-50, 150, -100}, {0, 150, 0}, {50, 150, 0}, {100, 150, 0}, {150, 150, -150}, {200, 150, 50}},
    {{-150, 200, 0}, {-100, 200, 0}, {-50, 200, 50}, {0, 200, -50}, {50, 200, 0}, {100, 200, -100}, {150, 200, 0}, {200, 200, 50}}
};

float controlPoints2[N][N][3] = {
    {{-200, -200, 150}, {-150, -200, 50}, {-100, -200, 0}, {-50, -200, 50}, {0, -200, 100}, {50, -200, 0}, {100, -200, -50}, {150, -200, 0}},
    {{-200, -150, 50}, {-150, -150, 0}, {-100, -150, 50}, {-50, -150, 0}, {0, -150, 50}, {50, -150, 100}, {100, -150, 0}, {150, -150, -50}},
    {{-200, -100, 0}, {-150, -100, 50}, {-100, -100, 0}, {-50, -100, 100}, {0, -100, 0}, {50, -100, 50}, {100, -100, -50}, {150, -100, 0}},
    {{-200, -50, 50}, {-150, -50, 0}, {-100, -50, 50}, {-50, -50, 0}, {0, -50, 150}, {50, -50, 0}, {100, -50, -50}, {150, -50, 0}},
    {{-200, 0, 0}, {-150, 0, 50}, {-100, 0, 0}, {-50, 0, 100}, {0, 0, 0}, {50, 0, 50}, {100, 0, 0}, {150, 0, -50}},
    {{-200, 50, 50}, {-150, 50, 0}, {-100, 50, 50}, {-50, 50, 0}, {0, 50, 100}, {50, 50, 0}, {100, 50, -50}, {150, 50, 0}},
    {{-200, 100, 0}, {-150, 100, 50}, {-100, 100, 0}, {-50, 100, 100}, {0, 100, 0}, {50, 100, 50}, {100, 100, 0}, {150, 100, -50}},
    {{-200, 150, 50}, {-150, 150, 0}, {-100, 150, 50}, {-50, 150, 0}, {0, 150, 100}, {50, 150, 0}, {100, 150, -50}, {150, 150, 0}}
};

float controlPoints3[N][N][3] = {
    {{-250, -250, 200}, {-200, -250, 100}, {-150, -250, 50}, {-100, -250, 0}, {-50, -250, 50}, {0, -250, 100}, {50, -250, 50}, {100, -250, 0}},
    {{-250, -200, 100}, {-200, -200, 50}, {-150, -200, 0}, {-100, -200, 50}, {-50, -200, 100}, {0, -200, 50}, {50, -200, 0}, {100, -200, 50}},
    {{-250, -150, 0}, {-200, -150, 50}, {-150, -150, 100}, {-100, -150, 0}, {-50, -150, 50}, {0, -150, 100}, {50, -150, 50}, {100, -150, 0}},
    {{-250, -100, 50}, {-200, -100, 0}, {-150, -100, 50}, {-100, -100, 100}, {-50, -100, 0}, {0, -100, 50}, {50, -100, 0}, {100, -100, 50}},
    {{-250, -50, 0}, {-200, -50, 50}, {-150, -50, 0}, {-100, -50, 50}, {-50, -50, 100}, {0, -50, 50}, {50, -50, 0}, {100, -50, 50}},
    {{-250, 0, 50}, {-200, 0, 0}, {-150, 0, 50}, {-100, 0, 0}, {-50, 0, 50}, {0, 0, 100}, {50, 0, 50}, {100, 0, 0}},
    {{-250, 50, 0}, {-200, 50, 50}, {-150, 50, 0}, {-100, 50, 50}, {-50, 50, 100}, {0, 50, 50}, {50, 50, 0}, {100, 50, 50}},
    {{-250, 100, 50}, {-200, 100, 0}, {-150, 100, 50}, {-100, 100, 0}, {-50, 100, 50}, {0, 100, 100}, {50, 100, 50}, {100, 100, 0}}
};

float rotateX = 0.0, rotateY = 0.0; // 旋转角度
int lastMouseX, lastMouseY; // 上次鼠标位置

float(*selectedControlPoints)[N][N][3]; // 选择的控制点

// 计算B样条基函数
float B(int i, int k, float t, const float* knots) {
    if (k == 0) {
        return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f;
    }
    else {
        float d1 = knots[i + k] - knots[i];
        float d2 = knots[i + k + 1] - knots[i + 1];
        float term1 = (d1 == 0) ? 0 : ((t - knots[i]) / d1) * B(i, k - 1, t, knots);
        float term2 = (d2 == 0) ? 0 : ((knots[i + k + 1] - t) / d2) * B(i + 1, k - 1, t, knots);
        return term1 + term2;
    }
}

// 计算并绘制B样条曲面
void drawBSplineSurface(float controlPoints[N][N][3]) {
    int numU = 70, numV = 70;
    float knotsU[N + 4];
    float knotsV[N + 4];
    for (int i = 0; i < N + 4; ++i) {
        knotsU[i] = knotsV[i] = (float)i / (N + 3);
    }

    glColor3f(0.0, 0.0, 1.0);
    for (int i = 0; i < numU; ++i) {
        float u = (float)i / numU;
        float nextU = (float)(i + 1) / numU;

        for (int j = 0; j < numV; ++j) {
            float v = (float)j / numV;
            float nextV = (float)(j + 1) / numV;

            glBegin(GL_QUADS);
            for (int k = 0; k < 4; ++k) {
                float currU = (k == 0 || k == 3) ? u : nextU;
                float currV = (k == 0 || k == 1) ? v : nextV;

                float x = 0, y = 0, z = 0;
                for (int m = 0; m < N; ++m) {
                    for (int n = 0; n < N; ++n) {
                        float Bu = B(m, 3, currU, knotsU);
                        float Bv = B(n, 3, currV, knotsV);
                        x += Bu * Bv * controlPoints[m][n][0];
                        y += Bu * Bv * controlPoints[m][n][1];
                        z += Bu * Bv * controlPoints[m][n][2];
                    }
                }
                glVertex3f(x, y, z);
            }
            glEnd();
        }
    }
}

// 绘制控制点
void drawControlPoints(float controlPoints[N][N][3]) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            glVertex3f(controlPoints[i][j][0], controlPoints[i][j][1], controlPoints[i][j][2]);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    // 绘制控制点
    drawControlPoints(*selectedControlPoints);

    // 绘制B样条曲面
    drawBSplineSurface(*selectedControlPoints);

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void mouseMotion(int x, int y) {
    rotateX += (y - lastMouseY);
    rotateY += (x - lastMouseX);
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("B样条曲面");
    init();

    std::cout << "以下有三种绘制B样条曲面的控制点组，请选择你要使用的组 (1, 2, 3): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        selectedControlPoints = &controlPoints1;
    }
    else if (choice == 2) {
        selectedControlPoints = &controlPoints2;
    }
    else if (choice == 3) {
        selectedControlPoints = &controlPoints3;
    }
    else {
        std::cerr << "无效的选择，默认使用控制点组1" << std::endl;
        selectedControlPoints = &controlPoints1;
    }

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}