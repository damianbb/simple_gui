#ifndef NETCLIENT_HPP
#define NETCLIENT_HPP

#include <QTcpSocket>
#include "dataeater.hpp"

class netClient final : QObject {
	public:
		netClient();
		~netClient();
		void startConnect(const QHostAddress &address, uint16_t port);
		bool is_connected();
		void send_msg(const std::string &msg);
	private:
		std::weak_ptr<commandExecutor> m_cmd_exec;
		QTcpSocket *m_socket;
		dataeater m_data_eater;
		QByteArray serialize_msg(const std::string &msg);

	private slots:
		void onTcpReceive();
};

#endif // NETCLIENT_HPP
