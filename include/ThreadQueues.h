/**********************************************************************************************
* Jino (JSON In NetCDF Out).                                                                  *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* Jino is free software: you can redistribute it and/or modify it under the terms of the GNU  *
* Lesser General Public License as published by the Free Software Foundation, either version  *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* Jino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without   *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the   *
* GNU Lesser General Public License for more details.                                         *
*                                                                                             *
* You should have received a copy of the GNU Lesser General Public License along with Jino.   *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#ifndef INCLUDE_THREADQUEUES_H_
#define INCLUDE_THREADQUEUES_H_

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace jino {
class ThreadQueues {
public:
  ThreadQueues(); // Constructor
  ~ThreadQueues(); // Destructor

  // Enqueue a new task to the specified queue
  template<class F>
  void enqueue(std::uint64_t queueId, F&& f) {
    {
      std::unique_lock<std::mutex> lock(queueMutex_); // Lock the queue to ensure thread safety
      taskQueues_[queueId].emplace(std::forward<F>(f)); // Add the task to the queue

      // If no thread is active for this queue, create one dynamically
      if (activeThreads_.find(queueId) == activeThreads_.end()) {
        activeThreads_[queueId] = std::thread(&ThreadQueues::workerThread, this, queueId);
      }
    }
    condition_.notify_all(); // Notify worker threads that a new task is available
  }

  void waitForCompletion(); // Wait for all tasks to complete

private:
  void workerThread(std::uint64_t queueId); // Worker thread function

  std::map<std::uint64_t, std::queue<std::function<void()>>> taskQueues_; // Map of task queues
  std::map<std::uint64_t, std::thread> activeThreads_; // Map of active worker threads
  std::mutex queueMutex_; // Mutex to protect shared resources
  std::condition_variable condition_; // Condition variable for task notification
  bool stop_ = false; // Flag to signal threads to stop
};
} // namespace jino

#endif // INCLUDE_THREADQUEUES_H_
