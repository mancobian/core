///
/// @class Factory<>::Manager
///

template <typename T>
uint32_t Factory<T>::Manager::FACTORY_ID = 0;

template <typename T>
Factory<T>::Manager::Manager()
{
}

template <typename T>
Factory<T>::Manager::~Manager()
{
}

template <typename T>
const uint_t Factory<T>::Manager::generateFactoryId()
{
	return ++Factory<T>::Manager::FACTORY_ID;
}

template <typename T>
bool Factory<T>::Manager::hasFactory(const uint32_t type)
{
	if (this->mFactories.find(type) == this->mFactories.end())
		return false;
	return true;
}

template <typename T>
Factory<T>* Factory<T>::Manager::getFactory(const uint32_t type)
{
  typename FactoryMap::iterator iter = this->mFactories.find(type);
  if (iter == this->mFactories.end())
    return NULL;
  return iter->second;
}

template <typename T>
bool Factory<T>::Manager::registerFactory(Factory<T> *factory)
{
	if (this->hasFactory(factory->getType()))
		return false;
	this->mFactories.insert(std::make_pair(factory->getType(), factory));
	return true;
}

template <typename T>
Factory<T>* Factory<T>::Manager::unregisterFactory(const uint32_t type)
{
	Factory<T> *factory = NULL;
	if (this->hasFactory(type))
	{
		typename FactoryMap::iterator iter = this->mFactories.find(type);
		factory = iter->second;
		this->mFactories.erase(iter);
	}
	return factory;
}

///
/// @class Factory<>
///

template <typename T>
Factory<T>::Factory()
{
}

template <typename T>
Factory<T>::~Factory()
{
}

template <typename T>
bool Factory<T>::operator ==(const uint32_t &value) const
{
	return (this->getType() == value);
}

template <typename T>
bool Factory<T>::operator <(const uint32_t &value) const
{
	return (this->getType() < value);
}

template <typename T>
bool Factory<T>::operator ==(const Factory &value) const
{
	return this->operator ==(value.getType());
}

template <typename T>
bool Factory<T>::operator <(const Factory &value) const
{
	return this->operator <(value.getType());
}

///
/// @class Factory<>::Impl<>
///

template <typename T>
template <typename U>
const uint_t Factory<T>::Impl<U>::TYPE = Factory<T>::Manager::generateFactoryId();

template <typename T>
template <typename U>
Factory<T>::Impl<U>::Impl() :
  Pattern::Manager<U*>()
{
  /// Register factory on creation
  Factory<T>::Manager::getPointer()->registerFactory(this);
}

template <typename T>
template <typename U>
Factory<T>::Impl<U>::~Impl()
{
  /// Register factory on destruction
  Factory<T>::Manager::getPointer()->unregisterFactory(this->getType());
}

template <typename T>
template <typename U>
T* Factory<T>::Impl<U>::create()
{
	T *value = this->createImpl();
	this->add(value);
	return value;
}

template <typename T>
template <typename U>
void Factory<T>::Impl<U>::destroy(T *value)
{
	this->remove(value);
	delete value;
}
