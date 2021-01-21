/*
 * timer.cpp
 *  timer sample of libev
 *
 * */

#include <ev++.h>

#include <iostream>

class callback_t {
    private:
        double timeout_;

    public:
        callback_t(const double& timeout)
            : timeout_(timeout) { }

    void awake(ev::timer& timer, int) noexcept {
        std::cout
            << "timer: " << timeout_ << "sec"
            << std::endl;
        timer.set(timeout_);
        timer.start();
    }
};

int main(const int, const char* const []) {
    // timer settings
    constexpr double timeout1 = 0.500000;
    constexpr double timeout2 = 0.700000;

    // loop
    ev::default_loop loop;

    // generate and setup timers
    ev::timer timer1(loop);
    callback_t callback1(timeout1);
    timer1.set<callback_t, &callback_t::awake>(&callback1);
    timer1.set(timeout1);
    timer1.start();

    ev::timer timer2(loop);
    callback_t callback2(timeout2);
    timer2.set<callback_t, &callback_t::awake>(&callback2);
    timer2.set(timeout2);
    timer2.start();

    // run
    loop.run(0);

    return 0;
}