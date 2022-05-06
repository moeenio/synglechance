DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

MODSHOT_PREFIX=$(ruby -e "puts ENV[\"MSYSTEM\"].downcase")

export LDFLAGS="-L$DIR/build-${MODSHOT_PREFIX}/lib -L$DIR/build-${MODSHOT_PREFIX}/bin"
export CFLAGS="-I$DIR/build-${MODSHOT_PREFIX}/include"
export PATH="$DIR/build-${MODSHOT_PREFIX}/bin:$PATH"
MODSHOT_OLD_PC=$(pkg-config --variable pc_path pkg-config)

# Try to load the stuff we built first
export PKG_CONFIG_PATH="$DIR/build-${MODSHOT_PREFIX}/lib/pkgconfig"
export MODSHOT_PREFIX="$DIR/build-${MODSHOT_PREFIX}"
