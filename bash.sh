#!/bin/bash
set -e
set -u
set -o pipefail

#########################################################################
# script_name: cme_om_server
# by: hashburner on qcl-syd-hashburner01
# Friday 19 May  11:22:21 AEST 2017
#########################################################################

echo_stderr() { echo "$@" 1>&2; }

function write_help()
{
{
    cat << EOF
    usage $0
		-P | --trading-process-config) path to process config file
		-N | --process-name) specify process name as found in the process config
		-T | --trade-date) override trade date
		-U | --universe-input) path to universe file
		-C | --config) path to fix config file
		-m | --msgw_config) path to msgw config file
		-d | --server) path to server defintion file
		-i | --interface) eg enp132s0f1 for A side
		-s | --secondary-interface) eg enp132s0f0 for B side
        -l | --log) logging , e.g. 'english:1 @ info'
        --gdb run under gdb
        --cgdb run under cgdb
        --dry-run dont run, just print out the args it would run with
        -h | --help) this help

EOF
} 1>&2
}


while [[ ${1:-} ]]
do
    case "$1" in
		-P | --trading-process-config)TRADING_PROCESS_ARG="$2"; shift 2;;
		-N | --process-name) PROCESS_NAME_ARG="$2"; shift 2;;
		-T | --trade-date) TRADEDATE_ARG="$2"; shift 2;;
		-U | --universe-input) REFDATA="$2"; shift 2;;
		-C | --config) MSGW_FIX_CONFIG="$2"; shift 2;;
		-m | --msgw_config) MSGW_CONFIG="$2"; shift 2;;
		-d | --server) SERVER_DEF="$2"; shift 2;;
		-i | --interface) INTERFACE="$2"; shift 2;;
		-a | --adapter-timestamps) ADAPTER_TS_ARG="$1"; shift 1;;
		-s | --secondary-interface) SECONDARY_INTERFACE_ARG="--secondary-interface $2"; shift 2;;
        -l | --log) LOG_ARG="-l \'$2\'"; shift 2;;
        --gdb) GDB=1; shift 1;;
        --cgdb) CGDB=1; shift 1;;
        --dry-run) DRY_RUN=1; shift 1;;
        -h | --help) write_help; shift 1; exit 1;;
        *)    echo "$0 does not recognize option $1"; write_help; shift 1; exit 65;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
HOST=$(hostname -s)

if ! [[ ${PROCESS_NAME_ARG:-} ]]; then
    echo_stderr "must supply a process name with the -N or --process-name switch"
    exit 65
fi

if ! [[ ${INTERFACE:-} ]]; then
    echo_stderr "must supply a network interface with the -i or --interface switch"
    exit 65
fi

# variables with values specific to the current machine only

if [[ -e "${SCRIPT_DIR}/environment.${HOST}" ]]; then
    source "${SCRIPT_DIR}/environment.${HOST}"
fi

# any variables not set by the machine specific environment above will take the
# defaults from below, machine specific is first incase you want to have a
# non-default root directory the below will use to build the rest of the paths

if [[ -e "${SCRIPT_DIR}/environment" ]]; then
    source "${SCRIPT_DIR}/environment"
fi


if [[ ! ${SERVER_DEF_ARG:-} ]]; then
    SERVER_DEF_ARG=${SERVER_DEF}
fi

if [[ ! ${TRADEDATE_ARG:-} ]]; then
    TRADEDATE_ARG=${TRADE_DATE}
fi

if [[ ! ${TRADING_PROCESS_ARG:-} ]]; then
    TRADING_PROCESS_ARG="${ACUTUS_PROCESS_DEFINITION}"
fi

if [[ ! ${CME_CONFIG_ARG:-} ]]; then
    echo setting cme
    CME_CONFIG_ARG="${ACUTUS_TRADING_DATA}/config.xml"
fi

