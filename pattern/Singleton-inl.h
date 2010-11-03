template <typename T>
Singleton<T>::Singleton(const Singleton<T> &)
{
}

template <typename T>
Singleton<T>& Singleton<T>::operator=(const Singleton<T> &)
{
}

template <typename T>
Singleton<T>::Singleton()
{
	assert(!Singleton<T>::_instance);
#if defined( _MSC_VER ) && _MSC_VER < 1200
	int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
	Singleton<T>::_instance = (T*)((int)this + offset);
#else
	Singleton<T>::_instance = static_cast<T*>(this);
#endif
}

template <typename T>
Singleton<T>::~Singleton()
{
	assert(Singleton<T>::_instance);
	Singleton<T>::_instance = 0;
}

template <typename T>
T& Singleton<T>::getReference()
{
	assert(Singleton<T>::_instance);
	return (*Singleton<T>::_instance);
}

template <typename T>
T* Singleton<T>::getPointer()
{
	return Singleton<T>::_instance;
}
