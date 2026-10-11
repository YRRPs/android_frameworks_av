/*
 * Copyright (C) 2026 The YRRP Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_SERVERS_CAMERA3_STREAM_TIMESTAMP_CHECK_H
#define ANDROID_SERVERS_CAMERA3_STREAM_TIMESTAMP_CHECK_H

#include <cstdint>

namespace android {

namespace camera3 {

// True when a returned output buffer breaks the stream's increasing-timestamp rule.
inline bool isNonIncreasingTimestamp(bool timestampIncreasing, int64_t timestamp,
        int64_t lastTimestamp, bool bufferHasError) {
    return timestampIncreasing && timestamp != 0 && timestamp <= lastTimestamp && !bufferHasError;
}

// Whether a buffer that breaks the increasing-timestamp rule is returned as an error buffer.
inline constexpr bool failsBufferOnNonIncreasingTimestamp() {
#ifdef OPLUS_CAMERA
    // Stock OxygenOS logs the timestamp and returns the buffer intact, on every output stream.
    // The OPlus camera's still-capture brackets appear to reuse older ZSL-style frames without
    // setting ANDROID_CONTROL_ENABLE_ZSL. An error buffer reaches the app as
    // onCaptureBufferLost, its multi-frame merge waits for a frame that never arrives, and the
    // photo is not saved.
    return false;
#else
    return true;
#endif
}

} // namespace camera3

} // namespace android

#endif
