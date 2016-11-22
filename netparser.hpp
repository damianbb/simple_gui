#ifndef NETPARSER_H
#define NETPARSER_H

#include <functional>
#include <queue>
#include <mutex>

#include "json.hpp"

using nlohmann::json;

class commandExecutor;

class order
{
public:
	order(commandExecutor *executor,json params);
	virtual void exec() = 0;
	std::string getAns(std::string ans);

protected:
	virtual void parse();

	json m_msg;
	commandExecutor *m_executor;
	int m_id;
	std::string m_topic;
};

typedef std::shared_ptr<order> order_ptr;	//TODO przerobic na unique_ptr

class commandExecutor
{
	std::queue<order_ptr> ord_queue;

public:
	void addOrder(order_ptr ord);

	// thread safe
	virtual void execNextOrder();

	virtual void show_msg(const json &msg) = 0;
//	virtual void askQuestion(const json &msg) = 0;
//	virtual void showWorning(const json & msg) = 0;
//	virtual void writeAns(const json &msg) = 0;
//	virtual void addPeer(const json &msg) = 0;
	virtual void send_request(const json &request) = 0;


	std::mutex m_order_mutex;

};

class msgOrder:public order {

public:
	msgOrder(commandExecutor *executor,json msg):order (executor,msg){;}

	virtual void exec();
};

class netParser {

public:
	netParser(commandExecutor &executor);

	void parseMsg(std::string msg);

private:
	commandExecutor &m_executor;

};

#endif // NETPARSER_H
