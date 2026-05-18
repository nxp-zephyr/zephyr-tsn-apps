# Bash completion for build_apps.sh
#

_build_apps_completions() {
    local cur prev
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    # Resolve the script path relative to this completion file
    local script_dir
    script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local script="${script_dir}/build_apps.sh"

    case "$prev" in
        --apps)
            local apps
            apps=$("$script" --list-apps 2>/dev/null)
            COMPREPLY=($(compgen -W "$apps" -- "$cur"))
            return
            ;;
        --boards)
            local boards
            boards=$("$script" --list-boards 2>/dev/null)
            COMPREPLY=($(compgen -W "$boards" -- "$cur"))
            return
            ;;
        --output-dir)
            # Fall back to directory completion
            COMPREPLY=($(compgen -d -- "$cur"))
            return
            ;;
    esac

    # Top-level options
    local opts="--apps --boards --singlecore --multicore --use-crosscompile-toolchain --with-memory-reports --output-dir --dry-run --help"
    COMPREPLY=($(compgen -W "$opts" -- "$cur"))
}

complete -F _build_apps_completions build_apps.sh
complete -F _build_apps_completions ./build_apps.sh
