#!/usr/bin/env bash
set -euo pipefail

BIN="${1:-./cub3D}"
MAP="${2:-maps/example.cub}"
LOG="${3:-valgrind_clean.log}"

if [[ ! -x "$BIN" ]]; then
	echo "Error\n'$BIN' no existe o no es ejecutable." >&2
	exit 1
fi

if [[ ! -f "$MAP" ]]; then
	echo "Error\nmapa '$MAP' no existe." >&2
	exit 1
fi

valgrind \
	--leak-check=full \
	--show-leak-kinds=definite,possible \
	--errors-for-leak-kinds=definite,possible \
	--track-origins=yes \
	--num-callers=30 \
	--log-file="$LOG" \
	"$BIN" "$MAP"

echo "Log guardado en: $LOG"
echo "Resumen:"
rg -n "ERROR SUMMARY|LEAK SUMMARY|definitely lost|possibly lost|Invalid read|Invalid write|uninitialised|Invalid free" "$LOG" || true
