#pragma once

//#include"stdafx.h"¡¡
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
	//shared_ptr<BsplineBase> curve = BsplineFactory::getFactory().createBspline(BsplineFactory::Quasi);
	//curve->setK(3);
	//curve->changeCurveRes(10);
	//changeInsertFocusTo(curve);
	BsplineFactory& fac = BsplineFactory::getFactory();
	fac.setKU(2);
	fac.setKV(2);
	fac.setCurveNumU(4);
	fac.setResU(5);
	fac.setResV(3);
	fac.setTypeU(BsplineFactory::Besier);
	fac.setTypeV(BsplineFactory::Uni);
	fac.set3DPoints(1, 110, 120);
	fac.set3DPoints(1, 90, 130);
	fac.set3DPoints(1, 114, 125);
	fac.set3DPoints(1, 102, 19);

	fac.set3DPoints(1, 80, 66);
	fac.set3DPoints(1, 50, 35);
	fac.set3DPoints(1, 10, 180);
	fac.set3DPoints(1, 99, 20);

	fac.set3DPoints(1, 72, 25);
	fac.set3DPoints(1, 92, 129);
	fac.set3DPoints(1, 80, 30);
	fac.set3DPoints(1, 54, 35);

	fac.set3DPoints(3, 10);
	fac.set3DPoints(3, 20);
	fac.set3DPoints(3, 15);
	fac.set3DPoints(3, 55);

	fac.set3DPoints(3, 23);
	fac.set3DPoints(3, 22);
	fac.set3DPoints(3, 10);
	fac.set3DPoints(3, 20);

	fac.set3DPoints(3, 15);
	fac.set3DPoints(3, 55);
	fac.set3DPoints(3, 23);
	fac.set3DPoints(3, 22);
	fac.genSuface();
	shared_ptr<BsplineSurface> surface = fac.getSurface();
	setWindowFuncsAndShow(true);
	return 0;

}
