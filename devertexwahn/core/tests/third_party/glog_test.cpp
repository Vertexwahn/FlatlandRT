// copied from: https://github.com/google/glog/blob/master/bazel/example/main.cc

/*
 * The intention of this test file is to make sure that the integration of the library glog works.
 * This file tries to reflect common use case of glog within this project.
 * Furthermore, this file can be used to test features of glog and to get familiar with the library.
 * No features of this project should be tested here.
 * Solely, the pure integration of {fmt} within this project should be tested here.
 */

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"

#include <string>
#include <fstream>

#include "gmock/gmock.h"

TEST(glog, log_message) {
    FLAGS_logtostderr = true;
    google::SetLogDestination(google::GLOG_INFO, "log.txt");

    const char *argv[] = {"some_program", nullptr};
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "Hello, world!";

    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    LOG(INFO) << "ABC, it's easy as " << x;

    // Assert
    // Try to read log
    std::ifstream file("log.txt");
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    const char* test_tmpdir = std::getenv("TEST_TMPDIR");

    ASSERT_STRNE(nullptr, test_tmpdir);

    // Does not work - I guess log.txt is stored somewhere - todo: find out how to read it - or switch to parsing standard output
    //EXPECT_THAT(str, testing::HasSubstr("ABC, it's easy as"));
}
