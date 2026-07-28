#!/bin/bash
# Runs every case in tests/cases against the constructor binary.
#
# Usage: ./run.sh [case-name-filter]
#   CTOR_BIN=/path/to/constructor ./run.sh
#
# Each case is a directory under cases/ containing:
#   cmd       command-line arguments (one line, may be empty)
#   in        stdin fed to the program
#   expected  exact expected stdout (optional if check is present)
#   check     bash assertions run in the work directory (optional)
#   keepboard if present, the 42-line board drawings are compared too; by
#             default they are stripped so expected files stay readable.
#             Case board-figure3 asserts the drawing itself.
#   needs     names of files under data/ to copy into the work directory,
#             one per line. Absent means the work directory starts empty,
#             which is how the "no layout.txt present" cases are written.
#   any other file in the case directory is copied into the work directory

here="$(cd "$(dirname "$0")" && pwd)"
bin="${CTOR_BIN:-$here/../project/constructor}"
filter="$1"

if [ ! -x "$bin" ]; then
  echo "no binary at $bin (set CTOR_BIN)" 1>&2
  exit 2
fi
bin="$(cd "$(dirname "$bin")" && pwd)/$(basename "$bin")"

pass=0
fail=0
failed=""

for dir in "$here"/cases/*/; do
  name="$(basename "$dir")"
  if [ -n "$filter" ] && [[ "$name" != *"$filter"* ]]; then continue; fi

  work="$(mktemp -d)"
  if [ -f "$dir/needs" ]; then
    while read -r n; do
      [ -n "$n" ] && cp "$here/data/$n" "$work"/
    done < "$dir/needs"
  fi
  for f in "$dir"*; do
    case "$(basename "$f")" in
      cmd|in|expected|check|keepboard|needs) ;;
      *) cp "$f" "$work"/ ;;
    esac
  done

  args=""
  [ -f "$dir/cmd" ] && args="$(cat "$dir/cmd")"

  if [ -n "$VALGRIND" ]; then
    ( cd "$work" && eval "valgrind --leak-check=full --error-exitcode=99 \"$bin\" $args" \
        < "$dir/in" > raw 2>stderr )
  else
    ( cd "$work" && eval "\"$bin\" $args" < "$dir/in" > raw 2>stderr )
  fi
  status=$?

  if [ -n "$VALGRIND" ]; then
    if [ $status -eq 99 ] || ! grep -q "ERROR SUMMARY: 0 errors" "$work/stderr"; then
      echo "VALGRIND ERRORS in $name"
      grep -E "ERROR SUMMARY|definitely lost|indirectly lost|in use at exit" "$work/stderr"
    else
      printf 'valgrind clean: %s -- %s\n' "$name" \
        "$(grep -E 'ERROR SUMMARY' "$work/stderr" | head -1 | sed 's/^==[0-9]*== //')"
    fi
    grep -E "in use at exit|All heap blocks" "$work/stderr" | sed 's/^==[0-9]*== /  /'
  fi

  # The "> " prompt carries a trailing space and the final one has no newline
  # after it. Normalise both so expected files stay hand-writable; a prompt is
  # still visible as ">".
  sed -e 's/[[:space:]]*$//' "$work/raw" > "$work/raw.n" && mv "$work/raw.n" "$work/raw"

  if [ -f "$dir/keepboard" ]; then
    cp "$work/raw" "$work/actual"
  else
    awk '/^[> ]*\|[^|][^|]\|--[^|]*--\|[^|]*\|$/ {
           if (skip == 0) {
             p = $0
             sub(/\|[^|][^|]\|--.*$/, "", p)
             sub(/ +$/, "", p)
             if (p != "") print p
             skip = 41
             next
           }
         }
         { if (skip > 0) { skip--; next } print }' \
      "$work/raw" > "$work/actual"
  fi

  ok=1
  reason=""

  if [ -f "$dir/expected" ]; then
    if ! diff -u "$dir/expected" "$work/actual" > "$work/diff" 2>&1; then
      ok=0
      reason="stdout differs from expected"
    fi
  fi

  if [ $ok -eq 1 ] && [ -f "$dir/check" ]; then
    if ! ( cd "$work" && CTOR_BIN="$bin" CASE_IN="$dir/in" bash "$dir/check" > checkout 2>&1 ); then
      ok=0
      reason="check script failed"
    fi
  fi

  if [ $ok -eq 1 ]; then
    pass=$((pass + 1))
    printf 'PASS  %s\n' "$name"
  else
    fail=$((fail + 1))
    failed="$failed $name"
    printf 'FAIL  %s (%s, exit %d)\n' "$name" "$reason" "$status"
    if [ -s "$work/diff" ]; then
      sed 's/^/      /' "$work/diff" | head -40
    fi
    if [ -s "$work/checkout" ]; then
      sed 's/^/      /' "$work/checkout" | head -40
    fi
    if [ -s "$work/stderr" ]; then
      echo "      --- stderr ---"
      sed 's/^/      /' "$work/stderr" | head -10
    fi
  fi

  rm -rf "$work"
done

echo
echo "passed: $pass   failed: $fail"
if [ $fail -ne 0 ]; then
  echo "failing:$failed"
  exit 1
fi
