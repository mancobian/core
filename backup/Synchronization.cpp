#include "Synchronization.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Concurrency;

DECLARE_SINGLETON(RSSD::Core::Concurrency::Synchronization::Manager);
DECLARE_SINGLETON(RSSD::Core::Concurrency::Event::Factory);
DECLARE_SINGLETON(RSSD::Core::Concurrency::CriticalSection::Factory);
