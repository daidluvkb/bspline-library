#include "display.h"

bool mouseRightIsDown = false;
struct Point
{
	int x, y;
	Point(){};
	Point(int tx, int ty)
	{
		x = tx;
		y = ty;
	}
};
const static int mainwin_wildth = 800, mainwin_height = 480;
const static int subwin_wildth = mainwin_wildth / 4, subwin_height = mainwin_height / 4;
const static int subwingap = 20;

int mode;
double beginx, beginy;

float ratx = 0;
float raty = 0;
float ratz = 0;

double dis = 10.0, azim = 120.0, elev = 20.0;
double ddis = 0.0, dazim = 0.0, delev = 0.0;
double rot1 = 0.0, rot2 = 0.0, rot3 = 0.0;


vector<Point> p;

shared_ptr<BsplineBase> curve_insert_ptr;
//bspline::NuniBspline 
//bspline::BezierBspline curve(2);

//bspline::UniBspline curve;
void changeInsertFocusTo(const shared_ptr<BsplineBase>& curve)
{
	curve_insert_ptr = curve;
}

double getRatio(double t,double a,double b,double c,double d)
{
	return a * pow(t, 3) + b * pow(t, 2) + c * t + d;
}
double caculateSquarDistance(Point a, Point b)
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}
int getIndexNearByMouse(int x, int y)
{
	double precision = 200;		//精确度
	int index = -1;
	double Min;
	for (int i = 0; i < p.size(); i++)
	{
		double dis = caculateSquarDistance(p[i], Point(x, y));
		if (dis < precision)
		{
			if (index == -1)
			{
				index = i;
				Min = dis;
			}
			else if (dis < Min)
			{
				index = i;
				Min = dis;
			}
		}
	}
	return index;
}
void Bspline__(Point a,Point b,Point c,Point d)
{
	int n = 5;
	double derta = 1.0 / n;
	glPointSize(2);
	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= n; i++)
	{
		double t = derta * i;
		double ratio[4];
		ratio[0] = getRatio(t, -1, 3, -3, 1);
		ratio[1] = getRatio(t, 3, -6, 0, 4);
		ratio[2] = getRatio(t, -3, 3, 3, 1);
		ratio[3] = getRatio(t, 1, 0, 0, 0);
		double x=0, y=0;
		x += ratio[0] * a.x + ratio[1] * b.x + ratio[2] * c.x + ratio[3] * d.x;
		y += ratio[0] * a.y + ratio[1] * b.y + ratio[2] * c.y + ratio[3] * d.y;
		x /= 6.0;
		y /= 6.0;
		//cout << "ordinarily: " << x << '\t' << y << endl;
		glVertex2d(x, y);
	}
	glEnd();
}

void drawControlPoints()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点
	for (int i = 0; i < fac.curveNum(); i++)
	{
		shared_ptr<BsplineBase> c = fac.getCurve(i);
		shared_ptr<vector<NEWMAT::ColumnVector>> ps = c->getPoints();
		NEWMAT::ColumnVector vec;
		glPointSize(5);
		glColor3d(1, 0, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < ps->size(); i++){
			vec = (*ps)[i];
			glVertex2d(vec(1), vec(2));
		}
		glEnd();

		//画线
		glLineWidth(2);
		glColor3d(0, 1, 0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < ps->size(); i++){
			vec = (*ps)[i];
			glVertex2d(vec(1), vec(2));
		}
		glEnd();
	}
}

void drawControlPoints3D()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点

	vector<array<double, 3>> c = fac.get3DPoints();
	glBegin(GL_POINTS);
	glPointSize(5);
	glColor3d(1, 0, 0);
	for (int i = 0; i < c.size(); i++)
	{
		glVertex3d(c[i][0], c[i][1], c[i][2]);
	}
	glEnd();
}

