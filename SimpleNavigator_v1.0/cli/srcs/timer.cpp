#include "../includes/timer.hpp"

namespace s21{

void Timer::Start(){
    start_ = std::chrono::steady_clock::now();
}

void Timer::End(){
    end_ = std::chrono::steady_clock::now();
}

long long Timer::GetDuration(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_ -
                                                                start_).count();
}

}
