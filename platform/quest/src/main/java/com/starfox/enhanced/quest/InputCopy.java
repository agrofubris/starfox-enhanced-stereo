package com.starfox.enhanced.quest;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/** Bounded streaming import; the caller owns streams and commit/rollback. */
final class InputCopy {
    static long copy(InputStream source, OutputStream destination, long limit) throws IOException {
        if (limit <= 0) throw new IllegalArgumentException("Invalid import limit");
        byte[] buffer = new byte[65536];
        long total = 0;
        for (;;) {
            int count = source.read(buffer);
            if (count < 0) break;
            if (count == 0) {
                int value = source.read();
                if (value < 0) break;
                buffer[0] = (byte) value;
                count = 1;
            }
            if (count > limit - total) throw new IOException("Selected file exceeds the import size limit");
            destination.write(buffer, 0, count);
            total += count;
        }
        if (total == 0) throw new IOException("Selected file is empty");
        return total;
    }
}
