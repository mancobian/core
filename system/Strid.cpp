#include "Strid.h"

using namespace RSSD;

boost::mutex Strid::HASHMUTEX;
Strid::id_s Strid::HASHSET;
Strid::Strid_m Strid::HASHMAP;

Strid::Strid() :
  _id(0), _text("__uninitialized__")
{
}

Strid::Strid(const char *text) :
  _id(0), _text(text)
{
  this->_id = getHash(this->_text);
}

Strid::Strid(uint32_t id, const char *text) :
  _id(id), _text(text)
{
}

Strid::Strid(const Strid &rhs) :
  _id(rhs._id), _text(rhs._text)
{
}

Strid::~Strid()
{
}

uint32_t Strid::getHash(const std::string &text)
{
  // Concurrency lock
  boost::mutex::scoped_lock lock(Strid::HASHMUTEX);

  // Return ID if input string has already been hashed
  if (Strid::HASHMAP.find(text) != Strid::HASHMAP.end())
  {
    return Strid::HASHMAP[text];
  }

  // Create string hash
  std::locale c_locale; // the "C" locale
  const std::collate<char>& collate_char = std::use_facet<std::collate<char> >(c_locale);
  uint32_t id = collate_char.hash(text.data(), text.data() + text.length());
  assert (Strid::HASHSET.find(id) == Strid::HASHSET.end());

  // Store string hash
  Strid::HASHMAP.insert(std::make_pair(text, id));
  Strid::HASHSET.insert(id);
  return id;
}
