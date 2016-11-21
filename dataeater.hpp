#ifndef DATAEATER_H
#define DATAEATER_H


#include <memory>
#include <queue>

class dataeater {
public:
	dataeater(): m_is_processing(false){;}

	void eat(std::vector<uint8_t> &data);
	void eat(std::string &data);
	void process();
	bool hasNextCommand();
	std::string getLastCommand();

private:
	std::queue<uint8_t> m_internal_buffer;
	std::queue<std::string> m_commands_list;
	std::string m_last_command;

	uint16_t pop_msg_size();
	bool processFresh();
	bool continiueProcessing();

	bool m_is_processing;

	int m_frame_size;
	int m_current_index;
};

// using trivialserialize
class simple_packet_eater {
public:
	static std::string process_packet(const std::string &pck);

	static std::vector<uint8_t> serialize_msg(const std::string &msg);
	static std::string deserialize_msg(const std::vector<uint8_t> &packet);

	void eat_packet(const std::string &pck);
	std::string pop_last_message();

private:
	std::queue<std::string> m_msg_queue;
};

#endif // DATAEATER_H
