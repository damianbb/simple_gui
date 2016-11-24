#include <QDebug>
#include <QProcess>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFileDialog>
#include <QFormLayout>

#include <regex>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "addressdialog.hpp"
#include "paramscontainer.hpp"
#include "dataeater.hpp"
#include "debugdialog.hpp"
#include "get_host_info.hpp"

#include "trivialserialize.hpp"
#include <boost/asio.hpp>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_tunserver_process(nullptr),
	m_dlg(nullptr)
{
	ui->setupUi(this);

	ParamsContainer params;
	params.readParams("peers.json");
	m_peer_lst = params.getPeerList();

	for(auto it : m_peer_lst){
		QString peer_val = QString::fromStdString(it.m_ipv6);
		ui->peerListWidget->addItem(peer_val);
	}

	std::vector <char> testData;

	std::string tst_msg ("{\"id\" : 1,\"cmd\" : \"info\",\"topic\":\"info1\",\"msg\":{\"text\":\"newmessage\"} }");
	testData.push_back(0xff);
	testData.push_back(tst_msg.size()+3);
	testData.push_back(0);
	for (auto it :tst_msg) {
		testData.push_back(it);
	}

/*
	dataeater eater;
	eater.eat(testData);
	eater.process();
	std::string msg = eater.getLastCommand();

//	std::string tst_msg ("{\"id\" : 1,\"cmd\" : \"info\",\"topic\":\"info1\",\"msg\":{\"text\":\"newmessage\"} }");

		netParser parser(*this);
		parser.parseMsg(msg);

	execNextOrder();
*/
}

std::shared_ptr<MainWindow> MainWindow::create_shared_ptr() {
	// TODO
	std::shared_ptr<MainWindow> ret(new MainWindow);// = std::make_shared<MainWindow>();
	ret->m_cmd_exec = commandExecutor::construct(ret);
	return ret;
}

MainWindow::MainWindow(MainWindow &&other) {
	if (this == &other)
		return;
	m_cmd_exec = std::move(other.m_cmd_exec);
	ui = other.ui;
	other.ui = nullptr;
	m_tunserver_process = other.m_tunserver_process;
	other.m_tunserver_process = nullptr;
	m_dlg = other.m_dlg;
	other.m_dlg = nullptr;
	m_peer_lst = std::move(other.m_peer_lst);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete m_tunserver_process;
	delete m_dlg;
}

peer_reference peer_reference::get_validated_ref(std::string ref) {
	std::string r_ipv4_port;
	std::string r_ipv4;
	std::string r_port;
	std::string r_ipv6;

	size_t pos1;
	if ((pos1 = ref.find('-')) != std::string::npos) {
		r_ipv4_port = ref.substr(0,pos1);
		r_ipv6 = ref.substr(pos1+1);

		std::regex pattern("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}:\\d{1,5}"); // i.e. 127.0.0.1:4562
		std::smatch result;
		qDebug()<<r_ipv4_port.c_str();
		if (std::regex_search(r_ipv4_port, result, pattern)) {
			size_t pos2 = r_ipv4_port.find(':');
			r_ipv4 = r_ipv4_port.substr(0, pos2);
			r_port = r_ipv4_port.substr(pos2+1);
		} else
			throw std::invalid_argument("bad format of input remote address and port");

		boost::system::error_code ec;
		// validate ipv4
		boost::asio::ip::address_v4::from_string(r_ipv4, ec);
		if (ec)
			throw std::invalid_argument("bad format of input remote IPv4 address");
		// validate ipv6
		boost::asio::ip::address_v6::from_string(r_ipv6, ec);
		if(ec)
			throw std::invalid_argument("bad format of input remote IPv6 address");

	} else
		throw std::invalid_argument("bad format of input ref - missing '-'");
	return {r_ipv4 ,stoi(r_port), r_ipv6};
}

void MainWindow::add_address(QString address)
{
	qDebug()<< "add address [" << address << ']';
	try {
		peer_reference peer_ref = peer_reference::get_validated_ref(address.toStdString());
		m_peer_lst.push_back(peer_ref);
		ui->peerListWidget->addItem(address);

	} catch (std::exception &er) {
		qDebug()<< er.what();
	} catch (...) {
		qDebug()<< "fail to parse address - bad format";
	}

}


