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

#define LOG_TAG "Camera3StreamTimestampCheckTest"

#include <gtest/gtest.h>

#include "../device3/Camera3StreamTimestampCheck.h"

using android::camera3::failsBufferOnNonIncreasingTimestamp;
using android::camera3::isNonIncreasingTimestamp;

TEST(Camera3StreamTimestampCheckTest, EqualOrOlderTimestampIsNonIncreasing) {
    EXPECT_TRUE(isNonIncreasingTimestamp(/*timestampIncreasing*/ true, /*timestamp*/ 100,
            /*lastTimestamp*/ 200, /*bufferHasError*/ false));
    EXPECT_TRUE(isNonIncreasingTimestamp(true, 200, 200, false));
}

TEST(Camera3StreamTimestampCheckTest, NewerTimestampIsIncreasing) {
    EXPECT_FALSE(isNonIncreasingTimestamp(true, 201, 200, false));
}

TEST(Camera3StreamTimestampCheckTest, SkippedWhenNotRequiredOrNotApplicable) {
    // ZSL still captures and reprocessing requests do not require increasing timestamps.
    EXPECT_FALSE(isNonIncreasingTimestamp(/*timestampIncreasing*/ false, 100, 200, false));
    // A zero timestamp means the HAL sent no shutter for this buffer.
    EXPECT_FALSE(isNonIncreasingTimestamp(true, 0, 200, false));
    // A buffer that already failed needs no second verdict.
    EXPECT_FALSE(isNonIncreasingTimestamp(true, 100, 200, /*bufferHasError*/ true));
}

TEST(Camera3StreamTimestampCheckTest, BufferFailsOnlyWithoutOplusCamera) {
#ifdef OPLUS_CAMERA
    // Stock OxygenOS logs the non-increasing timestamp but returns the buffer intact. The OPlus
    // camera's still-capture brackets appear to carry older ZSL-style timestamps without setting
    // ANDROID_CONTROL_ENABLE_ZSL, and an error buffer starves its multi-frame merge.
    EXPECT_FALSE(failsBufferOnNonIncreasingTimestamp());
#else
    EXPECT_TRUE(failsBufferOnNonIncreasingTimestamp());
#endif
}
