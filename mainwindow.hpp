#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QVector>

#include <thread>
#include <atomic>
#include <chrono>

#include "netparser.hpp"
#include "addressdialog.hpp"
#include "dataeater.hpp"

using nlohmann::json;
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

	void startConnection();
	void on_ping_clicked();

	void onReciveTcp();
	void peerlist_request_slot();

private:
	std::shared_ptr<commandExecutor> m_cmd_exec;
	std::mutex m_mutex;
	std::atomic<bool> m_pr_call { false };
	std::unique_ptr<std::thread> th_peerlist;

	bool check_connection();

	void sendReciveTcp(QString &msg);
	void call_peerlist_requests(const std::chrono::seconds &time_interval = 5s);

	QString my_ip;
	Ui::MainWindow *ui;
	QProcess *m_tunserver_process;
	addressDialog *m_dlg;
	QTcpSocket *m_socket;
	std::vector <peer_reference> m_peer_lst;
	dataeater m_data_eater;
	netParser m_parser;

signals:
	void ask_for_peerlist();
};


#endif // MAINWINDOW_H
