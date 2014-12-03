#include "mainwndctrlpannel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainwndCtrlPannel w;
	w.show();

	return a.exec();
}
