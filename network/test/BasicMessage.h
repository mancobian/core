#ifndef NOUS_NETWORK_BASICMESSAGE_HPP
#define NOUS_NETWORK_BASICMESSAGE_HPP

#include <network/Common.h>
#include <network/Message.h>

namespace RSSD {
namespace Network {

class BasicMessage : public BaseMessage<BasicMessage>
{
public:
	typedef boost::shared_ptr<BasicMessage> Pointer;
	typedef BaseMessage<BasicMessage>::Factory Factory;

public:
	BasicMessage();
	BasicMessage(
		uint32_t number,
		const std::string &text);
	virtual ~BasicMessage();

public:
	inline uint32_t getInt() const { return this->_number; }
	inline const std::string& getString() const { return this->_text; }

public:
	virtual const byte* toBytes();
	virtual void fromBytes(const byte *buffer);

protected:
	uint32_t _number;
	std::string _text;
}; // class BasicMessage

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_BASICMESSAGE_HPP
