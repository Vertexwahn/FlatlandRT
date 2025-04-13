#! /bin/sh

# Strict Shell
set -o errexit -o nounset

# Bazel substitutions
LAUNCHER="{{launcher}}"
BINARY="{{binary}}"
readonly LAUNCHER BINARY

# Execute!
"${LAUNCHER}" "${BINARY}" "${@}"
