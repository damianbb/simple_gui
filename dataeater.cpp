#include "dataeater.hpp"


void dataeater::eat(std::vector<char> &data)
{
	for(auto it:data) m_internal_buffer.push(it);
}

void dataeater::eat(std::string &data)
{
	for(auto it:data) m_internal_buffer.push(it);

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
	if(m_internal_buffer.size()<4){
		return false;
	}

	if(char (m_internal_buffer.front()) != char(0xff)) {		//frame should start with 0xff. If not - something goes wrong
		m_internal_buffer.pop();
		return false;
	}
	m_internal_buffer.pop();

	char size_tab[2];
	size_tab[0] = m_internal_buffer.front();m_internal_buffer.pop();
	size_tab[1] = m_internal_buffer.front();m_internal_buffer.pop();

	m_frame_size = reinterpret_cast<int16_t&>(*size_tab);
	m_current_index = 3;
	continiueProcessing();
	m_is_processing = true;
}

bool dataeater::continiueProcessing()
{
	while (!m_internal_buffer.empty()) {
		if(m_frame_size == m_current_index) {
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
