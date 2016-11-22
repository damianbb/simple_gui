#include "netparser.hpp"




order::order(commandExecutor *executor, nlohmann::json params):m_executor(executor),m_msg(params){;}

void order::parse()
{
	m_id = m_msg["id"];
	m_topic = m_msg["topic"];
}

void commandExecutor::addOrder(order_ptr ord){ord_queue.push(ord);}

void commandExecutor::execNextOrder() {
	std::lock_guard<std::mutex>lock(m_order_mutex);
	if(ord_queue.empty()) {
		return;
	}

	std::cout << "new order: [" << ord_queue.front() << std::endl;
	ord_queue.front()->exec();
	ord_queue.pop();
}

void msgOrder::exec()
{
	//parse();
	m_executor->show_msg(m_msg);
}

netParser::netParser(commandExecutor &executor):m_executor(executor){;}

void netParser::parseMsg(std::string msg) {
	if (msg.empty()) return;
	json j = json::parse(msg);
	std::string cmd = j["cmd"];
	std::cout << "cmd=" << cmd << '\n';
	if(cmd == "info") {
		order_ptr ord(new msgOrder(&m_executor,j));
		std::lock_guard<std::mutex>lock(m_executor.m_order_mutex);
		m_executor.addOrder(ord);
	} else if (cmd == "ping") {
		order_ptr ord(new msgOrder(&m_executor,j));
		std::lock_guard<std::mutex>lock(m_executor.m_order_mutex);
		m_executor.addOrder(ord);
	} else if (cmd == "peer_list") {

	}

}
