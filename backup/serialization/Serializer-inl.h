template <typename T> boost::any Serializer<T>::UNKNOWN_DATA_MEMBER;

template <typename T>
Serializer<T>::Serializer()
{

}

template <typename T>
Serializer<T>::Serializer(const DataMemberMap &dataMembers) :
  mDataMembers(dataMembers)
{

}

template <typename T>
Serializer<T>::~Serializer()
{

}

template <typename T>
typename Serializer<T>::Serializable* Serializer<T>::operator [](const char *key)
{
  Strid skey(key);
  if (this->mDataMembers.find(skey) == this->mDataMembers.end())
    //return Serializer<T>::UNKNOWN_DATA_MEMBER;
    return NULL;
  // Serializable *serializable = boost::any_cast<Serializable*>(this->mDataMembers[skey]);
  return this->mDataMembers[skey];
}

template <typename T>
uint_t Serializer<T>::size() const
{
  uint_t result = 0;
  typename DataMemberMap::const_iterator
    iter = this->mDataMembers.begin(),
    end = this->mDataMembers.end();
  for (; iter != end; ++iter)
  {
    // const Serializable *serializable = boost::any_cast<const Serializable*>(iter->second);
    Serializable *serializable = iter->second;
    result += serializable->size();
  }
  return result;
}
