#include <QDebug>
#include <QProcess>
#include <QTcpSocket>

#include <regex>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "addressdialog.hpp"
#include "paramscontainer.hpp"
#include "dataeater.hpp"
#include "debugdialog.hpp"

#include <boost/asio.hpp>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),m_parser(*this),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	/*
	m_msg_server = new QTcpServer(this);
	m_msg_server->setMaxPendingConnections(1);
	m_msg_server->listen(QHostAddress::LocalHost,9933);
	*/
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

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_connectButton_clicked()
{
	QStringList l_peer_list;

	for (auto it :m_peer_lst) {
		QString label = QString::fromStdString(it.m_ipv4+":"+std::to_string(it.m_port)+"-"+it.m_ipv6);
		l_peer_list.push_back(label);
	}
	/*
	for (int i= 0 ;i<ui->peerListWidget->count();i++) {
		l_peer_list.push_back(ui->peerListWidget->item(i)->text());

		//QString peer_string =" --peer "+my_ip +":9042-"+it;

	}
	*/
	startProgram(l_peer_list);
}

void MainWindow::addMsg()
{
	QString msg = m_socket->readAll();
	ui->debugWidget->addItem(msg);
}

void MainWindow::onNewConnection()
{

	/*
	QTcpSocket *m_socket = m_msg_server->nextPendingConnection();
	connect(m_socket,SIGNAL(readyRead()),this,SLOT(addMsg()));
	*/
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

void MainWindow::addAddress(QString address)
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

void MainWindow::startProgram(QStringList & l_peer_list)
{
	QString command = "./../../../galaxy42/tunserver.elf";
	QStringList params_list;
	foreach (auto it, l_peer_list) {
		QString peer_string =" --peer "+it;
		params_list.push_back(peer_string);
	}

	qDebug()<<params_list;
	QProcess *m_tunserver_process = new QProcess(this);
	//connect (m_tunserver_process,SIGNAL(readyReadStandardOutput()),this,SLOT(onProcessInfo()));
	//connect (m_tunserver_process,SIGNAL(readyReadStandardError()),this,SLOT(onProcessError()));

	m_tunserver_process->start(command , params_list);
	sleep(1);
}

void MainWindow::onReciveTcp()
{
	std::string arr = m_socket->readAll().toStdString();

	m_data_eater.eat(arr);
	m_data_eater.process();

	std::lock_guard<std::mutex> guard(m_order_mutex);
	execNextOrder();
}

void MainWindow::sendReciveTcp(QString &msg)
{

}


void MainWindow::showDebugPage(QByteArray &pageCode)
{

}

void MainWindow::onProcessInfo()
{
	qDebug()<<m_tunserver_process->readAll();
}

void MainWindow::onProcessError()
{
	qDebug()<<m_tunserver_process->readAll();
}

void MainWindow::on_plusButton_clicked()
{
	m_dlg = new addressDialog(this);
	connect (m_dlg,SIGNAL(addAddress(QString)),this,SLOT(addAddress(QString)));
	m_dlg->show();
}

void MainWindow::on_minusButton_clicked()
{
	auto delete_list= ui->peerListWidget->selectedItems();
	try{
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

void MainWindow::SavePeers(QString file_name)
{
	ParamsContainer container;
	container.setPeerList(m_peer_lst);
	container.writeParams(file_name);
}

void MainWindow::showMsg(const nlohmann::json &msg)
{
	std::cout<<"show new message \n";
	std::cout<<msg["topic"];
	auto tmp = msg["msg"];
	std::string text = tmp["text"];
	ui->debugWidget->addItem(text.c_str());
//	ui->debugWidget->addItem(text);
	qDebug()<<text.c_str();
}

void MainWindow::startConnection()
{
	m_socket = new QTcpSocket(this);

	m_socket->connectToHost("localhost", 8899);

	connect(m_socket, SIGNAL(readyRead()),this, SLOT(onReciveTcp()));

		// we need to wait...
		if(!m_socket->waitForConnected(5000))
		{
			qDebug() << "Error: " << m_socket->errorString();
		}
}


void MainWindow::on_actionDebug_triggered()
{
	qDebug()<< "show dlg";
		DebugDialog dialog;
		dialog.exec();
		dialog.show();
}


