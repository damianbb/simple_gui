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

std::string order::get_str() const {
	return std::string(); // TODO
}
