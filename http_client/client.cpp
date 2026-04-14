#include "client.h"

#include <curl/curl.h>

#include <QCoreApplication>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QStyleFactory>
#include <QFileInfo>
#include <QGroupBox>
#include <QUrl>
#include <QFile>

#include <fstream>
#include <stdio.h>
#include <iostream>

Client::Client(const std::string& mode) : m_mode(mode)
{
	createDownloadDialog();
}

Client::~Client()
{
	if (m_elapsedTimer != nullptr)
		delete m_elapsedTimer;
}

void Client::createDownloadDialog()
{
	setWindowTitle(QString("http(s)_client") + QString::fromWCharArray(L" (Режим ") + QString::fromStdString(this->m_mode) + ")");
	Qt::WindowFlags flags = windowFlags();
	flags &= ~Qt::WindowMaximizeButtonHint;
	flags &= ~Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowStaysOnTopHint;
	this->setWindowFlags(flags);

	if (m_ldtAddress == nullptr)
		m_ldtAddress = new QLineEdit(this);

	if (m_lblAddress == nullptr)
		m_lblAddress = new QLabel(this);

	m_lblAddress->setText(QString::fromWCharArray(L"Введите адрес сервера\nдля загрузки .exe файла"));

	QHBoxLayout* hBoxlayout = new QHBoxLayout(this);
	hBoxlayout->addWidget(m_lblAddress);
	hBoxlayout->addWidget(m_ldtAddress, 1);
	QWidget* hBoxWidget = new QWidget(this);
	hBoxWidget->setLayout(hBoxlayout);

	if (m_loadButton == nullptr)
		m_loadButton = new QPushButton("Загрузить файл", this);

	QHBoxLayout* hButtonlayout = new QHBoxLayout(this);
	hButtonlayout->addWidget(m_loadButton, -1, Qt::AlignRight);
	QWidget* hButtonWidget = new QWidget(this);
	hButtonWidget->setLayout(hButtonlayout);

	if (m_progressBar == nullptr)
		m_progressBar = new QProgressBar(this);

	m_progressBar->setFixedHeight(20);
	m_progressBar->setStyle(QStyleFactory::create("fusion"));
	m_progressBar->setRange(0, 100);
	m_progressBar->setValue(0);
	m_progressBar->setOrientation(Qt::Horizontal);
	m_progressBar->setAlignment(Qt::AlignCenter);

	QHBoxLayout* hProgBarLayout = new QHBoxLayout(this);
	hProgBarLayout->addWidget(m_progressBar);
	QWidget* hProgBarWidget = new QWidget(this);
	hProgBarWidget->setLayout(hProgBarLayout);
	m_progressBar->hide();

	if (m_vBoxlayout == nullptr)
		m_vBoxlayout = new QVBoxLayout(this);

	m_vBoxlayout->addWidget(hBoxWidget);
	m_vBoxlayout->addWidget(hButtonWidget);
	m_vBoxlayout->addWidget(hProgBarWidget);
	m_vBoxlayout->setSpacing(0);

	QGroupBox* mainGbx = new QGroupBox(this);
	mainGbx->setLayout(m_vBoxlayout);
	QHBoxLayout* mainGbxHboxLayout = new QHBoxLayout(this);
	mainGbxHboxLayout->addWidget(mainGbx);
	mainGbx->setStyleSheet("QGroupBox {""border: 1px solid grey;""}");

	setLayout(mainGbxHboxLayout);

	adjustSize();

	setMinimumHeight(height());
	setMaximumHeight(height() + m_progressBar->height());
	setFixedWidth(width() + 100);

	if (m_timer == nullptr)
		m_timer = new QTimer(this);

	if (m_elapsedTimer == nullptr)
		m_elapsedTimer = new QElapsedTimer();

	connections();

	show();
}

bool Client::validateUrl(const QString& url)
{
	QUrl checkUrl;
	checkUrl.setUrl(url, QUrl::StrictMode);

	QString scheme = checkUrl.scheme().toLower();

	if (scheme.isEmpty() || !scheme.isEmpty() && scheme != "https")
	{
		QMessageBox::information(this, QString("http(s)_client"), QString("Enter https://"));
		return false;
	}

	return true;
}

void Client::onUpdateProgressBar()
{
	m_progressBar->setValue(m_elapsedTimer->elapsed() * 0.005);
}

void Client::onDownloadFileCreateThread()
{
	const QString url = m_ldtAddress->text();
	if (!validateUrl(url) == true)
	{
		return;
	}

	m_loadButton->setEnabled(false);
	m_ldtAddress->setEnabled(false);

	m_progressBar->show();

	adjustSize();

	if (m_thread == nullptr && m_mode == "curl")
		m_thread = QThread::create(&Client::downloadFileCurlMode, this, url);

	if (m_eventLoop == nullptr)
		m_eventLoop = new QEventLoop(m_thread);

	connect(m_thread, &QThread::started, this, &Client::onDownloadFileThreadStarted);

	connect(m_thread, &QThread::finished, this, &Client::onDownloadFileThreadFinished);

	m_progressBar->setValue(0);
	m_elapsedTimer->restart();
	m_timer->start(100);

	m_thread->start();
}

