#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w = MainWindow::create();
	w.show();

	return a.exec();
}
