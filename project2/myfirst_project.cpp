#include<iostream>
#include<gl/glut.h>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
using namespace std;
const int window_width = 800, window_height = 600;
const int maxn = 99999;
float a, b, c;
struct point
{
	float x, y;
	point() {}
	point(int xx, int yy) :
		x(xx), y(yy) {}
};
vector<point> vertice; //顶点

typedef struct XET
{
	float x;
	float dx;  // 斜率的倒数
	float ymax; 
	XET* next;
}AET, NET; //AET 活性边表； NET新边表

void draw_a_point(int x, int y);
void PolyScan();
void mymouse(int button, int state, int x, int y);
void display();

int N;

int main(int argc, char* argv[])
{
	cout << "请输入你想要绘制图形的的rgb值（0.0-1.0）";
	cin >> a >> b >> c;
	cout << "选择您要绘制的点数";
	cin >> N;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("扫描线填充");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse);
	glutDisplayFunc(&display);

	glutMainLoop();
	return 0;
}

//画点函数
void draw_a_point(int x, int y)
{
	glBegin(GL_POINTS);
	glColor3f(a,b,c);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void PolyScan()
{

	int Max_Y = 0;
	for (int i = 0; i < vertice.size(); i++)
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y;
	AET* pAET = new AET;
	pAET->next = NULL;
	NET* pNET[800]; 
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;;
	}
	int l = vertice.size();
	for (int i = 0; i <= Max_Y; i++)
	{
		for (int j = 0; j < l; j++) 
		{
			if (i == vertice[j].y)
			{
				if (vertice[(j - 1 + l) % l].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + l) % l].y;
					float DX = vertice[(j - 1 + l) % l].x - vertice[j].x;
					float DY = vertice[(j - 1 + l) % l].y - vertice[j].y;
					p->dx = DX / DY;
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (vertice[(j + 1) % l].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % l].y;
					float DX = vertice[(j + 1) % l].x - vertice[j].x;
					float DY = vertice[(j + 1) % l].y - vertice[j].y;
					p->dx = DX / DY;
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}

	for (int i = 0; i <= Max_Y; i++)
	{
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx; 
			p = p->next;
		}

		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;
		}
		AET* q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;
		}
		p = pAET->next;
		while (p && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				draw_a_point(j, i);
			}
			p = p->next->next;
		}
	}
	glFlush();
}
void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y);

		point p(x, window_height - y);
		vertice.push_back(p);
		cout << "顶点" << vertice.size() << ": (" << x << ", " << window_height - y << ")" << endl;
	}
	
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glPointSize(1);
	glBegin(GL_POINTS);
	if (vertice.size() == N) {
		PolyScan();
	}
	glEnd();
	glFlush();
}

