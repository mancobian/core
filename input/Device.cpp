#include "Device.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

DECLARE_SINGLETON(RSSD::Core::Input::Device::Factory::Manager);
template <typename T> const uint_t RSSD::Core::Input::BaseDevice<T>::TYPE = Device::Factory::Impl<T>::TYPE;