void Client::onSetCode(int code)
{
	m_exitCode = code;
}

void Client::onDownloadFileThreadStarted()
{
	if (m_eventLoop != nullptr)
		m_eventLoop->exec();

	if (m_thread != nullptr)
		m_thread->wait();
}

void Client::onDownloadFileThreadFinished()
{
	m_timer->stop();

	m_thread = nullptr;
	m_eventLoop = nullptr;

	if (m_exitCode == int(CURLE_OK))
	{
		m_progressBar->setValue(100);
		QMessageBox::information(this, QString("http(s)_client"), QString::fromWCharArray(L"Файл file.exe загружен в папку download"));

		m_ldtAddress->setEnabled(true);
		m_loadButton->setEnabled(true);

		m_progressBar->hide();

		QTimer::singleShot(10, [this]()
		{
			adjustSize();
		});
	}
	if (m_exitCode != int(CURLE_OK))
	{
		if (m_mode == "curl" && m_exitCode != 404)
		{
			m_progressBar->setValue(100);
			QString errorMessage = QString::fromStdString(curl_easy_strerror(CURLcode(m_exitCode)));
			QMessageBox::information(this, QString("http(s)_client"), errorMessage);
			close();
		}

		if (m_mode == "curl" && m_exitCode == 404)
		{
			m_progressBar->setValue(100);

			QFileInfo fileInfo(QCoreApplication::applicationFilePath());
			const QString resourcesFilePath(fileInfo.absolutePath() + "/resources/file.exe");
			const QString downloadDirFilePath(fileInfo.absolutePath() + "/download/file.exe");

			if (QFile::exists(resourcesFilePath))
				QFile::copy(resourcesFilePath, downloadDirFilePath);

			if(QFile::exists(downloadDirFilePath) == true)
				QMessageBox::information(this, QString("http(s)_client"), QString::fromWCharArray(L"Файл file.exe не найден на сервере и был перемещен\nиз папки /resources в папку /download"));

			close();
		}
	}
}

void Client::connections()
{
	connect(m_timer, &QTimer::timeout, this, &Client::onUpdateProgressBar);

	connect(m_loadButton, &QPushButton::clicked, this, &Client::onDownloadFileCreateThread);

	connect(this, &Client::downloadFileThreadSetCode, this, &Client::onSetCode);
}

void Client::downloadFileCurlMode(const QString& url)
{
	const std::string urlServer = url.toStdString();

	CURL* curl = curl_easy_init();

	if (curl == NULL)
	{
		QMetaObject::invokeMethod(this, "downloadFileThreadSetCode", Qt::BlockingQueuedConnection, Q_ARG(int, CURLE_FAILED_INIT));
		m_thread->exit(int(CURLE_FAILED_INIT));
		m_thread->quit();
	}

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

	curl_easy_setopt(curl, CURLOPT_URL, urlServer.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);

	QFileInfo fileInfo(QCoreApplication::applicationFilePath());
	std::string filePath = fileInfo.absolutePath().toStdString();
	FILE* fileToDownload = fopen(filePath.append("/download/file.exe").c_str(), "wb");

	CURLcode resultCode;
	long statusCode = 0;

	if (fileToDownload)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileToDownload);

		resultCode = curl_easy_perform(curl);
	}

	if (resultCode != CURLE_OK)
	{
		m_thread->msleep(500);

		resultCode = curl_easy_perform(curl);

		if (resultCode != CURLE_OK)
		{
			fclose(fileToDownload);

			curl_easy_cleanup(curl);

			QMetaObject::invokeMethod(this, "downloadFileThreadSetCode", Qt::BlockingQueuedConnection, Q_ARG(int, int(resultCode)));
			m_thread->exit(int(resultCode));
			m_thread->quit();
		}
	}

	if (resultCode == CURLE_OK)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

		if (statusCode == 404)
		{
			fclose(fileToDownload);

			curl_easy_cleanup(curl);

			QMetaObject::invokeMethod(this, "downloadFileThreadSetCode", Qt::BlockingQueuedConnection, Q_ARG(int, int(statusCode)));
			m_thread->exit(int(statusCode));
			m_thread->quit();
		}

		if (statusCode == 200)
		{
			fclose(fileToDownload);

			curl_easy_cleanup(curl);

			QMetaObject::invokeMethod(this, "downloadFileThreadSetCode", Qt::BlockingQueuedConnection, Q_ARG(int, int(CURLE_OK)));
			m_thread->exit(int(CURLE_OK));
			m_thread->quit();
		}
	}
}

