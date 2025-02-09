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

jino::ThreadQueues::ThreadQueues() {}

jino::ThreadQueues::~ThreadQueues() {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    stopAll_ = true;
    for (auto& [queueId, _] : activeThreads_) {
      stopFlags_[queueId] = true;
    }
  }
  condition_.notify_all();

  for (auto& [queueId, thread] : activeThreads_) {
    if (thread.joinable() && !joinedThreads_[queueId]) {
      thread.join();
      joinedThreads_[queueId] = true;
    }
  }
}

void jino::ThreadQueues::workerThread(std::uint64_t queueId) {
  try {
    while (true) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(queueMutex_);
        condition_.wait(lock, [this, queueId] {
          return stopAll_ || stopFlags_[queueId] || !taskQueues_[queueId].empty();
        });
        if ((stopAll_ || stopFlags_[queueId]) && taskQueues_[queueId].empty()) {
          break;
        }
        if (!taskQueues_[queueId].empty()) {
          task = std::move(taskQueues_[queueId].front());
          taskQueues_[queueId].pop();
        }
      }
      if (task) {
        try {
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
  joinedThreads_[queueId] = true;
  condition_.notify_all();
}

void jino::ThreadQueues::stopThread(std::uint64_t queueId) {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    if (stopFlags_[queueId]) {
      return;
    }
    stopFlags_[queueId] = true;
  }
  condition_.notify_all();

  if (activeThreads_.count(queueId)) {
    if (activeThreads_[queueId].joinable() && !joinedThreads_[queueId]) {
      activeThreads_[queueId].join();
      joinedThreads_[queueId] = true;
    }
    if (joinedThreads_[queueId]) {
      activeThreads_.erase(queueId);
      stopFlags_.erase(queueId);
    }
  }
}

void jino::ThreadQueues::restartThread(std::uint64_t queueId) {
  std::unique_lock<std::mutex> lock(queueMutex_);
  if (activeThreads_.find(queueId) == activeThreads_.end()) {
    stopFlags_[queueId] = false;
    joinedThreads_[queueId] = false;
    activeThreads_[queueId] = std::thread(&ThreadQueues::workerThread, this, queueId);
  }
}

