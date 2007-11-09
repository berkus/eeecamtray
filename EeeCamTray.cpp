#include "EeeCamTray.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTimer>
#include <QMenu>

EeeCamTray::EeeCamTray()
	: QSystemTrayIcon()
{
	enableAct = new QAction(QIcon(":/CameraOn.png"), tr("Enable camera"), this);
	connect(enableAct, SIGNAL(triggered()), this, SLOT(enableCamera()));
	disableAct = new QAction(QIcon(":/CameraOff.png"), tr("Disable camera"), this);
	connect(disableAct, SIGNAL(triggered()), this, SLOT(disableCamera()));
	quitAct = new QAction(QIcon(":/Quit.png"), tr("Quit"), this);
	quitAct->setShortcut(tr("Ctrl+Q"));
	connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));
	setContextMenu(new QMenu()); // checkStatus() will fill it in.

	checkStatus();
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkStatus()));
	timer->start(60*1000); // 60 seconds
}

/* once-a-minute slot */
void EeeCamTray::checkStatus()
{
	if (cameraIsEnabled())
	{
		setIcon(QIcon(":/CameraOn.png"));
		setToolTip(tr("Eee camera is enabled."));
		QMenu *menu = contextMenu();
		menu->clear();
		menu->addAction(disableAct);
		menu->addSeparator();
		menu->addAction(quitAct);
	}
	else
	{
		setIcon(QIcon(":/CameraOff.png"));
		setToolTip(tr("Eee camera is disabled."));
		QMenu *menu = contextMenu();
		menu->clear();
		menu->addAction(enableAct);
		menu->addSeparator();
		menu->addAction(quitAct);
	}
}

bool EeeCamTray::cameraIsEnabled()
{
	qDebug() << "Trying to read camera status.";
	QFile ioctl("/proc/acpi/asus/camera");
	if (ioctl.open(QIODevice::ReadOnly|QIODevice::Text|QIODevice::Unbuffered))
	{
		char c;
		if (ioctl.getChar(&c))
			return c == '1' ? true : false;
		else
			qDebug() << "** enabling write failed.";
		ioctl.close();
	}
	else
		qDebug() << "** failed to open control file.";
	return false; // Can't tell, assume worst.
}

void EeeCamTray::enableCamera()
{
	qDebug() << "Trying to enable camera input.";
	QFile ioctl("/proc/acpi/asus/camera");
	if (ioctl.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Unbuffered))
	{
		if (ioctl.putChar('1'))
			qDebug() << "** enabling write successful.";
		else
			qDebug() << "** enabling write failed.";
		ioctl.close();
	}
	else
		qDebug() << "** failed to open control file.";

	checkStatus();
}

void EeeCamTray::disableCamera()
{
	qDebug() << "Trying to disable camera input.";
	QFile ioctl("/proc/acpi/asus/camera");
	if (ioctl.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Unbuffered))
	{
		if (ioctl.putChar('0'))
			qDebug() << "** disabling write successful.";
		else
			qDebug() << "** disabling write failed.";
		ioctl.close();
	}
	else
		qDebug() << "** failed to open control file.";

	checkStatus();
}
