#pragma once

//#include"stdafx.h"��
#include "evaluator.h"
//#include "bspline.h"

#include "display.h"
#include <conio.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

//#include <GL/glut.h>


using namespace std;
using namespace bspline;
using namespace NEWMAT;

int main(int argc, char *argv[])
{
	//UniBspline ub1 (1, 5, 5, 1);
	//UniBspline ub2 (2, 5, 5, 1);
	//UniBspline ub3 (3, 5, 5, 1);
	//UniBspline ub4 (4, 5, 5, 1);
	

	HWND mw;
	setWindowFuncsAndShow(mw);
	char a;
	cin >> a;
	return 0;

	//WId mainwin = (WId)::FindWindow("FREEGLUT", "bsplinecurve");
}

//GLuint VBO;
//GLuint VAO;
//GLuint EBO;
//
//void RenderScenceCB(){
//	//glUseProgram(shaderProgram);
//	glBindVertexArray(VAO);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);  
//    // �����ɫ����
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // ������������
//    glEnableVertexAttribArray(0);
//    // ��GL_ARRAY_BUFFER������
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // ���߹�����������bufer�е�����
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//    // ��ʼ���Ƽ���ͼ��(����һ����)
//    glDrawArrays(GL_LINES, 0, 3);
//	
//    //  ���ö�������
//    glDisableVertexAttribArray(0);
//
//    // ����ǰ�󻺴�
//    glutSwapBuffers();
//}
//void CreateVertexBuffer()
//{
//	//// ��������һ������Ķ�������
//	//Vector3f Vertices[1];
//	float vertices[] =   {
//		0.5f, 0.5f, 0.0f,   // ���Ͻ�
//		0.5f, -0.5f, 0.0f,  // ���½�
//		-0.5f, -0.5f, 0.0f, // ���½�
//		-0.5f, 0.5f, 0.0f   // ���Ͻ�
//	};
//	unsigned int indices[] = { // ע��������0��ʼ! 
//		0, 1, 3, // ��һ��������
//		1, 2, 3  // �ڶ���������
//	};
//	// 1. �󶨶����������
//	glBindVertexArray(VAO);
//	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	// 4. �趨��������ָ��
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//}
//int main(int argc, char *argv[])
//{
//	//glutInit(&argc, argv);
//	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
//	//glutInitWindowPosition(100, 100);
//	//glutInitWindowSize(400, 400);
//	//glutCreateWindow("openGL program");
//	////glutDisplayFunc(&RenderScenceCB);
//	//glutDisplayFunc(&display);
//
//
//
//	//// ���GLEW�Ƿ����������Ҫ��GLUT��ʼ��֮��
//	//GLenum res = glewInit();
//	//if (res != GLEW_OK) {
//	//	fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//	//	return 1;
//	//}
//
//	//// ������պ����ɫֵ
//	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//// �������㻺����
//	//CreateVertexBuffer();
//
//	//glutMainLoop();
//	//cout << Cmn(6, 4);
//	//UniBspline b(2, 5, 1, 0);
//	NEWMAT::Real A[] = {3, 4};
//	RowVector v(3);
//	v << A;
//	cout << v << endl;
//	cout << norm(v);
//	char a;
//	cin >> a;
//	return 0;
//}