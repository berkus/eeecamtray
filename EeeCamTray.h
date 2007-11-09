#ifndef EEECAMTRAY_H
#define EEECAMTRAY_H

#include <QSystemTrayIcon>
class QAction;

class EeeCamTray : public QSystemTrayIcon
{
	Q_OBJECT

	public:
		EeeCamTray();

	private Q_SLOTS:
		void checkStatus();
		void enableCamera();
		void disableCamera();

	private:
		bool cameraIsEnabled();

		QAction *enableAct, *disableAct, *quitAct;
};

#endif
