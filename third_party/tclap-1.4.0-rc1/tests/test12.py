#!/usr/bin/python

import simple_test

simple_test.test("test2", ["-i", "10", "-s", "hello", "-f", "nine", ],
                 expect_fail=True)