void MainWindow::showDebugPage(QByteArray &pageCode) {

}

void MainWindow::start_tunserver(std::vector<peer_reference> &peer_list, const QString &tunserver_path) {

	QStringList params_list;
	for (const auto &peer : peer_list) {
		std::string peer_string = " --peer ";
		peer_string += peer.m_ipv4 + ":" + std::to_string(peer.m_port);
		peer_string += "-" + peer.m_ipv6;
		params_list.push_back(QString::fromStdString(peer_string));
	}

	qDebug()<<params_list;
	QProcess *m_tunserver_process = new QProcess(this);
	m_tunserver_process->start(tunserver_path , params_list);

	if(m_tunserver_process->state() == QProcess::NotRunning)
		throw std::runtime_error("Fail to run tunserver process");
}

void MainWindow::onProcessInfo() {

	qDebug()<<m_tunserver_process->readAll();
}

void MainWindow::onProcessError() {

	qDebug()<<m_tunserver_process->readAll();
}

void MainWindow::on_plusButton_clicked() {

	m_dlg = new addressDialog(this);
	connect (m_dlg,SIGNAL(add_address(QString)),this,SLOT(add_address(QString)));
	m_dlg->show();
}

void MainWindow::on_minusButton_clicked() {
	auto delete_list= ui->peerListWidget->selectedItems();
	try{
		qDebug()<<delete_list.at(0);
		ui->peerListWidget->removeItemWidget(delete_list.at(0));
		ui->peerListWidget->update();
	} catch(...){
		qDebug()<<"co mam niby usunac?!";
	}

/*	foreach (auto it,delete_list) {
		ui->peerListWidget->removeItemWidget(it);
//		qDebug()<<it->text();
	}
*/
}

void MainWindow::on_run_tunserver_clicked() {
	QStringList l_peer_list;

	for (auto it :m_peer_lst) {
		QString label = QString::fromStdString(it.m_ipv4+":"+std::to_string(it.m_port)+"-"+it.m_ipv6);
		l_peer_list.push_back(label);
	}


	QString tunserver_location = QFileDialog::getOpenFileName(this,
		tr("Open tunserver binary file (tunserver.elf)"));

	try {
		start_tunserver(m_peer_lst, tunserver_location);
	} catch (const std::exception &err) {
		qDebug() << err.what();
	}
}

void MainWindow::add_host_info(QString host, uint16_t port) {

	qDebug() << "Host: " << host << "Port:" << port << '\n';
	if (host.isEmpty()) {
		host = "127.0.0.1";
		qDebug() << "use default 'localhost' host";
	}
	if (port == 0) {
		port = 42000;
		qDebug() << "use default '42000' port";
	}
	m_cmd_exec->startConnect(QHostAddress(host), port);
}

void MainWindow::on_connectButton_clicked() {
	hostDialog host_dialog;

	connect (&host_dialog, SIGNAL(host_info(QString, uint16_t)),
			 this, SLOT(add_host_info(QString, uint16_t)));

	if (host_dialog.exec() == QDialog::Accepted){
		qDebug() << "host inforation accepted";
	}
}

void MainWindow::on_ping_clicked() {
	order ord(order::e_type::PING);
	m_cmd_exec->sendNetRequest(ord);
}


void MainWindow::SavePeers(QString file_name)
{
	ParamsContainer container;
	container.setPeerList(m_peer_lst);
	container.writeParams(file_name);
}

/*void MainWindow::show_msg(const json &msg)
{
	qDebug()<<"show new message \n";
	//std::cout<<msg["topic"];
	std::string tmp = msg["msg"];
	//std::string text = tmp["text"];
	ui->debugWidget->addItem(tmp.c_str());
//	ui->debugWidget->addItem(text);
	qDebug()<<tmp.c_str();
}

void MainWindow::show_peers(const nlohmann::json &msg) {
	qDebug() << "show message as array\n";
	std::vector<std::string> messages = msg["msg"];
	ui->peerListWidget->clear();
	for (const auto &element : messages) {
		ui->debugWidget->addItem(element.c_str());
		qDebug() << element.c_str();
		ui->peerListWidget->addItem(QString(element.c_str()));
	}
}*/

void MainWindow::on_actionDebug_triggered() {
	qDebug()<< "show dialog";
		DebugDialog dialog;
		dialog.exec();
		dialog.show();
}
