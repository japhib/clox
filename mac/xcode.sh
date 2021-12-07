set -e

# cd to script directory
dir=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
pushd $dir

mkdir xcode 2>&1 >/dev/null || echo ""

pushd xcode

cmake ../.. -G "Xcode" -DCMAKE_OSX_SYSROOT=$(xcrun --show-sdk-path) -DCMAKE_C_COMPILER=/usr/bin/clang
