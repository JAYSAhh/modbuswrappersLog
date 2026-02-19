#!/bin/bash

PROFILE_DIR="$HOME/.conan2/profiles"
mkdir -p "$PROFILE_DIR"

cat <<EOL > "$PROFILE_DIR/debug"
[settings]
arch=x86_64
build_type=Debug
compiler=gcc
compiler.version=12
compiler.libcxx=libstdc++11
compiler.cppstd=17
os=Linux

[conf]
tools.cmake.cmaketoolchain:generator=Ninja
EOL

cat <<EOL > "$PROFILE_DIR/release"
[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.version=12
compiler.libcxx=libstdc++11
compiler.cppstd=17
os=Linux

[conf]
tools.cmake.cmaketoolchain:generator=Ninja
EOL
