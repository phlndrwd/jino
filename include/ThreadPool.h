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

#ifndef INCLUDE_THREADPOOL_H_
#define INCLUDE_THREADPOOL_H_

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace jino {
class ThreadPool {
public:
  ThreadPool(std::uint64_t);

  ~ThreadPool();

  template<class F>
  void enqueue(F&& f) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
  }

  void waitForCompletion();

private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable condition;

  int completedTasks;
  bool stop;
};
}  // namespace jino

#endif // INCLUDE_THREADPOOL_H_
