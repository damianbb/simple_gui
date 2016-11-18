#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QVector>

#include "netparser.hpp"
#include "addressdialog.hpp"
#include "dataeater.hpp"

using nlohmann::json;

struct peer_reference {
	std::string m_ipv4;
	int m_port;
	std::string m_ipv6;

	static peer_reference get_validated_ref(std::string ref);
};

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow,commandExecutor
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void startProgram(QStringList &l_params_list);

	void SavePeers(QString file_name);

	void showMsg(const json &msg) override;
	void askQuestion(const json &msg) override {}
	void showWorning(const json &msg) override {}
	void writeAns(const json &msg) override {}
	void addPeer(const json &msg) override {}
	void ping() override;

public slots:
	void onProcessInfo();
	void onProcessError();

	void addAddress(QString address);
	void addMsg();
	void showDebugPage(QByteArray &pageCode);

private slots:
	void on_connectButton_clicked();
	void on_plusButton_clicked();
	void onNewConnection();
	void on_minusButton_clicked();
	void on_actionDebug_triggered();

	void startConnection();
	void on_ping_clicked();

	void onReciveTcp();

private:


	void sendReciveTcp(QString &msg);

	QString my_ip;
	Ui::MainWindow *ui;
	QProcess *m_tunserver_process;
	addressDialog *m_dlg;
	QTcpSocket *m_socket;
	std::vector <peer_reference> m_peer_lst;
	//dataeater m_data_eater;  // replaced by simple_packet_eater but dataeater is still piece of good code.
	simple_packet_eater m_packet_eater;
	netParser m_parser;
};


#endif // MAINWINDOW_H
