#include "dataeater.hpp"
#include <iostream>
#include "trivialserialize.hpp"

void dataeater::eat(std::vector<char> &data)
{
	for(const auto &it:data) m_internal_buffer.push(it);
}

void dataeater::eat(std::string &data)
{
	for(const auto &it:data) m_internal_buffer.push(it);

}

void dataeater::process()
{
	if(!m_is_processing)
	{
		processFresh();
	}else{
		continiueProcessing();
	}
}

bool dataeater::processFresh()
{
	// change 4 to 2 because of no 0xff at the begin of packet
	if (m_internal_buffer.size() < 3){
		return false;
	}

	// Is it really neccessary?
	//if(char (m_internal_buffer.front()) != char(0xff)) {		//frame should start with 0xff. If not - something goes wrong
	//	m_internal_buffer.pop();
	//	return false;
	//}

	m_internal_buffer.pop();

	char size_tab[2];
	size_tab[0] = m_internal_buffer.front();m_internal_buffer.pop();
	size_tab[1] = m_internal_buffer.front();m_internal_buffer.pop();

	m_frame_size = reinterpret_cast<int16_t&>(*size_tab);

	std::cout << "qframe size = " << m_frame_size << std::endl;

	m_current_index = 3;
	continiueProcessing();
	m_is_processing = true;
}

bool dataeater::continiueProcessing()
{
	while (!m_internal_buffer.empty()) {
		if (m_frame_size == m_current_index) {
			m_commands_list.push(m_last_command);
			m_last_command.clear();
			m_have_new = true;
			m_is_processing= false;
			processFresh();
			return true;
		}else {
			m_last_command.push_back(m_internal_buffer.front()); m_internal_buffer.pop();
		}
		m_current_index++;
	}
}

std::string dataeater::getLastCommand()
{
	if(m_commands_list.empty()) {
		return std::string();
	}

	return std::string (m_commands_list.back());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

std::string simple_packet_eater::process_packet(const std::string &pck) {
	trivialserialize::parser parser(trivialserialize::parser::tag_caller_must_keep_this_string_valid(),
									pck);
	uint64_t msg_size = parser.pop_integer_uvarint();
	parser.pop_bytes_n(1);
	std::cout << "Parsed msg size: " << msg_size << '\n';
	return parser.pop_bytes_n(msg_size);
}

void simple_packet_eater::eat_packet(const std::string &pck) {
	m_msg_queue.push(simple_packet_eater::process_packet(pck));
}

std::string simple_packet_eater::pop_last_message() {
	if(m_msg_queue.empty()) {
		return "";
	}
	std::string msg = m_msg_queue.front();
	m_msg_queue.pop();
	std::cout << "msg=[" << msg << "] size=[" << msg.size() << "]\n";
	return msg;
}
