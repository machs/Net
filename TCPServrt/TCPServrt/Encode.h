#include <iostream>
#include <queue>
#include <vector>
#include <memory>

struct Header
{
    int32_t     mianCommon;
    int32_t     subCommon;
    int32_t     bodyLength;
};

struct Message
{
    int32_t         sendAnthorID;
    std::string     sendMsg;
};


//基础解码功能
class Encode   
{
public:
    Encode();
    ~Encode();

	bool init();

	void onRecvData(const char* revData, int msglenth);
	std::shared_ptr<Message> sendRecvData();

private:
	void moveBufferData(int movelength);

	void SetMessage();

	void UpdaeData(float df);

	void GetHeader(Header* header);

private:
    std::vector<char>   m_buffer_;
    std::queue<std::shared_ptr<Message>> m_message_;

};