set -e
exe=./build/Debug/TaskPlusPlus.exe
[[ -x "$exe" ]] || { echo "Executable not found. Run ./build.sh first." >&2; exit 1; }
exec "$exe" "$@"