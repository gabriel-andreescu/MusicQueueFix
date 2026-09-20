#include "DevBenchIntegration.h"
#include "MusicQueue.h"

#include <SKSE/SKSE.h>

namespace {
void MessageHandler(SKSE::MessagingInterface::Message* a_message) { // NOLINT(misc-const-correctness)
    if (a_message->type == SKSE::MessagingInterface::kPostPostLoad) {
        MusicQueueFix::DevBenchIntegration::Register();
    }
}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_extender) {
    SKSE::Init(a_extender, {.logPattern = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v"});

    MusicQueueFix::MusicQueue::Install();
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    return true;
}
