#ifndef NETPARSER_H
#define NETPARSER_H

#include <functional>
#include <queue>
#include <mutex>

#include "json.hpp"

class commandExecutor;

class order
{
public:
	order(commandExecutor *executor,nlohmann::json params):m_executor(executor),m_msg(params){;}
	virtual void exec() = 0;
	std::string getAns(std::string ans);

protected:
	virtual void parse()
	{
		m_id = m_msg["id"];
		m_topic = m_msg["topic"];
	}

	nlohmann::json m_msg;
	commandExecutor *m_executor;
	int m_id;
	std::string m_topic;
};

typedef std::shared_ptr<order> order_ptr;	//TODO przerobic na unique_ptr

class commandExecutor
{
	std::queue<order_ptr> ord_queue;

public:
	void addOrder(order_ptr ord){ord_queue.push(ord);}

	// thread safe
	virtual void execNextOrder() {
		std::lock_guard<std::mutex>lock(m_order_mutex);
		if(ord_queue.empty()) {
			return;
		}

		std::cout << "new order: [" << ord_queue.front() << std::endl;
		ord_queue.front()->exec();
		ord_queue.pop();
	}

	virtual void showMsg(const nlohmann::json &msg) = 0;
	virtual void askQuestion(const nlohmann::json &msg) = 0;
	virtual void showWorning(const nlohmann::json & msg) = 0;
	virtual void writeAns(const nlohmann::json &msg) = 0;
	virtual void addPeer(const nlohmann::json &msg) = 0;
	virtual void ping() = 0;


	std::mutex m_order_mutex;

};

class msgOrder:public order
{
public:
	msgOrder(commandExecutor *executor,nlohmann::json msg):order (executor,msg){;}

	virtual void exec()
	{
		//parse();
		m_executor->showMsg(m_msg);
	}
};

class netParser
{
public:
	netParser(commandExecutor &executor):m_executor(executor){;}

	void parseMsg(std::string msg) {
		if (msg.empty()) return;
		nlohmann::json j = nlohmann::json::parse(msg);
		std::string cmd = j["cmd"];
		std::cout << "cmd=" << cmd << '\n';
		if(cmd == "info") {
			order_ptr ord(new msgOrder(&m_executor,j));
			std::lock_guard<std::mutex>lock(m_executor.m_order_mutex);
			m_executor.addOrder(ord);
		} else if (cmd == "ping"){
			order_ptr ord(new msgOrder(&m_executor,j));
			std::lock_guard<std::mutex>lock(m_executor.m_order_mutex);
			m_executor.addOrder(ord);

		}

	}

private:
	commandExecutor &m_executor;

};

#endif // NETPARSER_H
