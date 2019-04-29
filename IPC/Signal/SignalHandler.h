#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <csignal>
#include <cstdio>
#include <memory.h>

#include "EventHandler.h"

class SignalHandler {

private:
    static SignalHandler *instance;
    static EventHandler *signalHandlers[NSIG];

    SignalHandler() = default;

    static void dispatcher(int signum);

public:
    static SignalHandler *getInstance();

    static void destroy();

    EventHandler *registerHandler(int signum, EventHandler *eventHandler);

    int removeHandler(int signum);

};

#endif /* SIGNALHANDLER_H_ */
