///
/// @class BinarySerializable<>
///

template <typename T> const uint_t BinarySerializable<T>::SIZE = sizeof(T);

template <typename T>
BinarySerializable<T>::BinarySerializable()
{
}

template <typename T>
BinarySerializable<T>::BinarySerializable(const T &value)
{
}

template <typename T>
BinarySerializable<T>::~BinarySerializable()
{
}

template <typename T>
uint_t BinarySerializable<T>::size() const
{
  return BinarySerializable<T>::SIZE;
}

template <typename T>
uint_t BinarySerializable<T>::to(byte *&dst) const
{
  std::memcpy(dst, &this->mValue, BinarySerializable<T>::SIZE);
  return BinarySerializable<T>::SIZE;
}

template <typename T>
uint_t BinarySerializable<T>::from(byte *&src)
{
  std::memcpy(&this->mValue, src, BinarySerializable<T>::SIZE);
  return BinarySerializable<T>::SIZE;
}
