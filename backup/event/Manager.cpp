#include <event/Manager.h>

using namespace Nous;
using namespace Nous::Event;

Manager::Manager() :
	Pattern::Manager<Event::Stream*>()
{
}

Manager::~Manager()
{
}

bool Manager::addEventProducer(Event::Stream *stream)
{
	stream->incrementNumConsumers();
	return this->add(stream);
}

bool Manager::removeEventProducer(Event::Stream *stream)
{
	stream->decrementNumConsumers();
	return this->remove(stream);
}

void Manager::clearEventProducers()
{
	BaseManager::clear();
}

bool Manager::processEventStream()
{
	return (this->aggregate() == this->_items.size());
}

uint32_t Manager::aggregate()
{
	if (this->_items.empty())
		return 0;

	Stream::clear();

	uint32_t result = 0;
	Stream_l::iterator iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		Stream *stream = *iter;
		if (Stream::append(stream))
		{
			/// @note This will only clear the event
			/// stream if all interested Event::Managers
			/// have already read from the stream,
			///  usu. on a per frame basis.
			stream->flush();
			++result;
		}
	}
	return result;
}
