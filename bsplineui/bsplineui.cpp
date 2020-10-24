#include "bsplineui.h"
#include <display.h>
#include <windows.h>
#include <BsplineFactory.h>

bsplineui::bsplineui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lblCurveRes->setText(QString::number(ui.curveRes->value()));
}

bsplineui::~bsplineui()
{

}


void bsplineui::enableParametricMethod(bool enable)
{
	ui.rbtHartleyJudd->setEnabled(enable);
	ui.rbtRiesenfeld->setEnabled(enable);
}

void bsplineui::showParametricMethod(bool hartley)
{
	ui.rbtHartleyJudd->setChecked(hartley);
	ui.rbtRiesenfeld->setChecked(!hartley);
}

void bsplineui::loadGlWindow3D()
{
	setWindowFuncsAndShow(true);
	return;
}

void bsplineui::setUV()
{
	static int cnt = -1;
	cnt ++;
	BsplineFactory& fac = BsplineFactory::getFactory();
	if (cnt)
	{
		fac.setKV(ui.spbxCurveK->value());
		if (ui.cbxUnibspl->isChecked())
		{
			ui.lstCurves->addItem("Uni");
			fac.setTypeV(BsplineFactory::Uni);
		}
		else if (ui.cbxQuasibspl->isChecked())
		{
			ui.lstCurves->addItem("Quasi");
			fac.setTypeV(BsplineFactory::Quasi);
		}
		else if (ui.cbxBezierbspl->isChecked())
		{
			ui.lstCurves->addItem("Besier");
			fac.setTypeV(BsplineFactory::Besier);
		}
		else if (ui.cbxNunibspl->isChecked())
		{
			ui.lstCurves->addItem("Nuni");
			fac.setTypeV(BsplineFactory::Nuni);
		}
	}
	else
	{
		fac.setKU(ui.spbxCurveK->value());
		fac.setPointNumU(ui.spbxSurfaceNumU->value());
		if (ui.cbxUnibspl->isChecked())
		{
			ui.lstCurves->addItem("Uni");
			fac.setTypeU(BsplineFactory::Uni);
		}
		else if (ui.cbxQuasibspl->isChecked())
		{
			ui.lstCurves->addItem("Quasi");
			fac.setTypeU(BsplineFactory::Quasi);
		}
		else if (ui.cbxBezierbspl->isChecked())
		{
			ui.lstCurves->addItem("Besier");
			fac.setTypeU(BsplineFactory::Besier);
		}
		else if (ui.cbxNunibspl->isChecked())
		{
			ui.lstCurves->addItem("Nuni");
			fac.setTypeU(BsplineFactory::Nuni);
		}
	}

}

void bsplineui::createSurface()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	fac.genSuface();
}

void bsplineui::loadGlWindow()
{
	setWindowFuncsAndShow(false);
	return;
	HWND hwnd;
	/*GlThread * glwin = new GlThread(hwnd);
	glwin->start();*/
	//HWND zuozhi;
	//setWindowFuncsAndShow(zuozhi);
	WId mainwin = (WId)::FindWindow(L"FREEGLUT", L"bsplinecurve");
	//WId mainwin = (WId)::FindWindow(L"CalcFrame", NULL);

	QWindow* native_wnd;
	native_wnd = QWindow::fromWinId(mainwin);
	native_wnd->setFlags(native_wnd->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	
	//QWidget* native_wdgt = QWidget::createWindowContainer(native_wnd, this, Qt::SubWindow);

	//QWidget* native_wdgt = QWidget::createWindowContainer(native_wnd, ui.widget);

	//ui.verticalLayoutGl->addWidget(native_wdgt);
	//native_wdgt->setMinimumSize(400, 300);
}



void bsplineui::changeParametricMethod()
{
	if (ui.lstCurves->currentItem()->text() == "Nuni")
	{
		BsplineFactory& fac = BsplineFactory::getFactory();
		shared_ptr<NuniBspline> nc = dynamic_pointer_cast<NuniBspline>(fac.getCurve(ui.lstCurves->currentRow()));
		nc->setParametricMethod(ui.rbtHartleyJudd->isChecked());
	}
}

void bsplineui::createCurve()
{
	BsplineFactory& fac = BsplineFactory::getFactory();
	shared_ptr<BsplineBase> newcurve;
	if (ui.cbxUnibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Uni);
		ui.lstCurves->addItem("Uni");
	}
	else if (ui.cbxQuasibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Quasi);
		QListWidgetItem it("Quasi");
		ui.lstCurves->addItem("Quasi");
	}
	else if (ui.cbxBezierbspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Besier);
		ui.lstCurves->addItem("Besier");
	}
	else if (ui.cbxNunibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Nuni);
		ui.lstCurves->addItem("Nuni");
	}
	if (newcurve)
	{
		newcurve->setK(ui.spbxCurveK->value());
		newcurve->changeCurveRes(ui.curveRes->value());
	}
	int n = ui.lstCurves->count();
	if (n > 0)
	{		
		ui.lstCurves->setCurrentItem(ui.lstCurves->item(n - 1));
	}
}


