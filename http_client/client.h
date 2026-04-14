#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDialog>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QElapsedTimer>
#include <QThread>
#include <QEventLoop>

class Client : public QDialog
{
	Q_OBJECT

public:
	explicit Client(const std::string& mode);
	~Client();

	void createDownloadDialog();
	bool validateUrl(const QString& url);
	void connections();	
	void downloadFileCurlMode(const QString& url);

private slots:
	void onUpdateProgressBar();
	void onDownloadFileThreadFinished();
	void onDownloadFileCreateThread();
	void onSetCode(int code);
	void onDownloadFileThreadStarted();

signals:
	void downloadFileThreadSetCode(int exitCode);


private:
	const std::string m_mode;

	QLineEdit* m_ldtAddress = nullptr;
	QLabel* m_lblAddress = nullptr;
	QPushButton* m_loadButton = nullptr;
	QProgressBar* m_progressBar = nullptr;
	QVBoxLayout* m_vBoxlayout = nullptr;
	QTimer* m_timer = nullptr;
	QElapsedTimer* m_elapsedTimer = nullptr;

	QThread* m_thread = nullptr;
	QEventLoop* m_eventLoop = nullptr;
	int m_exitCode = -1;
};
#endif // CLIENT_H
