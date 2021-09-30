set -e

# cd to script directory
dir=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
pushd $dir

mkdir out 2>&1 >/dev/null || echo ""

pushd out

cmake ../.. -G "Unix Makefiles" -DCMAKE_OSX_SYSROOT=$(xcrun --show-sdk-path) -DCMAKE_C_COMPILER=/usr/bin/clang

make -j 6
cp ./clox ../../

popd
popd