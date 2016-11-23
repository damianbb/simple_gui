#include "commandexecutor.hpp"
#include "json.hpp"

void commandExecutor::parseMsg(const std::string &msg) {
	using nlohmann::json;
	json j = json::parse(msg);
	std::string cmd = j["cmd"];
	if (cmd == "ping") {
		// exec ping
	}
}
