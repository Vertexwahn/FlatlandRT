// copied from: https://github.com/google/glog/blob/master/bazel/example/main.cc

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"

#include <string>
#include <fstream>

#include "include/gmock/gmock.h"

TEST(glog, log_message) {
    FLAGS_logtostderr = true;
    google::SetLogDestination(google::GLOG_INFO,"lovelyGoogle.txt");

    const char *argv[] = {"myProgramm", nullptr};
    //int argc = sizeof(argv) / sizeof(char*) - 1;

    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // Optional: parse command line flags
    //gflags::ParseCommandLineFlags(&argc, &argv, true);

    LOG(INFO) << "Hello, world!";

    // glog/stl_logging.h allows logging STL containers.
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    LOG(INFO) << "ABC, it's easy as " << x;

    std::ifstream file("lovelyGoogle.txt");
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    const char* test_tmpdir = std::getenv("TEST_TMPDIR");

    ASSERT_STRNE(nullptr, test_tmpdir);

    //EXPECT_THAT(str, testing::HasSubstr("ABC, it's easy as"));
}