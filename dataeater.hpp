#ifndef DATAEATER_H
#define DATAEATER_H


#include <memory>
#include <queue>

class dataeater
{
	std::queue<char> m_internal_buffer;
	std::queue<std::string> m_commands_list;
	std::string m_last_command;

	bool m_have_new;
	bool m_is_processing;

	int m_frame_size;
	int m_current_index;

	bool processFresh();
	bool continiueProcessing();

public:
	dataeater():m_have_new(false),m_is_processing(false){;}


	void eat(std::vector<char> &data);
	void eat(std::string &data);
	void process();
	bool hasNextCommand();
	std::string getLastCommand();


};

// using trivialserialize
class simple_packet_eater {
public:
	std::queue<std::string> m_msg_queue;

	static std::string process_pck(const std::string &pck);
	void eat_packet(const std::string &pck);
	std::string pop_last_message();

private:
	std::queue<std::string> m_msg_queue;
};

#endif // DATAEATER_H
