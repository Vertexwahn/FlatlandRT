load(":rule.bzl", _test = "test")

visibility("public")

e2e_symlink_test = _test
