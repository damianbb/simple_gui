#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QVector>

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "netparser.hpp"
#include "addressdialog.hpp"
#include "dataeater.hpp"
#include "commandexecutor.hpp"

using namespace std::chrono_literals;

struct peer_reference {
	std::string m_ipv4;
	int m_port;
	std::string m_ipv6;

	static peer_reference get_validated_ref(std::string ref);
};

namespace Ui {
class MainWindow;
}

class commandExecutor;

class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void startProgram(QStringList &l_params_list);

	void SavePeers(QString file_name);

public slots:
	void onProcessInfo();
	void onProcessError();

	void addAddress(QString address);
	void showDebugPage(QByteArray &pageCode);


private slots:
	void on_connectButton_clicked();
	void on_plusButton_clicked();
	void on_minusButton_clicked();
	void on_actionDebug_triggered();

	void on_ping_clicked();

	void peerlist_request_slot();

private:
	std::shared_ptr<commandExecutor> m_cmd_exec;

	Ui::MainWindow *ui;
	QProcess *m_tunserver_process;
	addressDialog *m_dlg;
	std::vector <peer_reference> m_peer_lst;

signals:
	void ask_for_peerlist();
};


#endif // MAINWINDOW_H
