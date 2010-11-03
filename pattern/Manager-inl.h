template <typename ITEM>
Manager<ITEM>::Manager()
{
}

template <typename ITEM>
Manager<ITEM>::~Manager()
{
	this->clear();
}

template <typename ITEM>
typename Manager<ITEM>::Handle Manager<ITEM>::get(const ITEM &item)
{
	Handle handle = std::find(
		this->_items.begin(),
		this->_items.end(),
		item);
	return handle;
}

template <typename ITEM>
bool Manager<ITEM>::has(const ITEM &item)
{
	Handle handle = this->get(item);
	return (handle != this->_items.end());
}

template <typename ITEM>
uint32_t Manager<ITEM>::size() const
{
	return this->_items.size();
}

template <typename ITEM>
bool Manager<ITEM>::add(ITEM item)
{
	if (this->has(item))
		return false;
	this->_items.push_back(item);
	return true;
}

template <typename ITEM>
bool Manager<ITEM>::remove(ITEM item)
{
	if (!this->has(item))
		return false;
	this->_items.remove(item);
	return true;
}

template <typename ITEM>
void Manager<ITEM>::clear()
{
	this->_items.clear();
}

template <typename ITEM>
Manager<ITEM*>::Manager()
{
}

template <typename ITEM>
Manager<ITEM*>::~Manager()
{
	this->clear();
}

template <typename ITEM>
typename Manager<ITEM*>::Handle Manager<ITEM*>::get(const ITEM* item)
{
	Handle handle = std::find(
		this->_items.begin(),
		this->_items.end(),
		item);
	return handle;
}

template <typename ITEM>
bool Manager<ITEM*>::has(const ITEM* item)
{
	if (!item) return false;
	Handle handle = this->get(item);
	return (handle != this->_items.end());
}

template <typename ITEM>
uint32_t Manager<ITEM*>::size() const
{
	return this->_items.size();
}

template <typename ITEM>
bool Manager<ITEM*>::add(ITEM* item)
{
	if (this->has(item))
		return false;
	this->_items.push_back(item);
	return true;
}

template <typename ITEM>
bool Manager<ITEM*>::remove(ITEM* item)
{
	if (!this->has(item))
		return false;
	this->_items.remove(item);
	return true;
}

template <typename ITEM>
void Manager<ITEM*>::clear()
{
	Handle iter = this->_items.begin();
	Handle end = this->_items.end();
	for (; iter != end; ++iter)
	{
		ITEM *item = *iter;
		delete item;
	}
	this->_items.clear();
}

