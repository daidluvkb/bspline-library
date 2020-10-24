#pragma once

//#include"stdafx.h"　
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
//    // 清空颜色缓存
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // 开启顶点属性
//    glEnableVertexAttribArray(0);
//    // 绑定GL_ARRAY_BUFFER缓冲器
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // 告诉管线怎样解析bufer中的数据
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//    // 开始绘制几何图形(绘制一个点)
//    glDrawArrays(GL_LINES, 0, 3);
//	
//    //  禁用顶点数据
//    glDisableVertexAttribArray(0);
//
//    // 交换前后缓存
//    glutSwapBuffers();
//}
//void CreateVertexBuffer()
//{
//	//// 创建含有一个顶点的顶点数组
//	//Vector3f Vertices[1];
//	float vertices[] =   {
//		0.5f, 0.5f, 0.0f,   // 右上角
//		0.5f, -0.5f, 0.0f,  // 右下角
//		-0.5f, -0.5f, 0.0f, // 左下角
//		-0.5f, 0.5f, 0.0f   // 左上角
//	};
//	unsigned int indices[] = { // 注意索引从0开始! 
//		0, 1, 3, // 第一个三角形
//		1, 2, 3  // 第二个三角形
//	};
//	// 1. 绑定顶点数组对象
//	glBindVertexArray(VAO);
//	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	// 4. 设定顶点属性指针
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
//	//// 检查GLEW是否就绪，必须要在GLUT初始化之后！
//	//GLenum res = glewInit();
//	//if (res != GLEW_OK) {
//	//	fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//	//	return 1;
//	//}
//
//	//// 缓存清空后的颜色值
//	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//// 创建顶点缓冲器
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