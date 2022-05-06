DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

MODSHOT_PREFIX=$(uname -m)
export LDFLAGS="-L$DIR/build-${MODSHOT_PREFIX}/lib"
export CFLAGS="-I$DIR/build-${MODSHOT_PREFIX}/include"
MODSHOT_OLD_PC=$(pkg-config --variable pc_path pkg-config)

# Try to load the stuff we built first
export PKG_CONFIG_LIBDIR="$DIR/build-${MODSHOT_PREFIX}/lib/pkgconfig:$DIR/build-${MODSHOT_PREFIX}/lib64/pkgconfig:${MODSHOT_OLD_PC}"
export PATH="$DIR/build-${MODSHOT_PREFIX}/bin:$PATH"
export LD_LIBRARY_PATH="$DIR/build-${MODSHOT_PREFIX}/lib:${LD_LIBRARY_PATH}"
export MODSHOT_PREFIX="$DIR/build-${MODSHOT_PREFIX}"
