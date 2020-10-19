#pragma once
#include <iostream>
#include <glew.h>
#include <gl/glut.h>
#include <math.h>
#include <windows.h>
#include <vector>
#include <algorithm>

#include "bspline.h"
#include "BsplineFactory.h"


using namespace std;
extern bool mouseRightIsDown;
struct Point;
extern vector<Point> p;
//extern bspline::NuniBspline curve;
void changeInsertFocusTo(const shared_ptr<BsplineBase>& curve);
double getRatio(double t,double a,double b,double c,double d);
double caculateSquarDistance(Point a, Point b);
int getIndexNearByMouse(int x, int y);
void Bspline__(Point a,Point b,Point c,Point d);
void drawControlPoints();
void drawControlPoints3D();
void drawCurve();
void drawSurface();
void MouseCB(int _b, int _s, int _x, int _y);
void subDisplayXY();
void subDisplayZ();
void myDisplay();
void myDisplay3D();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseXY(int button, int state, int x, int y);
void mouseZ(int button, int state, int x, int y);
void motion(int x, int y);
void Reshape(int w, int h);      //�������������ڱ��ƶ����С
void initWindow(int &argc, char *argv[], int width, int height, char *title);    //��ʼ������ʾ����Ļ����

void setWindowFuncsAndShow(bool threeD);