RUN_PREFIX="$ONLOAD "
export EF_NAME="om_server"
export EF_POLL_USEC=1000000
export EF_BUZZ_USEC=1000000
export EF_TCP_RECV_SPIN=1
export EF_TCP_SEND_SPIN=1
export EF_EPOLL_SPIN=0
export EF_UL_EPOLL=1
export EF_NONAGLE_INFLIGHT_MAX=65535
export EF_FORCE_TCP_NODELAY=1 #always set nagle
export EF_TCP_RCVBUF=268435456 #256M
#export EF_LOG_FILE=/tmp/onload_cme_om.log #file always empty
#export EF_LOG_TIMESTAMPS=1 #Onload claims to not know what this is
#export EF_LOG_VIA_IOCTL=1 #all debug to syslog
export EF_PREFAULT_PACKETS=100
export EF_BINDTODEVICE_HANDOVER=0 #don't hand to kernel if SO_BINDTODEVICE set
#export EF_UNIX_LOG=0x2873f
if [[ ${ADAPTER_TS_ARG:-} ]]; then
    export EF_TX_TIMESTAMPING=3 # 3=fail if we can't get tx hardware timestamps
    export EF_TIMESTAMPING_REPORTING=0 # 0=report when nic clock is set, 1=nic clock set and in sync using ptpd
fi

echo
echo "solarflare variables set are:"
for SF_VAR in ${!EF_@}; do
    echo "$SF_VAR"="${!SF_VAR}"
done



if [[ ${GDB:-} ]]; then
    echo Debugging...
    export LD_BIND_NOW=1
    RUN_PREFIX='gdb -ex "b main" -ex "directory /opt/qcl/acutus/source" -ex r --args'
else
    if [[ ${CGDB:-} ]]; then
        echo CDebugging...
        export LD_BIND_NOW=1
        RUN_PREFIX='cgdb -ex "b main" -ex "directory /opt/qcl/acutus/source" --ex r --args'
    fi
fi


export HUGETLB_MORECORE=yes
SUPERPAGE_PREFIX="hugectl --text --data --bss --heap --no-preload --verbose 3 -- "

THIS_BIN=${ACUTUS_BIN}/cme_om_server

# patch the binary being debugged
# not to use avx2 instructions so that gdb can record and step backwards
if [[ ${GDB:-} ]] || [[ ${CGDB:-} ]]; then
    if [[ -x /usr/bin/patchelf ]] && [[ -e /opt/qcl/acutus/bin/lib_x86_64-linux-gnu_ld-linux-noAVX2.so.2 ]]; then
        echo_stderr "patching $THIS_BIN to use libc with no avx2"
        /usr/bin/patchelf --set-interpreter /opt/qcl/acutus/bin/lib_x86_64-linux-gnu_ld-linux-noAVX2.so.2 "$THIS_BIN"
    else
        echo_stderr "gdb on $THIS_BIN will not be able to step backwards, missing patchelf or /opt/qcl/acutus/bin/lib_x86_64-linux-gnu_ld-linux-noAVX2.so.2"
    fi
fi


CMD_ARR=("exec ${SUPERPAGE_PREFIX:-} ${RUN_PREFIX:-} ${THIS_BIN}" \
    "--trading-process-config ${TRADING_PROCESS_ARG}" \
    "--process-name ${PROCESS_NAME_ARG}" \
    "--universe-input ${REFDATA}" \
    "--trade-date ${TRADEDATE_ARG}" \
    "--msgw_config ${MSGW_CONFIG}" \
    "--config ${MSGW_FIX_CONFIG}" \
	"--interface ${INTERFACE}" \
	"${SECONDARY_INTERFACE_ARG:-}" \
    "${ADAPTER_TS_ARG:-}" \
	"${LOG_ARG:-} "
    )
CMD="${CMD_ARR[*]}"

if [[ ${DRY_RUN:-} ]]; then
    echo "dry run - command we would run is:"
fi

#readable
echo
echo "command is:"
echo
for item in "${CMD_ARR[@]}"; do
    echo -e "\t$item"
done
echo
echo "pasteable command:"
echo
echo -e "$CMD"
echo
echo "binary output starts:"
echo

echo
if [[ ! ${DRY_RUN:-} ]] ; then
    eval "$CMD"
fi

