#include "bsplineui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	bsplineui w;
	w.show();
	return a.exec();
}
