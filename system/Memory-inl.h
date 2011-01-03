template <typename T>
WeakPointerEqualityPredicate<T>::WeakPointerEqualityPredicate(const std::tr1::weak_ptr<T> &item) :
  mItem(item)
{
}

template <typename T>
bool WeakPointerEqualityPredicate<T>::operator ()(const std::tr1::weak_ptr<T> &item) const
{
  return (item.lock() == this->mItem.lock());
}
