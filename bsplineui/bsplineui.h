#ifndef BSPLINEUI_H
#define BSPLINEUI_H

#include <QtWidgets/QMainWindow>
#include <QWindow>
#include <QThread>
#include "ui_bsplineui.h"

class bsplineui : public QMainWindow
{
	Q_OBJECT

public:
	bsplineui(QWidget *parent = 0);
	~bsplineui();
protected:

private:
	Ui::bsplineuiClass ui;
private:
	void enableParametricMethod(bool enable);
	void showParametricMethod(bool hartley);
	private slots:
		void loadGlWindow3D();
		void setUV();
		void createSurface();
		void loadGlWindow();
		void changeParametricMethod();
		void createCurve();
		void copyCurve();
		void changeInsertFocus();
		void on_cbxUnibspl_clicked();
		void on_cbxQuasibspl_clicked();
		void on_cbxBezierbspl_clicked();
		void on_cbxNunibspl_clicked();
		void curveRes_valuechanged(int v);
};

class GlThread : public QThread
{
public:
	GlThread(HWND& glwin);
protected:
	void run();
private:
	HWND* _hwnd;
};
#endif // BSPLINEUI_H
