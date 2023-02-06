#include <chrono>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <unistd.h>
#include <random>

std::mutex m, m2;
std::condition_variable cv, cv2;
bool ready, ready2;
int input_size, count, countCompute, countComputeCompute;

int main(int argc, char** argv){
  std::string input;
  for (int i = 1; i < argc; i++) {
    input += std::string(argv[i]) + ' ';
  }
  input.pop_back();
  input_size = input.size();
  countCompute = 0;
  std::cout << input << std::endl;
  auto l = [=] (std::string input) {
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(input), std::end(input), rng);
    ready = false;
    std::vector<std::thread> threads(0);
    for (int i = 0; i < input.size(); i++) {
      auto t = std::thread([=](std::string input, int index, int* res){
          std::unique_lock lk(m);
          cv.wait(lk, []{return ready;});
          countComputeCompute = 0;
          char c = input[index];
          ready2 = false;
          std::vector<std::thread> localThreads(0);
          std::vector<char> chars = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
          bool res2 = false;
          for (int i = 0; i < 255; i++) {
          auto t = std::thread([=] (char from, char target, std::vector<char> chars, bool* value) {
              std::unique_lock lk(m2);
              cv2.wait(lk, []{return ready2;});
              std::sort(chars.begin(), chars.end());

              std::cout << ("\r[" + ((std::to_string(countComputeCompute).size() < 3) ? std::string(3 - std::to_string(countComputeCompute).size(), ' ') + std::to_string(countComputeCompute) : std::to_string(countComputeCompute)) + "/255|" + ((std::to_string(countCompute).size() < 3) ? std::string(3 - std::to_string(countCompute).size(), ' ') + std::to_string(countCompute) : std::to_string(countCompute))
                  + "/" + std::to_string(input_size - 1) + "] Computing " + std::string(1, c) + " ..." ) << std::flush;
              (*value) |= (std::find(chars.begin(), chars.end(), from) != chars.end() && from == target);
              usleep(5e2);
              countComputeCompute++;
              lk.unlock();
              cv2.notify_all();
              }, c, (char)i, chars, &res2);
          localThreads.push_back(std::move(t));
          }
          ready2 = true;
          cv2.notify_all();
          for (auto &localThread : localThreads) {
            localThread.join();
          }

          if (res2) 
            (*res)++;
          std::cout << ((res2 ? "\x1B[32m" : "\x1B[31m") + std::string("\r[") + ((std::to_string(countComputeCompute).size() < 3) ? std::string(3 - std::to_string(countComputeCompute).size(), ' ') + std::to_string(countComputeCompute) : std::to_string(countComputeCompute)) + "/255|" + ((std::to_string(countCompute).size() < 3) ? std::string(3 - std::to_string(countCompute).size(), ' ') + std::to_string(countCompute) : std::to_string(countCompute)) + "/" + std::to_string(input_size - 1) + "] Computing " + std::string(1, c) + " ... done!" + (res2 ? " (found)" : "") + "\x1B[0m") << std::endl;
          countCompute++;
          fflush(stdout);
          lk.unlock();
          cv.notify_all();
      },input, i, &count);
      threads.push_back(std::move(t));
    }
    ready = true;
    cv.notify_all();
    for (auto &thread : threads) {
      thread.join();
    }
    return count;
  };

  std::cout << l(input) << std::endl;
  return EXIT_SUCCESS;
}
