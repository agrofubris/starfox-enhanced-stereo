package com.starfox.enhanced.quest;

import java.util.concurrent.atomic.AtomicBoolean;

/** Keeps SDL/OpenXR ownership exclusive through cleanup, not UI delivery. */
final class SessionGate {
    private final AtomicBoolean occupied = new AtomicBoolean();

    Lease tryAcquire() {
        return occupied.compareAndSet(false, true) ? new Lease() : null;
    }

    final class Lease {
        private final AtomicBoolean closed = new AtomicBoolean();

        void close(Runnable cleanup) {
            if (!closed.compareAndSet(false, true)) return;
            try {
                cleanup.run();
            } finally {
                occupied.set(false);
            }
        }
    }
}