void bsplineui::copyCurve()
{	
	QListWidgetItem* currentit = ui.lstCurves->currentItem();
	int oldind = ui.lstCurves->row(currentit);
	BsplineFactory& fac = BsplineFactory::getFactory();
	if (oldind < 0 || oldind >= fac.curveNum())
	{
		return;
	}
	shared_ptr<BsplineBase> newcurve = fac.getCurve(oldind);
	if (ui.cbxUnibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Uni, oldind);
		ui.lstCurves->addItem("Uni");
	}
	else if (ui.cbxQuasibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Quasi, oldind);
		ui.lstCurves->addItem("Quasi");
	}
	else if (ui.cbxBezierbspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Besier, oldind);
		ui.lstCurves->addItem("Besier");
	}
	else if (ui.cbxNunibspl->isChecked())
	{
		newcurve = fac.createBspline(BsplineFactory::Nuni, oldind);
		ui.lstCurves->addItem("Nuni");
	}
	if (newcurve)
	{
		newcurve->setK(ui.spbxCurveK->value());
		newcurve->changeCurveRes(ui.curveRes->value());
		newcurve->updateCurve();
	}
	int n = ui.lstCurves->count();
	if (n > 0)
	{		
		ui.lstCurves->setCurrentItem(ui.lstCurves->item(n - 1));
	}
}

void bsplineui::changeInsertFocus()
{
	QListWidgetItem* currentit = ui.lstCurves->currentItem();
	int ind = ui.lstCurves->row(currentit);

	if(ind >= 0)
	{	
		BsplineFactory& factory = BsplineFactory::getFactory();
		shared_ptr<BsplineBase> bspl = factory.getCurve(ind);
		changeInsertFocusTo(bspl);
		ui.curveRes->setValue(bspl->getCurveRes());
		QString _type = currentit->text();
		char Head = _type[0].unicode();
		switch (Head)
		{
		case 'U':
			enableParametricMethod(false);
			break;
		case 'Q':
			enableParametricMethod(false);
			break;
		case 'B':
			enableParametricMethod(false);
			break;
		case 'N':
			enableParametricMethod(true);
			shared_ptr<NuniBspline> nbspl = dynamic_pointer_cast<NuniBspline>(bspl);
			showParametricMethod(nbspl->getParametricMethod());
			break;
		}
	}
	else
	{
		changeInsertFocusTo(nullptr);
	}
}

void bsplineui::on_cbxUnibspl_clicked()
{
	if (ui.cbxUnibspl->isChecked())
	{
		ui.cbxQuasibspl->setChecked(false);
		ui.cbxBezierbspl->setChecked(false);
		ui.cbxNunibspl->setChecked(false);
	}
}

void bsplineui::on_cbxQuasibspl_clicked()
{
	if (ui.cbxQuasibspl->isChecked())
	{
		ui.cbxUnibspl->setChecked(false);
		ui.cbxBezierbspl->setChecked(false);
		ui.cbxNunibspl->setChecked(false);
	}
}

void bsplineui::on_cbxBezierbspl_clicked()
{

	if (ui.cbxBezierbspl->isChecked())
	{
		ui.cbxUnibspl->setChecked(false);
		ui.cbxQuasibspl->setChecked(false);
		ui.cbxNunibspl->setChecked(false);
	}
}

void bsplineui::on_cbxNunibspl_clicked()
{
	if (ui.cbxNunibspl->isChecked())
	{
		ui.cbxUnibspl->setChecked(false);
		ui.cbxQuasibspl->setChecked(false);
		ui.cbxBezierbspl->setChecked(false);
	}
}

void bsplineui::curveRes_valuechanged(int v)
{ 
	int curveind = ui.lstCurves->currentRow();
	if (curveind >= 0)
	{
		ui.lblCurveRes->setText(QString::number(ui.curveRes->value()));
		BsplineFactory& fac = BsplineFactory::getFactory();
		fac.changeCurveRes(curveind, ui.curveRes->value());
	}
}

GlThread::GlThread(HWND& glwin)
{
	_hwnd = &glwin;
}

void GlThread::run()
{
	setWindowFuncsAndShow(*_hwnd);
}
