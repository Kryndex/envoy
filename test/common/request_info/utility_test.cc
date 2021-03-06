#include "common/request_info/utility.h"

#include "test/mocks/request_info/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using testing::NiceMock;
using testing::Return;
using testing::_;

namespace Envoy {
namespace RequestInfo {

TEST(ResponseFlagUtilsTest, toShortStringConversion) {
  std::vector<std::pair<ResponseFlag, std::string>> expected = {
      std::make_pair(ResponseFlag::FailedLocalHealthCheck, "LH"),
      std::make_pair(ResponseFlag::NoHealthyUpstream, "UH"),
      std::make_pair(ResponseFlag::UpstreamRequestTimeout, "UT"),
      std::make_pair(ResponseFlag::LocalReset, "LR"),
      std::make_pair(ResponseFlag::UpstreamRemoteReset, "UR"),
      std::make_pair(ResponseFlag::UpstreamConnectionFailure, "UF"),
      std::make_pair(ResponseFlag::UpstreamConnectionTermination, "UC"),
      std::make_pair(ResponseFlag::UpstreamOverflow, "UO"),
      std::make_pair(ResponseFlag::NoRouteFound, "NR"),
      std::make_pair(ResponseFlag::DelayInjected, "DI"),
      std::make_pair(ResponseFlag::FaultInjected, "FI"),
      std::make_pair(ResponseFlag::RateLimited, "RL")};

  for (const auto& testCase : expected) {
    NiceMock<MockRequestInfo> request_info;
    ON_CALL(request_info, getResponseFlag(testCase.first)).WillByDefault(Return(true));
    EXPECT_EQ(testCase.second, ResponseFlagUtils::toShortString(request_info));
  }

  // No flag is set.
  {
    NiceMock<MockRequestInfo> request_info;
    ON_CALL(request_info, getResponseFlag(_)).WillByDefault(Return(false));
    EXPECT_EQ("-", ResponseFlagUtils::toShortString(request_info));
  }

  // Test combinations.
  // These are not real use cases, but are used to cover multiple response flags case.
  {
    NiceMock<MockRequestInfo> request_info;
    ON_CALL(request_info, getResponseFlag(ResponseFlag::DelayInjected)).WillByDefault(Return(true));
    ON_CALL(request_info, getResponseFlag(ResponseFlag::FaultInjected)).WillByDefault(Return(true));
    ON_CALL(request_info, getResponseFlag(ResponseFlag::UpstreamRequestTimeout))
        .WillByDefault(Return(true));
    EXPECT_EQ("UT,DI,FI", ResponseFlagUtils::toShortString(request_info));
  }
}

} // namespace RequestInfo
} // namespace Envoy
