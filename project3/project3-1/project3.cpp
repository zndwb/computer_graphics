#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_POINTS = 100; // 设置最大点数限制
int n; // 控制点数量
int kk;//基函数阶数
vector<float> px, py; // 控制点的x和y坐标
vector<float> knots; // 节点向量

void drawPoint(float x, float y) {
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0); // 红色
    glVertex2f(x, y);
    glEnd();
}

// 计算B样条基函数
float N(int i, int k, float t) {
    if (k == 1) {
        return (t >= knots[i] && t < knots[i + 1]) ? 1.0 : 0.0;
    }
    else {
        float a = 0.0, b = 0.0;
        if (knots[i + k - 1] != knots[i]) {
            a = (t - knots[i]) / (knots[i + k - 1] - knots[i]) * N(i, k - 1, t);
        }
        if (knots[i + k] != knots[i + 1]) {
            b = (knots[i + k] - t) / (knots[i + k] - knots[i + 1]) * N(i + 1, k - 1, t);
        }
        return a + b;
    }
}

// 初始化节点向量
void initializeKnots() {
    int knotCount = n + kk;
    knots.resize(knotCount);
    for (int i = 0; i < knotCount; ++i) {
        if (i < kk) {
            knots[i] = 0.0;
        }
        else if (i >= knotCount - kk) {
            knots[i] = 1.0;
        }
        else {
            knots[i] = (float)(i - kk+1) / (knotCount - 2*kk+1);
        }
    }
}

// 绘制B样条曲线
void drawBSpline() {
    glColor3f(0.0, 0.0, 1.0); // 蓝色
    glBegin(GL_LINE_STRIP);
    for (float t = 0.0; t <= 1.0; t += 0.01) {
        float x = 0.0, y = 0.0;
        for (int i = 0; i < n; i++) {
            float b = N(i, kk, t);
            x += b * px[i];
            y += b * py[i];
        }
        glVertex2f(x, y);
    }
    glEnd();
}

void mouseHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && px.size() < n) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float normalizedX = (float)x;
        float normalizedY = (float)(windowHeight - y);
        drawPoint(normalizedX, normalizedY);
        px.push_back(normalizedX);
        py.push_back(normalizedY);
        glutPostRedisplay();
    }
}

void display() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // 白色背景
    glClear(GL_COLOR_BUFFER_BIT);

    if (px.size() == n) {
        drawBSpline();
    }

    // 在每次刷新时重新绘制所有已存储的点
    for (int i = 0; i < px.size(); ++i) {
        drawPoint(px[i], py[i]);
    }

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    cout << "请输入点的数量（不超过 " << MAX_POINTS << "）：" << endl;
    cin >> n;
    if (n < 1) {
        cout << "点数过少，至少需要1个点。" << endl;
        return -1;
    }

    cout << "请输入基函数的阶数：" << endl;
    cin >> kk;
    if (kk < 2) {
        cout << "基函数的阶数至少为2。" << endl;
        return -1;
    }

    px.reserve(n);
    py.reserve(n);
    initializeKnots();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("B样条曲线");
    glutMouseFunc(mouseHandler);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}