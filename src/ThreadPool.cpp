/**********************************************************************************************
* JSON In NetCDF Out                                                                          *
*                                                                                             *
* (C) Copyright 2025, Phil Underwood.                                                         *
*                                                                                             *
* This program is free software: you can redistribute it and/or modify it under the terms of  *
* the GNU General Public License as published by the Free Software Foundation, either version *
* 3 of the License, or (at your option) any later version.                                    *
*                                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;   *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   *
* See the GNU General Public License for more details.                                        *
*                                                                                             *
* You should have received a copy of the GNU General Public License along with this program.  *
* If not, see <https://www.gnu.org/licenses/>.                                                *
**********************************************************************************************/

#include "ThreadPool.h"

#include <utility>

jino::ThreadPool::ThreadPool(std::uint64_t numThreads) : stop_(false), completedTasks_(0) {
  for (size_t i = 0; i < numThreads; ++i) {
    workers_.emplace_back([this] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queueMutex_);
          condition_.wait(lock, [this] {
            return stop_ || !tasks_.empty();
          });
          if (stop_ && tasks_.empty()) return;
          task = std::move(tasks_.front());
          tasks_.pop();
          ++completedTasks_;
        }
        task();
        {
          std::lock_guard<std::mutex> lock(queueMutex_);
          completedTasks_ = 0;
        }
        condition_.notify_all();
      }
    });
  }
}

jino::ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread& worker : workers_) {
    worker.join();
  }
}

void jino::ThreadPool::waitForCompletion() {
  std::unique_lock<std::mutex> lock(queueMutex_);
  condition_.wait(lock, [this] {
    return completedTasks_ == 0;
  });
}
