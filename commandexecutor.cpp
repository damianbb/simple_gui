#include "commandexecutor.hpp"
#include "json.hpp"

void commandExecutor::parseAndExecMsg(const std::string &msg) {
	using nlohmann::json;
	json j = json::parse(msg);
	std::string cmd = j["cmd"];
	if (cmd == "ping") {
		// exec ping
	}
}

void commandExecutor::sendNetRequest(const order &ord) {
	m_net_client->send_msg(ord.get_str());
}

void commandExecutor::startConnect(const QHostAddress &address, uint16_t port) {
	m_net_client->startConnect(address, port);
}

std::string order::get_str() const {
	nlohmann::json j{{"cmd", m_cmd} , {"msg", m_msg}};
	return j.dump();
}
