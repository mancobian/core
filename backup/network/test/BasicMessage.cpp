#include "BasicMessage.h"

using namespace RSSD;
using namespace RSSD::Network;

template <> BasicMessage::Factory* BasicMessage::Factory::Singleton::_instance = NULL;

BasicMessage::BasicMessage() :
	BaseMessage<BasicMessage>(),
	_number(0),
	_text("")
{
}

BasicMessage::BasicMessage(
	uint32_t number,
	const string_t &text) :
	BaseMessage<BasicMessage>(),
	_number(number),
	_text(text)
{
	this->toBytes();
}

BasicMessage::~BasicMessage()
{}

const byte* BasicMessage::toBytes()
{
	this->_buffer.setpos(Buffer::WRITE, Message::DATA_OFFSET)
		<< this->_number
		<< this->_text;
	// NLOG (*this, "BASICMESSAGE");
	return Message::toBytes();
}

void BasicMessage::fromBytes(const byte *buffer)
{
	Message::fromBytes(buffer);
	this->_buffer.setpos(Buffer::READ, Message::DATA_OFFSET)
		>> this->_number
		>> this->_text;
	// NLOG (*this, "BASICMESSAGE");
}
