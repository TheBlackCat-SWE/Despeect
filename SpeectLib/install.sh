#!/bin/sh

# This scripts requires several tools in order to work, such as:
# git curl gcc cmake swig libxml2-dev python-dev.
# On a Debian system they can be installed by:
# apt-get install git curl gcc cmake swig libxml2-dev python-dev

set -e

# Detect platform
PLATFORM="`uname -m`-`uname -s`"
BASEDIR=`pwd`
echo Compiling for "$PLATFORM" on "$BASEDIR"

# Prepare directory layout
mkdir -p voices sources downloads/voices builds/"$PLATFORM" install

case "$1" in
    "download")
        # Get speect from Mivoq' github repository
        git clone https://github.com/mivoq/speect sources/speect|| test -d sources/speect/.git
        ;;
    "download_deps")
        git clone -b 1.03.x https://github.com/mivoq/hts_engine_api sources/hts_engine_api-1.03 || test -d sources/hts_engine_api-1.03/.git
        # # Get hts_engine_API-1.03 sources from sf.net
        # curl -L -o downloads/hts_engine_API-1.03.tar.gz 'https://drive.google.com/uc?export=download&id=15uEApO9hcJ60MKYm9Bgb1-r3dRvX0osN' 

        # Download voices from http://hlt.mirror.ac.za/TTS/Speect/
        # Have a look at http://speect.sourceforge.net/download.html#download
        mkdir -p downloads/voices
        for file in README.cmu_arctic_slt-1.0           \
                        README.meraka_lwazi2_alta-1.0   \
                        README.meraka_lwazi2_john-1.2   \
                        cmu_arctic_slt-1.0.tar.gz       \
                        meraka_lwazi2_alta-1.0.tar.gz   \
                        meraka_lwazi2_john-1.2.tar.gz
        do
           wget http://hlt.mirror.ac.za/TTS/Speect/"$file" -c -O downloads/voices/"$file"
        done
        ;;
    "extract_deps")
        # # Extract hts_engine_API-1.03 sources
        # tar zxvf downloads/hts_engine_API-1.03.tar.gz -C sources
	cd sources/hts_engine_api-1.03/src
	autoreconf -ifs
	cd "$BASEDIR"

        # Extract voices
        mkdir -p voices
        for file in downloads/voices/*.tar.gz
        do
            tar zxvf "$file" -C voices
        done
        ;;
    "build_deps")
        # Compile and install hts_engine_API-1.03
        mkdir -p builds/"$PLATFORM"/hts_engine_API-1.03
        cd builds/"$PLATFORM"/hts_engine_API-1.03
        CFLAGS="-fPIC" "$BASEDIR"/sources/hts_engine_api-1.03/src/configure --prefix="$BASEDIR"/install
        make -j
        make install
        cd "$BASEDIR"
        ;;
    "build")
        # Compile and install speect
        BASEDIR=`pwd`
        mkdir -p builds/"$PLATFORM"/speect
        cd builds/"$PLATFORM"/speect
	if [ -z "$LD_LIBRARY_PATH" ]; then
	    LD_LIBRARY_PATH="$BASEDIR"/builds/"$PLATFORM"/speect/engine/lib
	else
	    LD_LIBRARY_PATH="$LD_LIBRARY_PATH":"$BASEDIR"/builds/"$PLATFORM"/speect/engine/lib
	fi
        cmake "$BASEDIR"/sources/speect                                           \
              -DCMAKE_BUILD_TYPE=Debug                                            \
              -DCMAKE_INSTALL_PREFIX="$BASEDIR"/install                           \
              -DWANT_TESTS=ON                                                     \
              -DWANT_EXAMPLES=ON                                                  \
              -DERROR_HANDLING=ON                                                 \
              -DHTS_ENGINE_INCLUDE_103:PATH="$BASEDIR"/install/include            \
              -DHTS_ENGINE_LIB_103:FILEPATH="$BASEDIR"/install/lib/libHTSEngine.a \
	      -DWANT_PYTHON_WRAPPER=NO -DNO_SWIG=YES 
        make -j
	make -j test
        make -j install
        cd "$BASEDIR"
        ;;
    "clean")
	rm -fr builds
	rm -fr install
	rm -f tmp.wav*
	;;
    "run")
	if [ -z "$LD_LIBRARY_PATH" ]; then
            LD_LIBRARY_PATH="$BASEDIR"/install/lib
        else
            LD_LIBRARY_PATH="$LD_LIBRARY_PATH":"\
$BASEDIR"/install/lib
        fi
	"$BASEDIR"/builds/"$PLATFORM"/speect/engine/tests/speect_test -t "Hello world!" -v "$BASEDIR"/voices/cmu_arctic_slt/voice.json -m text -o tmp.wav
	;;
    *)
        "$0" download_deps
        "$0" extract_deps
	"$0" build_deps
        "$0" download
        "$0" build
        "$0" run
        ;;
esac

