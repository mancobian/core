#include "concurrency/tbb/TbbScheduler.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Concurrency;
using namespace RSSD::Core::Concurrency::Impl;

///
/// @class TbbScheduler
///

TbbScheduler::TbbScheduler() :
  mIsGraphDirty(false)
{

}

TbbScheduler::~TbbScheduler()
{
  this->clear();
}

bool TbbScheduler::registerTask(const TbbScheduler::TaskType::Pointer task)
{
  /// Local vars
  bool result = true;
  TaskMap::accessor taskAccessor;
  TaskDependencyMap::accessor dependAccessor;

  /// Add to task collection
  assert (this->mTasks.insert(taskAccessor, task->getType()));
  taskAccessor->second = task;

  /// Add task as a parent to dependency collection
  assert (this->mDependencies.insert(dependAccessor, task->getType()));
  dependAccessor->second = TaskMap();

  /// Add to task as a child (dependent) to dependents collection (optional)
  if (task->getDependency())
  {
    this->mDependencies.insert(dependAccessor, task->getDependency());
    TaskMap &taskMap = dependAccessor->second;
    if (taskMap.insert(taskAccessor, task->getType())) { taskAccessor->second = task; }
  }

  /// Update graph dirty flag
  this->setIsGraphDirty(true);
  return result;
}

bool TbbScheduler::unregisterTask(const TbbScheduler::TaskType::IdType taskType)
{
  /// Local vars
  TaskMap::accessor taskAccessor;
  TaskMap::accessor eraseTaskAccessor;
  TaskDependencyMap::accessor dependAccessor;

  /// Remove from task collection
  if (!this->mTasks.find(taskAccessor, taskType)) { return false; }
  TaskType::Pointer task(taskAccessor->second);

  /// Remove from task dependents collection (optional)
  if (task->getDependency())
  {
    if (!this->mDependencies.find(dependAccessor, task->getDependency())) { return false; }
    TaskMap &taskMap = dependAccessor->second;
    if (!taskMap.find(eraseTaskAccessor, task->getType())) { return false; }
    if (!taskMap.erase(eraseTaskAccessor)) { return false; }
  }

  /// Update graph dirty flag
  this->setIsGraphDirty(true);
  return true;
}

void TbbScheduler::schedule()
{
  /// Local vars
  TaskMap::accessor taskAccessor;
  TaskDependencyMap::iterator
    dependencyIter = this->mDependencies.begin(),
    dependencyEnd = this->mDependencies.end();
  TaskMap::iterator taskIter, taskEnd;

  /// For each task with registered dependency(s)...
  for (; dependencyIter != dependencyEnd; ++dependencyIter)
  {
    /// Get the parent task
    if (!this->mTasks.find(taskAccessor, dependencyIter->first)) { continue; }
    TaskType::Pointer parentTask(taskAccessor->second);

    /// Create task graph edges
    TaskMap &taskMap = dependencyIter->second;
    if (taskMap.empty())
    {
      /// Task has no dependency(s)
      tbb::executable_node<TaskType::InputType> head(this->mGraph, parentTask->getFunctor());
      tbb::make_edge(this->mRootNode, head);
    }
    else
    {
      /// Task has dependency(s)
      TaskMap::iterator
        taskIter = taskMap.begin(),
        taskEnd = taskMap.end();
      for (; taskIter != taskEnd; ++taskIter)
      {
        TaskType::Pointer childTask(taskIter->second);
        tbb::executable_node<TaskType::InputType> tail(this->mGraph, parentTask->getFunctor());
        tbb::executable_node<TaskType::InputType> head(this->mGraph, childTask->getFunctor());
        tbb::make_edge(tail, head);
      }
    }
  }

  /// Update graph dirty flag
  this->setIsGraphDirty(false);
}

void TbbScheduler::run(TaskType::InputType input, const bool wait)
{
  if (this->getIsGraphDirty()) { this->schedule(); }
  this->mRootNode.try_put(input);
  if (wait) { this->mGraph.wait_for_all(); }
}

void TbbScheduler::clear()
{
  this->mTasks.clear();
  this->mDependencies.clear();
  this->setIsGraphDirty(false);
}
