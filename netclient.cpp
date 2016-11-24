#include "netclient.hpp"
#include <cassert>

netClient::netClient(std::shared_ptr<commandExecutor> cmd_exec_ptr)
:
	m_cmd_exec(cmd_exec_ptr)
{
	m_socket = new QTcpSocket();
	connect(m_socket, SIGNAL(readyRead()),this, SLOT(onTcpReceive()));
}

netClient::~netClient() {
	delete m_socket;
}

void netClient::startConnect(const QHostAddress &address, uint16_t port) {
	m_socket->connectToHost(address, port);
	// we need to wait...
	if(!m_socket->waitForConnected(5000)) {
		qDebug() << "Error: " << m_socket->errorString();
	}
/*	m_pr_call = true;
	connect(this, SIGNAL(ask_for_peerlist()),this, SLOT(peerlist_request_slot()));

	th_peerlist = std::make_unique<std::thread>([this]() { call_peerlist_requests(); });*/
}

bool netClient::is_connected() {
	if(m_socket == nullptr) {
		qDebug()<<"Socket is not defined (nullptr)";
		return false;
	}
	else if (m_socket->state() != QAbstractSocket::ConnectedState) {
		qDebug()<<"Socket is not connected";
		return false;
	} else {
		return true;
	}
}

void netClient::send_msg(const std::string &msg) {
	QByteArray packet = serialize_msg(msg);
	size_t send_bytes = m_socket->write(packet);
	if (send_bytes != packet.size())
		throw std::runtime_error("send packet error");
}

void netClient::onTcpReceive() {
	std::cout << "onTcpReceive\n";
	QByteArray data_array = m_socket->readAll();
	std::string arr(data_array.data(), static_cast<size_t>(data_array.size()));
	m_data_eater.eat(arr);
	m_data_eater.process();
	std::string last_cmd = m_data_eater.getLastCommand();
	std::cout << "last command " << last_cmd << std::endl;
	if (!last_cmd.empty()) {
		auto cmd_exec_ptr = m_cmd_exec.lock();
		cmd_exec_ptr->parseAndExecMsg(last_cmd);
	}
}

QByteArray netClient::serialize_msg(const std::string &msg) {
	assert(msg.size() <= std::numeric_limits<uint16_t>::max() && "Too big message");
	uint16_t msg_size = static_cast<uint16_t>(msg.size());

	QByteArray packet(msg_size + 2, 0); // 2 is bytes for size

	packet[0] = static_cast<char>(msg_size >> 8);
	packet[1] = static_cast<char>(msg_size & 0xFF);

	for (unsigned int i = 0; i < msg_size; ++i) {
		packet[i + 2] = msg.at(i);
	}
	return packet;
}
