#include <QApplication>
#include "EeeCamTray.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	EeeCamTray tray;
	tray.show();
	return app.exec();
}
