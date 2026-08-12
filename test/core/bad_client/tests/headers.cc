//
//
// Copyright 2015 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#include <grpc/grpc.h>

#include "src/core/server/server.h"
#include "src/core/util/grpc_check.h"
#include "test/core/bad_client/bad_client.h"
#include "test/core/test_util/test_config.h"
#include "gtest/gtest.h"

#define PFX_STR                      \
  "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n" \
  "\x00\x00\x00\x04\x00\x00\x00\x00\x00"

static void verifier(grpc_server* server, grpc_completion_queue* cq,
                     void* /*registered_method*/) {
  while (grpc_core::Server::FromC(server)->HasOpenConnections()) {
    GRPC_CHECK(grpc_completion_queue_next(
                   cq, grpc_timeout_milliseconds_to_deadline(20), nullptr)
                   .type == GRPC_QUEUE_TIMEOUT);
  }
}

TEST(HeadersTest, PartialHttp2HeaderPrefixes) {
  // partial http2 header prefixes
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test2) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test3) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test4) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00\x00\x01",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test5) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00\x00\x01\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test6) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00\x00\x01\x04",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test7) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr, PFX_STR "\x00\x00\x00\x01\x05",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test8) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x04\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test9) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x04\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test10) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x04\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test11) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x04\x00\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test12) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x04\x00\x00\x00\x01",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, TestAddingPrioritizationData) {
  // test adding prioritization data
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x01\x01\x24\x00\x00\x00\x01"
                           "\x00",
                           0);
}

TEST(HeadersTest, Test14) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x02\x01\x24\x00\x00\x00\x01"
                           "\x00\x00",
                           0);
}

TEST(HeadersTest, Test15) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x24\x00\x00\x00\x01"
                           "\x00\x00\x00",
                           0);
}

TEST(HeadersTest, Test16) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x24\x00\x00\x00\x01"
                           "\x00\x00\x00\x00",
                           0);
}

TEST(HeadersTest, Test17) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test18) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test19) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test20) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test21) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "\x00\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test22) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x24\x00\x00\x00\x01"
                           "\x00\x00\x00\x00\x00",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, TestLookingUpAnInvalidIndex) {
  // test looking up an invalid index
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x01\x01\x04\x00\x00\x00\x01"
                           "\xfe",
                           0);
}

TEST(HeadersTest, Test24) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x04\x00\x00\x00\x01"
                           "\x7f\x7f\x01"
                           "a",
                           0);
}

TEST(HeadersTest, Test25) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x04\x00\x00\x00\x01"
                           "\x0f\x7f\x01"
                           "a",
                           0);
}

TEST(HeadersTest, Test26) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x04\x00\x00\x00\x01"
                           "\x1f\x7f\x01"
                           "a",
                           0);
}

TEST(HeadersTest, TestNvrNotIndexedInStaticTable) {
  // test nvr, not indexed in static table
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\x01\x01"
                           "a",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test28) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\x11\x01"
                           "a",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, IllegalOpCode) {
  // illegal op code
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x01\x01\x04\x00\x00\x00\x01"
                           "\x80",
                           0);
}

TEST(HeadersTest, ParseSomeLongIndices) {
  // parse some long indices
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x02\x01\x04\x00\x00\x00\x01"
                           "\xff\x00",
                           0);
}

TEST(HeadersTest, Test31) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x00",
                           0);
}

TEST(HeadersTest, Test32) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x00",
                           0);
}

TEST(HeadersTest, Test33) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x05\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x00",
                           0);
}

TEST(HeadersTest, Test34) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x06\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x00",
                           0);
}

TEST(HeadersTest, Test35) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x07\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x80\x00",
                           0);
}

TEST(HeadersTest, Test36) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test37) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test38) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test39) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test40) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test41) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test42) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x80\x80",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test43) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x80\x80\x00",
                           0);
}

