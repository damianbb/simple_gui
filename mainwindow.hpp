#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QVector>

#include "netparser.hpp"
#include "addressdialog.hpp"
#include "dataeater.hpp"


struct peer_reference {
	std::string m_ipv4;
	int m_port;
	std::string m_ipv6;

	static peer_reference get_validated_ref(std::string ref);
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,commandExecutor
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void startProgram(QStringList &l_params_list);

	void SavePeers(QString file_name);

	virtual void showMsg(const nlohmann::json &msg);
	virtual void askQuestion(const nlohmann::json &msg) {}
	virtual void showWorning(const nlohmann::json &msg) {}
	virtual void writeAns(const nlohmann::json &msg) {}
	virtual void addPeer(const nlohmann::json &msg) {}

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
private:


	void sendReciveTcp(QString &msg);
	QString my_ip;
	Ui::MainWindow *ui;
	QProcess *m_tunserver_process;
	addressDialog *m_dlg;
	QTcpSocket *m_socket;
	std::vector <peer_reference> m_peer_lst;
	dataeater m_data_eater;
	netParser m_parser;

	void onReciveTcp();


};


#endif // MAINWINDOW_H
