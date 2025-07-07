#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int X1, Y1, X2, Y2;
int a[10];
int f;
void dda(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps;
    float xIncrement, yIncrement, x = x1, y = y1;

    // 确定步数，选择变化较大的轴作为步进方向
    if (abs(dx) > abs(dy)) {
        steps = abs(dx);
    }
    else {
        steps = abs(dy);
    }

    // 计算x和y的增量
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;

    // 绘制起点
    glBegin(GL_POINTS);
    glVertex2i(round(x), round(y));
    glEnd();

    // 迭代计算并绘制后续点
    for (int i = 0; i < steps; i++) {
        x += xIncrement;
        y += yIncrement;
        glBegin(GL_POINTS);
        glVertex2i(round(x), round(y));
        glEnd();
    }
}
void Bresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (true) {
        glBegin(GL_POINTS);
        glVertex2i(x1, y1);
        glEnd();

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
void midpoint_line(int x1, int y1, int x2, int y2)///中点画线法
{
    int x, y, p1, p2, p0, xNext, yNext;    //决策变量p0，p0<=0时增量为p2, p0>0时增量为p1
    float k;
    if (x2 < x1)
    {
        p0 = x1, x1 = x2, x2 = p0;
        p0 = y1, y1 = y2, y2 = p0;
    }
    x = y1 - y2, y = x2 - x1;//x==a y==b
    if (y == 0)
        k = -1 * x * 100;
    else
        k = (float)x / (x1 - x2);
    xNext = x1, yNext = y1;
    glBegin(GL_POINTS);///画点函数,只有两个参数,
    glVertex2i(xNext, yNext);///根据x,y进行划线
    //printf("%d %d\n",xNext,yNext);
    glEnd();
    if (k >= 0 && k <= 1)
    {
        p0 = 2 * x + y; p1 = 2 * x, p2 = 2 * (x + y);
        while (xNext < x2)
        {
            if (p0 <= 0)
            {
                xNext++, yNext++, p0 += p2;
            }
            else
            {
                xNext++, p0 += p1;
            }
            glBegin(GL_POINTS);///画点函数,只有两个参数,
            glVertex2i(xNext, yNext);///根据x,y进行划线
            //printf("%d %d\n",xNext,yNext);
            glEnd();
        }
    }
    else if (k <= 0 && k >= -1)
    {
        p0 = 2 * x - y; p1 = 2 * x - 2 * y, p2 = 2 * x;
        while (xNext < x2)
        {
            if (p0 > 0)
            {
                xNext++, yNext--, p0 += p1;
            }
            else
            {
                xNext++, p0 += p2;
            }
            glBegin(GL_POINTS);///画点函数,只有两个参数,
            glVertex2i(xNext, yNext);///根据x,y进行划线
            //printf("%d %d\n",xNext,yNext);
            glEnd();
        }
    }
    else if (k > 1)
    {
        p0 = x + 2 * y; p1 = 2 * (x + y), p2 = 2 * y;
        while (yNext < y2)
        {
            if (p0 > 0)
            {
                xNext++, yNext++, p0 += p1;
            }
            else
            {
                yNext++, p0 += p2;
            }
            glBegin(GL_POINTS);///画点函数,只有两个参数,
            glVertex2i(xNext, yNext);///根据x,y进行划线
            //printf("%d %d\n",xNext,yNext);
            glEnd();
        }
    }
    else
    {
        p0 = x - 2 * y;
        p1 = -2 * y, p2 = 2 * (x - y);
        while (yNext > y2)
        {
            if (p0 <= 0)
            {
                xNext++, yNext--, p0 += p2;
            }
            else
            {
                yNext--, p0 += p1;
            }
            glBegin(GL_POINTS);///画点函数,只有两个参数,
            glVertex2i(xNext, yNext);///根据x,y进行划线
            //printf("%d %d\n",xNext,yNext);
            glEnd();
        }
    }
}


void display(void)///创建一个没有参数的函数来调用上一个函数
{
    glClear(GL_COLOR_BUFFER_BIT); ///刷新缓存,在窗口中出现
    if(f==1) midpoint_line(a[0], a[1], a[2], a[3]);
    else if(f==2) dda(a[0], a[1], a[2], a[3]);
    else if(f==3) Bresenham(a[0], a[1], a[2], a[3]);
    else {
        cout << "输入错误";
        display();
    }

    glFlush();///清理缓存来处理函数
}
void Scanf()
{
    cout << "请输入起始坐标值：\n";
    cin >> a[0] >> a[1];
    cout << "请输入终点坐标值：\n";
    cin >> a[2] >> a[3];
}
int main(int argc, char* argv[])
{
    Scanf();
    cout << "选择想要使用的算法 1:数值微分算法 2:中点算法 3:Bresenham算法。"<<endl;
    cin >> f;
    glutInit(&argc, argv);///glut的初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);///窗口的大小600*600
    glutInitWindowPosition(0, 0);///定义窗口的位置,在屏幕向左边0像素,屏幕上面向下0像素
    glutCreateWindow("直线扫描转换");
    glClearColor(1.0, 1.0, 1.0, 0.0); ///三个1.0是白色背景,三个0.0是黑色背景
    glColor3f(1, 0, 0); ///红线
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);///在窗口内建立二维直角坐标系,x1,x2,y1,y2
    glutDisplayFunc(display);///在窗口中显示diaplay
    glutMainLoop();
}