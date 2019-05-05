#include "SignalHandler.h"

SignalHandler *SignalHandler::instance = nullptr;
EventHandler *SignalHandler::signalHandlers[NSIG];

SignalHandler *SignalHandler::getInstance() {
    if (instance == nullptr)
        instance = new SignalHandler();

    return instance;
}

void SignalHandler::destroy() {
    if (instance != nullptr) {
        delete (instance);
        instance = nullptr;
    }
}

EventHandler *SignalHandler::registerHandler(int signum, EventHandler *eventHandler) {
    EventHandler *oldEventHandler = SignalHandler::signalHandlers[signum];
    SignalHandler::signalHandlers[signum] = eventHandler;

    struct sigaction saction{};
    memset(&saction, 0, sizeof(saction));
    saction.sa_handler = SignalHandler::dispatcher;
    sigemptyset(&saction.sa_mask);
    sigaddset(&saction.sa_mask, signum);
    sigaction(signum, &saction, nullptr);
    return oldEventHandler;
}

void SignalHandler::dispatcher(int signum) {
    if (SignalHandler::signalHandlers[signum] != nullptr)
        SignalHandler::signalHandlers[signum]->handleSignal(signum);
}

int SignalHandler::removeHandler(int signum) {
    SignalHandler::signalHandlers[signum] = nullptr;
    return 0;
}
