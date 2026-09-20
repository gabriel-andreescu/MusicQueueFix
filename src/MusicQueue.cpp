#include "MusicQueue.h"

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/Logger.h>

#include <algorithm>
#include <memory>

namespace {
using RemoveQueueEntry = bool(RE::BSTArray<RE::BSIMusicType*>*, const RE::BSMusicEvent* const*);

bool IsRemoval(const RE::BSMusicEvent& a_event) {
    using MessageType = RE::BSMusicEvent::MUSIC_MESSAGE_TYPE;
    return a_event.msgType == MessageType::kRemove || a_event.msgType == MessageType::kRemoveImmediate;
}

void RemoveOneQueueEntry(RE::BSTArray<RE::BSIMusicType*>& a_queue, const RE::BSMusicEvent& a_event) {
    static const REL::Relocation<RemoveQueueEntry> remove {REL::VariantID(87953, 90326, 0x1100D80)};
    // The helper dereferences a pointer to a cursor, then reads the music pointer from its first field.
    const auto* event = std::addressof(a_event);
    remove(std::addressof(a_queue), std::addressof(event));
}

struct ProcessEventHook {
    static RE::BSEventNotifyControl Thunk(
        RE::BSMusicManager* a_manager,
        const RE::BSMusicEvent* a_event,
        RE::BSTEventSource<RE::BSMusicEvent>* a_eventSource
    ) {
        const auto* music = a_event->musicType;
        if (IsRemoval(*a_event) && music != nullptr) {
            const RE::BSSpinLockGuard guard(MusicQueueFix::MusicQueue::GetLock());
            if (a_manager->current
                == music
                && music->flags.all(RE::BSIMusicType::MST::kRemovalQueued)
                && std::ranges::count(a_manager->musicQueue, music)
                > 1) {
                RemoveOneQueueEntry(a_manager->musicQueue, *a_event);
            }
            // ProcessEvent uses this recursive lock, so forwarding while held keeps the removal atomic.
            return original(a_manager, a_event, a_eventSource);
        }
        return original(a_manager, a_event, a_eventSource);
    }

    static inline REL::Relocation<decltype(Thunk)> original;
};
}

RE::BSSpinLock& MusicQueueFix::MusicQueue::GetLock() {
    static const REL::Relocation<RE::BSSpinLock*> lock {REL::VariantID(525734, 412215, 0x31A1530)};
    return *lock;
}

void MusicQueueFix::MusicQueue::Install() {
    REL::Relocation vtable {RE::VTABLE_BSMusicManager[0]};
    ProcessEventHook::original = vtable.write_vfunc(1, ProcessEventHook::Thunk);
    SKSE::log::info("Installed music queue fix");
}
