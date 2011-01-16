#include "Core"

namespace RSSD {
namespace Core {

DECLARE_SINGLETON(Utilities::Timer::Manager);
DECLARE_SINGLETON(Utilities::BasicTimer::Factory);

DECLARE_SINGLETON(Concurrency::Task::Manager);
DECLARE_SINGLETON(Concurrency::BasicScheduler);

DECLARE_SINGLETON(Input::Device::Manager);
DECLARE_SINGLETON(Input::BasicInputManager);
DECLARE_SINGLETON(Input::BasicMouse::Factory);
DECLARE_SINGLETON(Input::BasicKeyboard::Factory);

void create()
{
  /// Timer
  new Utilities::Timer::Manager();
  new Utilities::BasicTimer::Factory();

  /// Concurrency
  new Concurrency::Task::Manager();
  new Concurrency::BasicScheduler();

  /// Input
  new Input::Device::Manager();
  new Input::BasicInputManager();
  new Input::BasicMouse::Factory();
  new Input::BasicKeyboard::Factory();
}

void destroy()
{
  /// Timer
  delete Utilities::Timer::Manager::getPointer();

  /// Input
  delete Input::BasicInputManager::getPointer();
  delete Input::Device::Manager::getPointer();
}

} /// namespace Core
} /// namespace RSSD
