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

namespace jino {
class ThreadQueues {
public:
  ThreadQueues();
  ~ThreadQueues();

  template<class F>
  void enqueue(std::uint64_t queueId, F&& f) {
    {
      std::unique_lock<std::mutex> lock(queueMutex_);
      taskQueues_[queueId].emplace(std::forward<F>(f));
      if (activeThreads_.find(queueId) == activeThreads_.end()) {
        activeThreads_[queueId] = std::thread(&ThreadQueues::workerThread, this, queueId);
        joinedThreads_[queueId] = false;
      }
    }
    condition_.notify_all();
  }

  void stopThreads();
  void stopThread(std::uint64_t queueId);
  void restartThread(std::uint64_t queueId);

private:
  void workerThread(std::uint64_t queueId);

  std::map<std::uint64_t, std::queue<std::function<void()>>> taskQueues_;
  std::map<std::uint64_t, std::thread> activeThreads_;
  std::map<std::uint64_t, bool> stopFlags_;
  std::map<std::uint64_t, bool> joinedThreads_;
  std::mutex queueMutex_;
  std::condition_variable condition_;
  std::uint8_t stopAll_ = false;
};
} // namespace jino

#endif // INCLUDE_THREADQUEUES_H_
