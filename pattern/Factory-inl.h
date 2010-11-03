///
/// @class FactoryManager
///

template <typename PRODUCT>
FactoryManager<PRODUCT>::FactoryManager() :
	_id(1)
{
}

template <typename PRODUCT>
FactoryManager<PRODUCT>::~FactoryManager()
{
}

template <typename PRODUCT>
uint32_t FactoryManager<PRODUCT>::generateFactoryId()
{
	return this->_id++;
}

template <typename PRODUCT>
bool FactoryManager<PRODUCT>::hasFactory(const uint32_t type)
{
	if (this->_factories.find(type) == this->_factories.end())
		return false;
	return true;
}

template <typename PRODUCT>
bool FactoryManager<PRODUCT>::hasProduct(const PRODUCT *product)
{
	if (this->_products.find(const_cast<PRODUCT*>(product)) == this->_products.end())
		return false;
	return true;
}

template <typename PRODUCT>
bool FactoryManager<PRODUCT>::registerFactory(Factory<PRODUCT> *factory)
{
	if (this->hasFactory(factory->getType()))
		return false;
	this->_factories.insert(std::make_pair(factory->getType(), factory));
	return true;
}

template <typename PRODUCT>
Factory<PRODUCT>* FactoryManager<PRODUCT>::unregisterFactory(const uint32_t type)
{
	Factory<PRODUCT> *factory = NULL;
	if (this->hasFactory(type))
	{
		typename FactoryManager<PRODUCT>::Factory_m::iterator iter = this->_factories.find(type);
		factory = iter->second;
		this->_factories.erase(iter);
	}
	return factory;
}

template <typename PRODUCT>
PRODUCT* FactoryManager<PRODUCT>::create(const uint32_t type)
{
	if (!this->hasFactory(type))
		return NULL;
	PRODUCT *product = this->_factories[type]->create();
	this->_products.insert(std::make_pair(product, type));
	return product;
}

template <typename PRODUCT>
bool FactoryManager<PRODUCT>::destroy(PRODUCT *product)
{
	if (!this->hasProduct(product))
		return false;
	typename FactoryManager<PRODUCT>::Product_m::iterator iter = this->_products.find(product);
	uint32_t type = iter->second;
	this->_products.erase(iter);
	if (!this->hasFactory(type))
		return false;
	this->_factories[type]->destroy(product);
	return true;
}

///
/// @class Factory
///

template <typename PRODUCT>
Factory<PRODUCT>::Factory() // : _type(FactoryManager<PRODUCT>::getPointer()->generateFactoryId())
{
}

template <typename PRODUCT>
Factory<PRODUCT>::~Factory()
{
	pattern::Manager<PRODUCT*>::clear();
}

template <typename PRODUCT>
bool Factory<PRODUCT>::operator ==(const uint32_t &value) const
{
	return (this->getType() == value);
}

template <typename PRODUCT>
bool Factory<PRODUCT>::operator <(const uint32_t &value) const
{
	return (this->getType() < value);
}

template <typename PRODUCT>
bool Factory<PRODUCT>::operator ==(const Factory &value) const
{
	return this->operator ==(value.getType());
}

template <typename PRODUCT>
bool Factory<PRODUCT>::operator <(const Factory &value) const
{
	return this->operator <(value.getType());
}

template <typename PRODUCT>
PRODUCT* Factory<PRODUCT>::create()
{
	PRODUCT *product = this->createImpl();
	this->add(product);
	return product;
}

template <typename PRODUCT>
void Factory<PRODUCT>::destroy(PRODUCT *product)
{
	this->remove(product);
}