TEST(HeadersTest, OverflowOnByte4) {
  // overflow on byte 4
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x06\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x7f",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, Test45) {
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x06\x01\x04\x00\x00\x00\x01"
                           "\xff\xff\xff\xff\xff\x0f",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, OverflowAfterByte4) {
  // overflow after byte 4
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x08\x01\x04\x00\x00\x00\x01"
                           "\xff\x80\x80\x80\x80\x80\x80\x02",
                           0);
}

TEST(HeadersTest, EndOfHeadersMidOpcode) {
  // end of headers mid-opcode
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x01\x01\x04\x00\x00\x00\x01"
                           "\x01",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, DynamicTableSizeUpdateSetToDefault) {
  // dynamic table size update: set to default
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\x3f\xe1\x1f",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, DynamicTableSizeUpdateSetTooLarge) {
  // dynamic table size update: set too large
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\x3f\xf1\x1f",
                           0);
}

TEST(HeadersTest, DynamicTableSizeUpdateSetTwice) {
  // dynamic table size update: set twice
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x04\x01\x04\x00\x00\x00\x01"
                           "\x20\x3f\xe1\x1f",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, DynamicTableSizeUpdateSetThrice) {
  // dynamic table size update: set thrice
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x03\x01\x04\x00\x00\x00\x01"
                           "\x20\x20\x20",
                           0);
}

TEST(HeadersTest, NonEndingHeaderFollowedByContinuationFrame) {
  // non-ending header followed by continuation frame
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x00\x01\x00\x00\x00\x00\x01"
                           "\x00\x00\x00\x09\x04\x00\x00\x00\x01",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, NonEndingHeaderFollowedByNonContinuationFrame) {
  // non-ending header followed by non-continuation frame
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x00\x01\x00\x00\x00\x00\x01"
                           "\x00\x00\x00\x00\x04\x00\x00\x00\x01",
                           0);
}

TEST(HeadersTest,
     NonEndingHeaderFollowedByAContinuationFrameForADifferentStream) {
  // non-ending header followed by a continuation frame for a different stream
  //
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x00\x01\x04\x00\x00\x00\x01"
                           "\x00\x00\x00\x01\x00\x00\x00\x00\x03"
                           "\x00\x00\x00\x09\x04\x00\x00\x00\x01",
                           0);
}

TEST(HeadersTest, OpeningWithAContinuationFrame) {
  // opening with a continuation frame
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x09\x04\x00\x00\x00\x01", 0);
}

TEST(HeadersTest, ThreeHeaderFrames) {
  // three header frames
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x00\x01\x04\x00\x00\x00\x01"
                           "\x00\x00\x00\x01\x04\x00\x00\x00\x01"
                           "\x00\x00\x00\x01\x04\x00\x00\x00\x01",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, AnInvalidHeaderFoundWithFuzzing) {
  // an invalid header found with fuzzing
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR "\x00\x00\x00\x01\x39\x67\xed\x1d\x64",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, ABadlyEncodedTimeoutValue) {
  // a badly encoded timeout value
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x19\x01\x04\x00\x00\x00\x01"
                           "\x10\x0cgrpc-timeout\x0a"
                           "15 seconds",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

TEST(HeadersTest, ABadlyEncodedTimeoutValueTwiceCatchesCaching) {
  // a badly encoded timeout value: twice (catches caching)
  GRPC_RUN_BAD_CLIENT_TEST(verifier, nullptr,
                           PFX_STR
                           "\x00\x00\x19\x01\x04\x00\x00\x00\x01"
                           "\x10\x0cgrpc-timeout\x0a"
                           "15 seconds"
                           "\x00\x00\x19\x01\x04\x00\x00\x00\x03"
                           "\x10\x0cgrpc-timeout\x0a"
                           "15 seconds",
                           GRPC_BAD_CLIENT_DISCONNECT);
}

int main(int argc, char** argv) {
  grpc::testing::TestEnvironment env(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  grpc_init();
  int result = RUN_ALL_TESTS();
  grpc_shutdown();
  return result;
}
