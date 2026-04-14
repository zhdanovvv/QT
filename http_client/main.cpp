#include "client.h"

#include <QApplication>
#include <QMessageBox>
#include <QString>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if (argv[1] == nullptr || argc == 1 && argv[1] != "curl")
	{
		QMessageBox::information(nullptr, QString("http_client"),
			QString::fromWCharArray(L"Запуск приложения должен быть выполнен в командной строке\nаргументы командной строки - curl\nпример: http_client curl"));
		exit(1);
	}

	Client window(argv[1]);

	return app.exec();
}
