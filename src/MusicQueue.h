#pragma once

namespace RE {
class BSSpinLock;
}

namespace MusicQueueFix::MusicQueue {
RE::BSSpinLock& GetLock();
void Install();
}
