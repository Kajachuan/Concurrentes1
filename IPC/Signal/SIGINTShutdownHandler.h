#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <csignal>
#include <cassert>
#include "EventHandler.h"

class SIGINTShutdownHandler : public EventHandler {
private:
    sig_atomic_t gracefulQuit;

public:
    SIGINTShutdownHandler(): gracefulQuit(0) {
    }

    int handleSignal(int signum) override {
        assert (signum == SIGINT);
        this->gracefulQuit = 1;
        return 0;
    }

    sig_atomic_t getGracefulQuit() const {
        return this->gracefulQuit;
    }
};

#endif /* SIGINT_HANDLER_H_ */
