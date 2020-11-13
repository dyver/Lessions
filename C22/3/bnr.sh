#!/bin/bash

if [ -d /mingw32 ] || [ -d /mingw64 ]
then
    readonly leaks_checker="/c/Program Files (x86)/Dr. Memory/bin/drmemory.exe"
else
    readonly leaks_checker="valgrind"
fi

readonly name=$(basename ${PWD})
readonly sources=$(ls *.cpp)
readonly destination=${name}.exe
# readonly compiler="/c/Temp/mingw64/bin/c++"
readonly compiler="c++"
readonly standart=c++17
readonly strict="-pedantic -pedantic-errors -Wall -Wextra -Werror"
# readonly optimization="-O2 -flto -Wl,--strip-all,--gc-sections"
readonly includes="-I."
# readonly no_dependencies="-static"
readonly debug="-g"
readonly flags="--std=${standart} ${strict} ${optimization} ${includes} ${no_dependencies} ${debug}"

PATH=.:${PATH}

cleanup() {
    clear
    rm -f ${destination}
}

build() {
    echo "Compiling..."
    ${compiler} ${sources} ${flags} -o ${destination}
    [ $? == 0 ] && echo "Done."
}

run() {
    ${destination} "${@}"
}

check_leaks() {
    "${leaks_checker}" ${destination} "${@}"
}

debug() {
    gdb ${destination} "${@}" <<MARK
        run
        bt
MARK
}

report() {
    echo -e "\nExecutable file type:\n"
    file -i ${destination}
    echo -e "\nExecutable file dependencies:\n"
    ldd ./${destination}
}

cleanup
build
[ ${?} != 0 ] && exit 1
# time run
check_leaks
# debug
# report
