template <typename T>
bool operator ==(
  const std::tr1::weak_ptr<typename RSSD::Core::Pattern::Publisher<T>::Subscriber> &a,
  const std::tr1::weak_ptr<typename RSSD::Core::Pattern::Publisher<T>::Subscriber> &b)
{
    return a.lock() == b.lock();
}

template <typename T>
Publisher<T>::Publisher() :
  mSubscriberManager(new Publisher<T>::SubscriberManager())
{}

template <typename T>
Publisher<T>::~Publisher()
{}

///
/// @ref This strategy for culling expired pointer references was informed by the following article:
///   http://schneide.wordpress.com/2008/12/08/observerlistener-structures-in-c-with-boosts-smart-pointers/
///
template <typename T>
void Publisher<T>::publish(const T &publication)
{
  /// @todo Re-factor to use std::for_each() algorithm
  typename SubscriberManager::ItemList::iterator
    iter = this->mSubscriberManager->getItems().begin(),
    end = this->mSubscriberManager->getItems().end();
  for (; iter != end; ++iter)
  {
    typename Subscriber::WeakPointer &weakSubscriber = *iter;
    if (weakSubscriber.expired())
    {
      iter = this->mSubscriberManager->getItems().erase(iter);
      continue;
    }
    typename Subscriber::Pointer strongSubscriber = weakSubscriber.lock();
    strongSubscriber->onNotification(publication);
  }
}

template <typename T>
bool Publisher<T>::hasSubscriber(Publisher<T>::Subscriber *subscriber)
{
  return this->mSubscriberManager->has(Subscriber::WeakPointer(subscriber));
}

template <typename T>
bool Publisher<T>::registerSubscriber(Publisher<T>::Subscriber *subscriber)
{
  return this->mSubscriberManager->add(Subscriber::WeakPointer(subscriber));
}

template <typename T>
bool Publisher<T>::unregisterSubscriber(Publisher<T>::Subscriber *subscriber)
{
  return this->mSubscriberManager->remove(Subscriber::WeakPointer(subscriber));
}
