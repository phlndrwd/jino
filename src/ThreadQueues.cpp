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

  // Join all active threads safely
  for (auto& [queueId, thread] : activeThreads_) {
    if (thread.joinable() && !joinedThreads_[queueId]) {
      thread.join();
      joinedThreads_[queueId] = true;
      std::cerr << "Thread " << queueId << " joined." << std::endl;
    }
  }
}

void ThreadQueues::workerThread(std::uint64_t queueId) {
  try {
    while (true) {
      std::function<void()> task;

      {
        std::unique_lock<std::mutex> lock(queueMutex_);
        condition_.wait(lock, [this, queueId] {
          return stopAll_ || stopFlags_[queueId] || !taskQueues_[queueId].empty();
        });

        std::cerr << "[Worker] queueId: " << queueId << " - Woke up." << std::endl;

        if ((stopAll_ || stopFlags_[queueId]) && taskQueues_[queueId].empty()) {
          std::cerr << "[Worker] queueId: " << queueId << " - Stopping thread." << std::endl;
          break;
        }

        if (!taskQueues_[queueId].empty()) {
          task = std::move(taskQueues_[queueId].front());
          taskQueues_[queueId].pop();
        }
      }

      if (task) {
        try {
          std::cerr << "[Worker] queueId: " << queueId << " - Running task." << std::endl;
          task(); // Execute task
        } catch (const std::exception& e) {
          std::cerr << "[Worker] queueId: " << queueId << " - Exception: " << e.what() << std::endl;
        } catch (...) {
          std::cerr << "[Worker] queueId: " << queueId << " - Unknown exception." << std::endl;
        }
        condition_.notify_all(); // Notify that task is complete
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "[Worker] queueId: " << queueId << " - Unhandled exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "[Worker] queueId: " << queueId << " - Unhandled unknown exception." << std::endl;
  }

  // Cleanup
  std::unique_lock<std::mutex> lock(queueMutex_);
  std::cerr << "[Worker] queueId: " << queueId << " - Marking thread as stopped." << std::endl;
  joinedThreads_[queueId] = true; // Only mark it as joined
  condition_.notify_all();
  std::cerr << "[Worker] queueId: " << queueId << " - Exited cleanly." << std::endl;
}

void ThreadQueues::waitForCompletion() {
  std::unique_lock<std::mutex> lock(queueMutex_);
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
    if (stopFlags_[queueId]) {
      std::cerr << "[Stop] queueId: " << queueId << " - Already stopping." << std::endl;
      return;
    }
    stopFlags_[queueId] = true;
    std::cerr << "[Stop] queueId: " << queueId << " - Stop signal sent." << std::endl;
  }
  condition_.notify_all();  // Wake up the thread

  if (activeThreads_.count(queueId)) {
    std::cerr << "[Stop] queueId: " << queueId << " - Waiting for thread to join." << std::endl;

    // Prevent double-joining!
    if (activeThreads_[queueId].joinable() && !joinedThreads_[queueId]) {
      activeThreads_[queueId].join();
      joinedThreads_[queueId] = true;
      std::cerr << "[Stop] queueId: " << queueId << " - Thread joined." << std::endl;
    }

    // Erase only if it's really done
    if (joinedThreads_[queueId]) {
      activeThreads_.erase(queueId);
      stopFlags_.erase(queueId);
    }
  }
}

void ThreadQueues::restartThread(std::uint64_t queueId) {
  std::unique_lock<std::mutex> lock(queueMutex_);
  if (activeThreads_.find(queueId) == activeThreads_.end()) {
    stopFlags_[queueId] = false; // Reset the stop flag
    joinedThreads_[queueId] = false;
    activeThreads_[queueId] = std::thread(&ThreadQueues::workerThread, this, queueId);
    std::cerr << "Thread " << queueId << " restarted." << std::endl;
  }
}

} // namespace jino

