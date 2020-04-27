#!/usr/bin/env bash

# Make sure we always run from root
cd "${0%/*}/.."

echo "Running tests"
bazel test //string_view:string_view_test
if [ $? -ne 0 ]; then
  echo "Commit failed due to failing tests"
  exit 1
fi
