#include "dataeater.hpp"
#include <iostream>
#include <cstring>

#include "trivialserialize.hpp"

void dataeater::eat(std::vector<uint8_t> &data) {
	for(const auto &i:data) m_internal_buffer.push(i);
}

void dataeater::eat(std::string &data) {
	static_assert(sizeof(char) == sizeof(uint8_t), "size of char are different than size of uint8_t");
	for(const auto i:data) {
		uint8_t que_ele;
		memcpy(&que_ele,&i,sizeof(uint8_t));
		m_internal_buffer.push(que_ele);
	}
}

void dataeater::process() {

	if(!m_is_processing) {
		processFresh();
	} else {
		continiueProcessing();
	}
}

uint16_t dataeater::pop_msg_size() {
	uint16_t msg_size;
	msg_size = m_internal_buffer.front() << 8;
	m_internal_buffer.pop();
	msg_size += m_internal_buffer.front();
	m_internal_buffer.pop();
	return msg_size;
}

bool dataeater::processFresh() {
	// change 4 to 2 because of no 0xff at the begin of packet
	if (m_internal_buffer.size() < 2){
		return false;
	}

	// Is it really neccessary?
	//if(char (m_internal_buffer.front()) != char(0xff)) {	//frame should start with 0xff. If not - something goes wrong
	//	m_internal_buffer.pop();
	//	return false;
	//}

	m_frame_size = pop_msg_size();

	std::cout << "qframe size = " << m_frame_size << std::endl;

	m_current_index = 0;
	continiueProcessing();
	return m_is_processing = true;
}

bool dataeater::continiueProcessing() {

	while (!m_internal_buffer.empty()) {
		if (m_frame_size == m_current_index) {
			m_commands_list.push(m_last_command);
			m_last_command.clear();
			m_is_processing= false;
			processFresh();
			return true;
		}else {
			m_last_command.push_back(m_internal_buffer.front()); m_internal_buffer.pop();
		}
		m_current_index++;
	}
	return m_is_processing;
}

std::string dataeater::getLastCommand() {

	if(m_commands_list.empty()) {
		return std::string();
	}
	return std::string (m_commands_list.back());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

QByteArray simple_packet_eater::serialize_msg(const std::string &msg) {
	assert(msg.size() <= std::numeric_limits<uint16_t>::max() && "Too big message");
	uint16_t msg_size = static_cast<uint16_t>(msg.size());

	QByteArray packet(msg_size + 2, 0); // 2 is bytes for size

	packet[0] = static_cast<char>(msg_size >> 8);
	packet[1] = static_cast<char>(msg_size & 0xFF);

	for (unsigned int i = 0; i < msg_size; ++i) {
		packet[i + 2] = msg.at(i);
	}
	return packet;
}

std::string simple_packet_eater::deserialize_msg(const std::vector<uint8_t> &packet) {
	uint16_t msg_size;
	msg_size = packet[0] << 8;
	msg_size += packet[1];
	std::string msg(msg_size, 0);
	for (size_t i = 0; i < msg_size; ++i) {
		msg.at(i) = packet.at(i + 2);
	}
	return msg;
}

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
