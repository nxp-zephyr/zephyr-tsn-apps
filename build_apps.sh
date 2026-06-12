#!/bin/bash

set -euo pipefail

usage() {
      cat <<EOF
Usage:
    ./build_apps.sh [OPTIONS]

To enable tab-completion:
   source ./build_apps_completion.bash

Options:
    --apps <app1,app2,...>          Comma-separated list of apps to build (default: all)
    --boards <board1,board2,...>    Comma-separated list of boards to build for (default: all)
    --singlecore                    Build single-core configuration only (default: builds all possible core configurations)
    --multicore                     Build multi-core configuration only (default: builds all possible core configurations)
    --use-crosscompile-toolchain    Use ARM GCC cross-compiler (requires ARMGCC_DIR env variable)
    --with-memory-reports           Generate ROM and RAM usage reports after each build
    --output-dir <dir>              Output directory for build artifacts (default: ./build_output)
    --dry-run                       Print the build matrix and commands without executing
    --help                          Show this help message
EOF
}

# --- Supported Build Configurations ---------------------------------------------------------
#
# List of supported configurations: (app, board, multicore) triple.
# Format:  "<app> | <board> | <multicore: yes|no>"
#

CONFIGURATIONS=(
    "networking | mimxrt1180_evk/mimxrt1189/cm33 | no "
    "tsn_init   | mimxrt1180_evk/mimxrt1189/cm33 | yes "
    "tsn_app    | mimxrt1180_evk/mimxrt1189/cm33 | yes "
)

# --- Runtime Default State ------------------------------------------------------------------

REQUESTED_APPS=()
REQUESTED_BOARDS=()
REQUESTED_CORES=()
OUTPUT_DIR="./build_output"
DRY_RUN=false
MEMORY_REPORTS=false
USE_CROSSCOMPILE=false

# --- Color Codes ----------------------------------------------------------------------------

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# --- Logging Helpers ------------------------------------------------------------------------

log_info()    { echo -e "${BLUE}[INFO]${NC}    $*"; }
log_success() { echo -e "${GREEN}[SUCCESS]${NC} $*"; }
log_warning() { echo -e "${YELLOW}[WARNING]${NC} $*"; }
log_error()   { echo -e "${RED}[ERROR]${NC}   $*" >&2; }
log_cmd()     { echo -e "${YELLOW}[CMD]${NC}     \$ $*"; }
log_dry()     { echo -e "${CYAN}[DRY-RUN]${NC} \$ $*"; }
log_header()  { echo -e "\n${BOLD}${CYAN}======================================================================${NC}"; \
                echo -e "${BOLD}${CYAN}  $*${NC}"; \
                echo -e "${BOLD}${CYAN}======================================================================${NC}"; }

# --- Helpers --------------------------------------------------------------------------------