void _drawControlPointsXY()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点
	for (int i = 0; i < fac.surfaceNum(); i++)
	{
		shared_ptr<BsplineSurface> c = fac.getSurface(i);
		for (int i = 0; i < c->getUSize(); i++)
		{
			shared_ptr<BsplineBase> tmp = c->getUi(i);
			shared_ptr<vector<NEWMAT::ColumnVector>> ps = tmp->getPoints();
			NEWMAT::ColumnVector vec;

			glPointSize(5);
			glColor3d(1, 0, 0);
			glBegin(GL_POINTS);
			for (int i = 0; i < ps->size(); i++){
				vec = (*ps)[i];
				glVertex2d(vec(1) / mainwin_wildth * subwin_wildth, vec(2) / mainwin_height * subwin_height + subwingap + subwin_height);
			}
			glEnd();

			//画线
			glLineWidth(2);
			glColor3d(0, 1, 0);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < ps->size(); i++){
				vec = (*ps)[i];
				glVertex2d(vec(1) / mainwin_wildth * subwin_wildth, vec(2) / mainwin_height * subwin_height + subwingap + subwin_height);
			}
			glEnd();
		}
	}
}
void drawControlPointsXY()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点

	vector<array<double, 3>> c = fac.get3DPoints();
	glBegin(GL_POINTS);
	glPointSize(5);
	glColor3d(1, 0, 0);
	for (int i = 0; i < c.size(); i++)
	{
		glVertex2d(c[i][0] / mainwin_wildth * subwin_wildth, c[i][1] / mainwin_height * subwin_height + subwingap + subwin_height);
	}
	glEnd();
}

void _drawControlPointsZ()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点
	for (int i = 0; i < fac.surfaceNum(); i++)
	{
		shared_ptr<BsplineSurface> c = fac.getSurface(i);
		for (int i = 0; i < c->getUSize(); i++)
		{
			shared_ptr<BsplineBase> tmp = c->getUi(i);
			shared_ptr<vector<NEWMAT::ColumnVector>> ps = tmp->getPoints();
			NEWMAT::ColumnVector vec;

			glPointSize(5);
			glColor3d(1, 0, 0);
			glBegin(GL_POINTS);
			for (int i = 0; i < ps->size(); i++){
				vec = (*ps)[i];
				glVertex2d(vec(2) / mainwin_wildth * subwin_wildth, vec(3) / mainwin_height * subwin_height + 2*subwingap + 2*subwin_height);
			}
			glEnd();

			//画线
			glLineWidth(2);
			glColor3d(0, 1, 0);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < ps->size(); i++){
				vec = (*ps)[i];
				glVertex2d(vec(2) / mainwin_wildth * subwin_wildth, vec(3) / mainwin_height * subwin_height + 2*subwingap + 2*subwin_height);
			}
			glEnd();
		}
	}
}

void drawControlPointsZ()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	//画点

	vector<array<double, 3>> c = fac.get3DPoints();
	glBegin(GL_POINTS);
	glPointSize(5);
	glColor3d(1, 0, 0);
	for (int i = 0; i < c.size(); i++)
	{
		glVertex2d(c[i][1] / mainwin_wildth * subwin_wildth, c[i][2] / mainwin_height * subwin_height + 2*subwingap + 2*subwin_height);
	}
	glEnd();
}
void drawCurve()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	glLineWidth(5);
	glColor3d(75.0 / 255, 0, 130.0 / 255);
	for (int i = 0; i < fac.curveNum(); i++)
	{
		shared_ptr<BsplineBase> c = fac.getCurve(i);
		glBegin(GL_LINE_STRIP);
		NEWMAT::Matrix pxy = c->getCurve();
		double x = 0, y = 0, z = 0;
		//cout << pxy.Nrows();
		for (int i = 0; i < pxy.Nrows(); i++)
		{
			x = (pxy)[i][0];
			y = (pxy)[i][1];
			glVertex2d(x, y);
		}
		glEnd();
	}

}

void drawSurface()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	glLineWidth(5);
	glColor3d(75.0 / 255, 0, 130.0 / 255);
	for (int i = 0; i < fac.surfaceNum(); i++)
	{
		shared_ptr<BsplineSurface> c = fac.getSurface(i);
		glBegin(GL_LINE_STRIP);
		NEWMAT::Matrix pxy = c->getSurface();
		double x = 0, y = 0, z = 0;
		//cout << pxy.Nrows();
		for (int i = 0; i < pxy.Nrows(); i++)
		{
			x = (pxy)[i][0];
			y = (pxy)[i][1];
			z = (pxy)[i][2];
			glVertex3d(x / mainwin_wildth, y / mainwin_height, z);
		}
		glEnd();
		//cout << c->getK() << '\t' << c->getCurveRes() << endl;
		//cout << pxy << endl;
	}

}

