#include "DevBenchIntegration.h"
#include "MusicQueue.h"

#include <BMK/Skyrim/DevBench.h>
#include <RE/Skyrim.h>
#include <REL/ID.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <algorithm>
#include <cstdint>
#include <format>
#include <string>

namespace {
// The saved request array is at +0x10 in Steam 1.7.104's TESAudio singleton.
struct ScriptedMusicStateView {
    void* vtable;
    std::uint64_t unknown08;
    RE::BSTSmallArray<RE::BGSMusicType*, 2> music;
};
static_assert(sizeof(ScriptedMusicStateView) == 0x30);

std::string Inspect() {
    if (REL::Module::get().version() != REL::Version {1, 7, 104, 0}) {
        return R"({"ok":false,"error":"Music inspection requires Steam 1.7.104"})";
    }
    const auto* music = RE::TESForm::LookupByID<RE::BGSMusicType>(0x8CDA3); // MUSCombatCivilWar
    if (music == nullptr) {
        return R"({"ok":false,"error":"Civil War music is not loaded"})";
    }
    const REL::Relocation<ScriptedMusicStateView**> scriptedState {REL::ID(401264)};
    const auto& scripted = (*scriptedState)->music;
    const auto savedCount = std::ranges::count(scripted, music);

    const RE::BSSpinLockGuard guard(MusicQueueFix::MusicQueue::GetLock());
    const auto* manager = RE::BSMusicManager::GetSingleton();
    return std::format(
        R"({{"ok":true,"scripted":{},"queued":{},"current":{},"status":{}}})",
        savedCount,
        std::ranges::count(manager->musicQueue, music),
        manager->current == music,
        music->typeStatus.underlying()
    );
}

constexpr auto kInspection = BMK::Skyrim::DevBench::Inspection {
    .name = "music-queue-fix",
    .descriptor
    = R"({"readOnly":true,"description":"Civil War music requests, queue entries and playback state. Requires Steam 1.7.104."})",
    .snapshot = Inspect,
    .timeoutResponse = R"({"ok":false,"error":"Music inspection timed out waiting for the game thread"})",
    .failureResponse = R"({"ok":false,"error":"Music inspection failed. See the plugin log."})",
};
}

void MusicQueueFix::DevBenchIntegration::Register() {
    BMK::Skyrim::DevBench::RegisterInspection<kInspection>();
}