# Strip leading and trailing whitespace from a string.
trim() {
    local s="$1"
    s="${s#"${s%%[![:space:]]*}"}"
    s="${s%"${s##*[![:space:]]}"}"
    echo "$s"
}

# Parse a CONFIGURATIONS entry into its three fields.
# Sets the caller's variables: _app  _board  _mc
# Usage: parse_config_entry "$entry"
parse_config_entry() {
    local entry="$1"
    local raw_app raw_board raw_mc
    IFS='|' read -r raw_app raw_board raw_mc <<< "$entry"
    _app=$(trim "$raw_app")
    _board=$(trim "$raw_board")
    _mc=$(trim "$raw_mc")
}

array_contains() {
    local needle="$1"; shift
    local element
    for element in "$@"; do
        [[ "$element" == "$needle" ]] && return 0
    done
    return 1
}

# Extract unique values (boards or apps) for a given field from CONFIGURATIONS
# Usage: get_configurations_field <app|board>
get_configurations_field() {
    local field="$1"
    local ret_values=()
    local entry _app _board _mc value
    for entry in "${CONFIGURATIONS[@]}"; do
        parse_config_entry "$entry"
        [[ "$field" == "app" ]] && value="$_app" || value="$_board"
        if ! array_contains "$value" "${ret_values[@]+"${ret_values[@]}"}"; then
            ret_values+=("$value")
        fi
    done
    printf '%s\n' "${ret_values[@]}"
}

# --- Argument Parsing -----------------------------------------------------------------------

# Validate that an option requiring an argument actually received one.
# Usage: require_arg <option_name> <remaining_arg_count> [next_arg]
require_arg() {
    local option="$1"
    local argc="$2"
    local next="${3:-}"

    if [[ "$argc" -lt 2 || "$next" == --* ]]; then
        log_error "'${option}' requires an argument."
        usage
        exit 1
    fi
}

parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --apps)
                require_arg "$1" "$#" "${2:-}"
                IFS=',' read -ra REQUESTED_APPS <<< "$2"
                shift 2
                ;;
            --boards)
                require_arg "$1" "$#" "${2:-}"
                IFS=',' read -ra REQUESTED_BOARDS <<< "$2"
                shift 2
                ;;
            --singlecore)
                REQUESTED_CORES+=("single-core")
                shift
                ;;
            --multicore)
                REQUESTED_CORES+=("multi-core")
                shift
                ;;
            --use-crosscompile-toolchain)
                USE_CROSSCOMPILE=true
                shift
                ;;
            --with-memory-reports)
                MEMORY_REPORTS=true
                shift
                ;;
            --output-dir)
                require_arg "$1" "$#" "${2:-}"
                OUTPUT_DIR="$2"
                shift 2
                ;;
            --dry-run)
                DRY_RUN=true
                shift
                ;;
            --help)
                usage
                exit 0
                ;;
            --list-apps)
                get_configurations_field app
                exit 0
                ;;
            --list-boards)
                get_configurations_field board
                exit 0
                ;;
            *)
                log_error "Unknown option: $1"
                usage
                exit 1
                ;;
        esac
    done

    # Validate requested apps
    if [[ ${#REQUESTED_APPS[@]} -gt 0 ]]; then
        local supported_apps
        mapfile -t supported_apps <<< "$(get_configurations_field app)"
        local app
        for app in "${REQUESTED_APPS[@]}"; do
            if ! array_contains "$app" "${supported_apps[@]}"; then
                log_error "Unknown application: '${app}'"
                log_error "Supported applications: ${supported_apps[*]}"
                exit 1
            fi
        done
    fi

    # Validate requested boards
    if [[ ${#REQUESTED_BOARDS[@]} -gt 0 ]]; then
        local supported_boards
        mapfile -t supported_boards <<< "$(get_configurations_field board)"
        local board
        for board in "${REQUESTED_BOARDS[@]}"; do
            if ! array_contains "$board" "${supported_boards[@]}"; then
                log_error "Unknown board: '${board}'"
                log_error "Supported applications: ${supported_boards[*]}"
                exit 1
            fi
        done
    fi
}

# --- Matrix Resolution ----------------------------------------------------------------------

MATRIX_APPS=()
MATRIX_BOARDS=()
MATRIX_CORES=()

resolve_matrix() {
    local entry _app _board _mc

    for entry in "${CONFIGURATIONS[@]}"; do
        parse_config_entry "$entry"

        # Apply app filter
        if [[ ${#REQUESTED_APPS[@]} -gt 0 ]]; then
            array_contains "$_app" "${REQUESTED_APPS[@]}" || continue
        fi

        # Apply board filter
        if [[ ${#REQUESTED_BOARDS[@]} -gt 0 ]]; then
            array_contains "$_board" "${REQUESTED_BOARDS[@]}" || continue
        fi

        # Determine which core configs this entry supports
        local cores=("single-core")
        [[ "$_mc" == "yes" ]] && cores+=("multi-core")

        local core
        for core in "${cores[@]}"; do
            # If the user specifies core configs, filter out any supported configs that are not listed.
            if [[ ${#REQUESTED_CORES[@]} -gt 0 ]]; then
                array_contains "$core" "${REQUESTED_CORES[@]}" || continue
            fi

            MATRIX_APPS+=("$_app")
            MATRIX_BOARDS+=("$_board")
            MATRIX_CORES+=("$core")
        done
    done

    if [[ ${#MATRIX_APPS[@]} -eq 0 ]]; then
        log_error "No build combinations match the requested configuration"
        log_error "Check that the requested apps, boards, and core configs are compatible."
        exit 1
    fi
}

# --- Build Directory Naming -----------------------------------------------------------------

# Extract the base board name (first segment before any slash).
#   "mimxrt1180_evk/mimxrt1189/cm33"  ->  "mimxrt1180_evk"
board_to_short_name() {
    echo "${1%%/*}"
}

get_build_dir() {
    local app="$1"
    local board="$2"
    local core="$3"

    local board_short
    board_short=$(board_to_short_name "$board")

    local dir_name="${app}_${board_short}"
    if [[ "$core" == "multi-core" ]]; then
        dir_name="${dir_name}_multicore"
    fi

    echo "${OUTPUT_DIR}/${dir_name}"
}

# --- Build Matrix Display -------------------------------------------------------------------

print_matrix() {
    local total=${#MATRIX_APPS[@]}
    local reports_label="no"
    [[ "$MEMORY_REPORTS" == true ]] && reports_label="yes"

    log_header "Build Matrix (${total} builds)  |  memory-reports=${reports_label}"
    printf "${BOLD}%-4s %-14s %-35s %-13s %-s${NC}\n" "#" "APPLICATION" "BOARD" "CORE CONFIG" "BUILD DIRECTORY"
    printf "%-4s %-14s %-35s %-13s %-s\n" "---" "-------------" "----------------------------------" "------------" "---------------------------------------------"

    local i
    for (( i=0; i<total; i++ )); do
        local app="${MATRIX_APPS[$i]}"
        local board="${MATRIX_BOARDS[$i]}"
        local core="${MATRIX_CORES[$i]}"
        local build_dir
        build_dir=$(get_build_dir "$app" "$board" "$core")
        printf "%-4s %-14s %-35s %-13s %-s\n" "$(( i+1 ))" "${app}" "${board}" "${core}" "${build_dir}"
    done
    echo ""
}

# --- Memory Reports -------------------------------------------------------------------------

# Run a single report target and save its output.
# Usage: run_report <report_target: e.g ram_report or rom_report> <build_dir> <output_file> <label>
run_report() {
    local target="$1"
    local dir="$2"
    local output="$3"
    local label="$4"

    if [[ "$DRY_RUN" == true ]]; then
        log_dry "west build -t ${target} -d ${dir}  >  ${output}"
        return 0
    fi

    log_cmd "west build -t ${target} -d ${dir}  >  ${output}"

    if west build -t "${target}" -d "${dir}" > "${output}" 2>&1; then
        log_success "${label} saved to ${output}"
    else
        log_warning "${label} failed -- see ${output}"
    fi
}

generate_memory_reports() {
    local build_dir="$1"
    local app="$2"
    local board="$3"
    local core="$4"

    log_info "Generating memory reports for ${BOLD}${app}${NC} (${board}, ${core})..."

    if [[ "$core" == "single-core" ]]; then
        # Single-core: one report pair against the build directory itself
        local rom_report="${build_dir}/rom_report.txt"
        local ram_report="${build_dir}/ram_report.txt"

        run_report rom_report "${build_dir}" "${rom_report}" "ROM report"
        run_report ram_report "${build_dir}" "${ram_report}" "RAM report"
    else
        # Multi-core (sysbuild): two report pairs -- primary and secondary
        local primary_dir="${build_dir}/${app}"
        local secondary_dir="${build_dir}/${app}_secondary"

        local primary_rom="${build_dir}/${app}_rom_report.txt"
        local primary_ram="${build_dir}/${app}_ram_report.txt"
        local secondary_rom="${build_dir}/${app}_secondary_rom_report.txt"
        local secondary_ram="${build_dir}/${app}_secondary_ram_report.txt"

        log_info "  Primary domain   -> ${primary_dir}"
        run_report rom_report "${primary_dir}" "${primary_rom}" "Primary ROM report"
        run_report ram_report "${primary_dir}" "${primary_ram}" "Primary RAM report"

        log_info "  Secondary domain -> ${secondary_dir}"
        run_report rom_report "${secondary_dir}" "${secondary_rom}" "Secondary ROM report"
        run_report ram_report "${secondary_dir}" "${secondary_ram}" "Secondary RAM report"
    fi
}

# --- Single Build Step ----------------------------------------------------------------------

run_build() {
    local app="$1"
    local board="$2"
    local core="$3"

    local src_dir="apps/${app}"

    local build_dir
    build_dir=$(get_build_dir "$app" "$board" "$core")

    log_info "Building: app=${BOLD}${app}${NC}  board=${BOLD}${board}${NC}  config=${BOLD}${core}${NC}"

    local cmd=(
        west build
        -p always
        -b "${board}"
    )

    if [[ "$core" == "multi-core" ]]; then
        cmd+=(--sysbuild)
    fi

    cmd+=(
        "${src_dir}"
        -d "${build_dir}"
    )

    if [[ "$DRY_RUN" == true ]]; then
        log_dry "${cmd[*]}"
        return 0
    fi

    log_cmd "${cmd[*]}"

    "${cmd[@]}"
}

# --- Toolchain Setup ------------------------------------------------------------------------

setup_crosscompile_toolchain() {
    if [[ -z "${ARMGCC_DIR:-}" ]]; then
        log_error "ARMGCC_DIR is not defined. Set it to the ARM GCC toolchain installation path."
        exit 1
    fi

    export ZEPHYR_TOOLCHAIN_VARIANT=cross-compile
    export CROSS_COMPILE="${ARMGCC_DIR}/bin/arm-none-eabi-"

    log_info "Cross-compile toolchain configured:"
    log_info "  ARMGCC_DIR               = ${ARMGCC_DIR}"
    log_info "  ZEPHYR_TOOLCHAIN_VARIANT = ${ZEPHYR_TOOLCHAIN_VARIANT}"
    log_info "  CROSS_COMPILE            = ${CROSS_COMPILE}"
}

# --- Main -----------------------------------------------------------------------------------

main() {
    parse_args "$@"
    resolve_matrix
    print_matrix

    if [[ "$DRY_RUN" == true ]]; then
        log_warning "Dry-run mode -- commands will be printed but not executed."
    else
        if ! command -v west &>/dev/null; then
            log_error "'west' command not found. Please install or activate your Zephyr environment."
            exit 1
        fi

        if [[ "$USE_CROSSCOMPILE" == true ]]; then
            setup_crosscompile_toolchain
        fi

        mkdir -p "${OUTPUT_DIR}"
    fi

    local total=${#MATRIX_APPS[@]}
    local i

    for (( i=0; i<total; i++ )); do
        local app="${MATRIX_APPS[$i]}"
        local board="${MATRIX_BOARDS[$i]}"
        local core="${MATRIX_CORES[$i]}"
        local build_dir
        build_dir=$(get_build_dir "$app" "$board" "$core")

        log_header "[$(( i+1 ))/${total}]  ${app}  |  ${board}  |  ${core}"

        if ! run_build "$app" "$board" "$core"; then
            log_error "Build failed: app=${app}  board=${board}  config=${core}  dir=${build_dir}"
            log_error "Aborting -- $(( i+1 )) of ${total} combinations completed."
            exit 1
        fi

        if [[ "$DRY_RUN" != true ]]; then
            log_success "Built ${app} for ${board} (${core})  ->  ${build_dir}"
        fi

        if [[ "$MEMORY_REPORTS" == true ]]; then
            generate_memory_reports "$build_dir" "$app" "$board" "$core"
        fi
    done

    exit 0
}

main "$@"