void subdrawAxis(int k)
{
	int n = 5;
	glLineWidth(1);
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i <= k; i++)
	{
		glVertex2d(0.0, subwin_height * (i + 1) / (k + 1));
		glVertex2d(subwin_wildth, subwin_height * (i + 1)/ (k + 1));
	}
	glEnd();
}
void subdrawNikus()
{
	int n = 5;
	glLineWidth(3);
	glColor3d(0.0, 0.0, 1.0);
	int num_of_u = 5;
	if (0)
	{
		for (int k = 0; k < curve_insert_ptr->getK(); k++)
		{
			for (int i = 0; i < curve_insert_ptr->getN() + 1; i++)
			{
				for (int u = 0; u <= num_of_u; u++)
				{
					double y = curve_insert_ptr->getNiku(i, k, u / num_of_u);
					cout << y << '\t';
					glVertex2d(u / num_of_u * subwin_wildth, y * subwin_height /  5);
				}
			}
		}
	}
	if (curve_insert_ptr)
	{
		int res = 20;
		NEWMAT::Matrix base = curve_insert_ptr->getBase(res);
		//cout << "\n"<< base.Nrows() << endl;
		if (base.Nrows())
		{
			const int n = curve_insert_ptr->getN() + 1;
			for (int i = 0; i < n; i++)
			{
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j < res; j++)
				{
					double x, y;
					glVertex2d(base[i * res + j][0] * subwin_wildth , subwin_height - base[i * res + j][1] * subwin_height);
					//cout << base[i * res + j][0] << '\t' << base[i * res + j][1] << endl;
				}
				glEnd();
				//cout << endl;
			}
		}
		
	}
}

void MouseCB(int _b, int _s, int _x, int _y)
{
	if (_s == GLUT_UP)
	{
		dis += ddis;
		azim += dazim;
		elev += delev;
		ddis = 0.0;
		dazim = 0.0;
		delev = 0.0;
		return;
	}

	if (_b == GLUT_RIGHT_BUTTON)
	{
		mode = 0;
		beginy = _y;
		return;
	}
	else
	{
		mode = 1;
		beginx = _x;
		beginy = _y;
	}
}
void subDisplay()
{
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT);    //清除颜色缓存和深度缓存
	subdrawAxis(5);
	subdrawNikus();
	glFlush();
}

void subDisplayXY()
{
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT);    //清除颜色缓存和深度缓存
	subdrawAxis(5);

	drawControlPointsXY();
	glFlush();
}

void subDisplayZ()
{
	glClear(GL_COLOR_BUFFER_BIT);    //清除颜色缓存和深度缓存
	subdrawAxis(5);
	drawControlPointsZ();
	glFlush();
}

void myDisplay()
{
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT);    //清除颜色缓存和深度缓存
	drawControlPoints();
	/*if (p.size() >= 4)
		for (int i = 0; i < p.size() - 3; i++)
			Bspline__(p[i], p[i + 1], p[i + 2], p[i + 3]);*/
	drawCurve();	
	glFlush();
}

void myDisplay3D()
{
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();  
	glTranslatef(0.0f + ratx, 0.0f + raty, -0);

	glRotated(elev+delev, 1.0, 0.0, 0.0);
	glRotated(azim+dazim, 0.0, 1.0, 0.0);
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT);    //清除颜色缓存和深度缓存
	drawControlPoints3D();

	drawSurface();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)		//ESC
		exit(0);
	if (key == 8)		//退格键
	{
		int index = getIndexNearByMouse(x, y);
		if (index == -1)
			return;
		p.erase(p.begin() + index);
		glutPostRedisplay();
	}
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		/*Point t(x, y);
		p.push_back(t);*/
		if (curve_insert_ptr)
		{
			curve_insert_ptr->setPi(x, y);
		}
		//glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mouseRightIsDown = true;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		mouseRightIsDown = false;
}

void submouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cout << x << '\t' << y << endl;
		glFlush();
	}
}

