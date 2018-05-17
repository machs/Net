
#include "Encode.h"


Encode::Encode()
{

}
Encode::~Encode()
{

}


bool Encode::init()
{
	return true;
}

void Encode::onRecvData(const char* revData, int msglenth)
{
	//先把收到的数据加入到buffer_中
	for (size_t i = 0; i < msglenth; i++)
		m_buffer_.push_back(revData[i]);
	printf("======================m_buffer_.size() = %d\n", m_buffer_.size());
	//在判断收到的数据完整性
	int headerSize = sizeof(Header);
	if (m_buffer_.size() >= headerSize)
	{
		Header header;
		GetHeader(&header);
		if (m_buffer_.size() >= (headerSize + header.bodyLength))
		{
			printf("======================m_buffer_.size() >= headerSize\n");
			SetMessage();
		}
	}
}

std::shared_ptr<Message> Encode::sendRecvData()
{
	if (m_message_.size() == 0) return nullptr;
	auto item = m_message_.front();
	m_message_.pop();
	return item;
}

void Encode::moveBufferData(int movelength)
{
	if (m_buffer_.size() >= movelength)
	{
		std::vector<char> temp(m_buffer_.begin() + movelength, m_buffer_.end());
		m_buffer_.clear();
		m_buffer_.swap(temp);
	}
}

void Encode::GetHeader(Header* header)
{
	char* buffers = new char[sizeof(Header)];
	std::copy(m_buffer_.begin(), m_buffer_.end(), buffers);
	memcpy(header, buffers, sizeof(Header));
}

void Encode::SetMessage()
{
	moveBufferData(sizeof(Header));
	char buffers[sizeof(Message)];
	std::copy(m_buffer_.begin(), m_buffer_.end(), buffers);
	std::shared_ptr<Message> msg(new Message());
	auto buff = msg.get();
	memcpy(msg.get(), buffers, sizeof(Message));
	m_message_.push(msg);
	moveBufferData(sizeof(Message));

	printf("====================TTTTTT  %d,%s\n",(msg.get())->sendAnthorID,(msg.get())->sendMsg.c_str());
}

void Encode::UpdaeData(float df)
{

}