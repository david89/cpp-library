#!/usr/bin/env bash

# Make sure we always run from root
cd "${0%/*}/.."

# Adapted from https://github.com/andrewseidl/githook-clang-format
format_file() {
  file="${1}"
  if [ -f $file ]; then
    clang-format -i ${1}
    git add ${1}
  fi
}

echo "Formatting files"
for file in `git diff-index --cached --name-only HEAD` ; do
  format_file "${file}"
done

echo "Running tests"
bazel test --cxxopt='-std=c++11' //string_view:string_view_test
if [ $? -ne 0 ]; then
  echo "Commit failed due to failing tests"
  exit 1
fi