void mouseXY(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		BsplineFactory& fac = BsplineFactory::getFactory();
		fac.set3DPoints(1, x * mainwin_wildth /subwin_wildth, (y/* - subwingap - subwin_height*/) * mainwin_height / subwin_height);
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mouseRightIsDown = true;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		mouseRightIsDown = false;
}

void mouseZ(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		BsplineFactory& fac = BsplineFactory::getFactory();
		fac.set3DPoints(3, (y /*- subwingap * 2 - subwin_height * 2*/) * mainwin_height / subwin_height);
		glFlush();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mouseRightIsDown = true;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		mouseRightIsDown = false;
}
void motion(int x, int y)
{
	if (mouseRightIsDown)		//按住右键移动点
	{
		/*= getIndexNearByMouse(x, y);
		if (index == -1)
			return;
		p[index].x = x;
		p[index].y = y;*/
		curve_insert_ptr->changePi(x, y);
		glutPostRedisplay();
	}
}
void Reshape(int w, int h)      //两个参数：窗口被移动后大小
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initWindow(int &argc, char *argv[], int width, int height, char *title)    //初始化并显示到屏幕中央
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1);       //指定窗口位置
	glutInitWindowSize(width, height);       //指定窗口大小
	GLuint mainWin = glutCreateWindow(title);

	//GLuint subWIn1 = glutCreateSubWindow(mainWin, 0, 0, width / 5, height / 5);
	glClearColor(1, 215.0 / 255, 0, 0);
	glShadeModel(GL_FLAT);
}

void initWindow3D(int &argc, char *argv[], int width, int height, char *title)    //初始化并显示到屏幕中央
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1);       //指定窗口位置
	glutInitWindowSize(width, height);       //指定窗口大小
	GLuint mainWin = glutCreateWindow(title);

	//GLuint subWIn1 = glutCreateSubWindow(mainWin, 0, 0, width / 5, height / 5);
	glClearColor(1, 215.0 / 255, 0, 0);
	glShadeModel(GL_FLAT);
}

void setWindowFuncsAndShow(bool threeD /*= false*/)
{
	if (threeD)
	{	
		int x;
	initWindow3D(x, 0, mainwin_wildth, mainwin_height, "bsplinecurve3D");

	puts("\n\t鼠标在窗口点击绘制B样条曲线，鼠标右键移动控制点");
	puts("\t退格键（←）删除鼠标所在的点");
	//mainwin = ::FindWindow("FREEGLUT", "bsplinecurve");
	//cout << mainwin;
	glutDisplayFunc(myDisplay3D);
	glutReshapeFunc(Reshape);
	glutMouseFunc(submouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	GLuint mainWin = glutGetWindow();//xy coordinate
	GLuint subWIn1 = glutCreateSubWindow(mainWin, 0, 0, subwin_wildth, subwin_height);
	glClearColor(1, 1, 1, 0);
	glShadeModel(GL_FLAT);
	glutDisplayFunc(subDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(submouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	GLuint subWInxy = glutCreateSubWindow(mainWin, 0, subwin_height + subwingap, subwin_wildth, subwin_height);//z coordinate
	glClearColor(1, 1, 1, 0);
	glShadeModel(GL_FLAT);
	glutDisplayFunc(subDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouseXY);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	GLuint subWInyz = glutCreateSubWindow(mainWin, 0, subwin_height * 2 + subwingap * 2, subwin_wildth, subwin_height);
	glClearColor(1, 1, 1, 0);
	glShadeModel(GL_FLAT);
	glutDisplayFunc(subDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouseZ);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	}
	int x;
	initWindow(x, 0, mainwin_wildth, mainwin_height, "bsplinecurve");

	puts("\n\t鼠标在窗口点击绘制B样条曲线，鼠标右键移动控制点");
	puts("\t退格键（←）删除鼠标所在的点");
	//mainwin = ::FindWindow("FREEGLUT", "bsplinecurve");
	//cout << mainwin;
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	GLuint mainWin = glutGetWindow();
	GLuint subWIn1 = glutCreateSubWindow(mainWin, 0, 0, subwin_wildth, subwin_height);
	glClearColor(1, 1, 1, 0);
	glShadeModel(GL_FLAT);
	glutDisplayFunc(subDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(submouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}
