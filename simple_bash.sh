#!/bin/bash

set -e
set -u

echo_stderr() { echo "$@" 1>&2; }

function print_usage()
{
{
    cat << EOF
    case $0
        --gdb run under gdb
        --cgdb run under cgdb
        --dry-run dont run, just print out the args it would run with
        -h | --help) print usage
EOF
} 1>&2
}

PROCESS_ARG=""


while [[ ${1:-} ]]
do
   case "$1" in
        --gdb) GDB=1; shift 1;;
        --cgdb) CGDB=1; shift 1;;
        --dry-run) DRY_RUN=1; shift 1;;
        -h | --help) print_usage; shift 1; exit 1;;
        *) echo "$0 does not recognize option $1"; print_usage; shift 1; exit -1;;
   esac
done

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
HOST=$(hostname -s)

DEBUGGER_STARTUP_COMMAND='-ex "b main" -ex "directory /source/code/path" -ex -r --args '

if [[ ${GDB:-} ]]; then
    echo Debugging...
    RUN_PREFIX="gdb $DEBUGGER_STARTUP_COMMAND"
else
    if [[ ${CGDB:-} ]]; then
    RUN_PREFIX="cgdb $DEBUGGER_STARTUP_COMMAND"
    fi
fi

CMD=" exec ${RUN_PREFIX}:-} ls -lrth"

echo
echo -e "$CMD"
echo

if [[ ! ${DRY_RUN:-} ]]; then
    eval "$CMD"
fi
