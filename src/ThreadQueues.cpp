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
#include "ThreadQueues.h"
#include <iostream>

namespace jino {

ThreadQueues::ThreadQueues() {}

ThreadQueues::~ThreadQueues() {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    stopAll_ = true; // Signal all threads to stop
    for (auto& [queueId, _] : activeThreads_) {
      stopFlags_[queueId] = true; // Ensure all threads are flagged to stop
    }
  }
  condition_.notify_all(); // Notify all threads in case they are waiting

  // Join all active threads
  for (auto& [_, thread] : activeThreads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void ThreadQueues::workerThread(std::uint64_t queueId) {
  while (true) {
    std::function<void()> task;

    {
      std::unique_lock<std::mutex> lock(queueMutex_);
      condition_.wait(lock, [this, queueId] {
        return stopAll_ || stopFlags_[queueId] || !taskQueues_[queueId].empty();
      });

      // Exit condition if the application is stopping or if the specific thread is flagged to stop
      if ((stopAll_ || stopFlags_[queueId]) && taskQueues_[queueId].empty()) {
        break;
      }

      // Retrieve the next task if available
      if (!taskQueues_[queueId].empty()) {
        task = std::move(taskQueues_[queueId].front());
        taskQueues_[queueId].pop();
      }
    }

    if (task) {
      try {
        task(); // Execute the task
      } catch (const std::exception& e) {
        std::cerr << "Exception in thread " << queueId << ": " << e.what() << std::endl;
      } catch (...) {
        std::cerr << "Unknown exception in thread " << queueId << std::endl;
      }
      condition_.notify_all(); // Notify other threads that a task has completed
    }
  }

  // Clean up after thread stops
  std::unique_lock<std::mutex> lock(queueMutex_);
  activeThreads_.erase(queueId);
  stopFlags_.erase(queueId);
  condition_.notify_all();
}

void ThreadQueues::waitForCompletion() {
  std::unique_lock<std::mutex> lock(queueMutex_);
  // Wait until all queues are empty and no active threads are running
  condition_.wait(lock, [this] {
    for (const auto& [_, queue] : taskQueues_) {
      if (!queue.empty()) return false;
    }
    return activeThreads_.empty();
  });
}

void ThreadQueues::stopThread(std::uint64_t queueId) {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    stopFlags_[queueId] = true; // Signal the thread to stop
  }
  condition_.notify_all();

  if (activeThreads_.count(queueId) && activeThreads_[queueId].joinable()) {
    activeThreads_[queueId].join(); // Join the thread to ensure it has terminated
    activeThreads_.erase(queueId);
    stopFlags_.erase(queueId);
  }
}

void ThreadQueues::restartThread(std::uint64_t queueId) {
  std::unique_lock<std::mutex> lock(queueMutex_);
  if (activeThreads_.find(queueId) == activeThreads_.end()) {
    stopFlags_[queueId] = false; // Reset the stop flag
    activeThreads_[queueId] = std::thread(&ThreadQueues::workerThread, this, queueId);
  }
}

} // namespace jino

