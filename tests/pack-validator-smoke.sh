#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

VALID_MIN="examples/packs/community/valid-minimal.json"
VALID_LARGE="examples/packs/community/valid-larger.json"
UNKNOWN_FIELDS="examples/packs/community/unknown-fields.json"
INVALID_PACK="examples/packs/invalid/invalid-pack.json"

echo "[smoke] pack-validator: valid packs should pass"
./build/pack-validator "$VALID_MIN" "$VALID_LARGE" >/tmp/nx_pack_validator_valid.out

if ! grep -q "\[OK\] $VALID_MIN" /tmp/nx_pack_validator_valid.out; then
  echo "Expected OK for $VALID_MIN"
  cat /tmp/nx_pack_validator_valid.out
  exit 1
fi
if ! grep -q "\[OK\] $VALID_LARGE" /tmp/nx_pack_validator_valid.out; then
  echo "Expected OK for $VALID_LARGE"
  cat /tmp/nx_pack_validator_valid.out
  exit 1
fi

echo "[smoke] pack-validator: unknown fields should pass with warnings"
./build/pack-validator --verbose "$UNKNOWN_FIELDS" >/tmp/nx_pack_validator_warn.out
if ! grep -q "\[OK\] $UNKNOWN_FIELDS" /tmp/nx_pack_validator_warn.out; then
  echo "Expected OK for $UNKNOWN_FIELDS"
  cat /tmp/nx_pack_validator_warn.out
  exit 1
fi
if ! grep -q "\[WARN\] Unknown root field" /tmp/nx_pack_validator_warn.out; then
  echo "Expected WARN for unknown fields"
  cat /tmp/nx_pack_validator_warn.out
  exit 1
fi

echo "[smoke] pack-validator: invalid pack should fail"
set +e
./build/pack-validator "$INVALID_PACK" >/tmp/nx_pack_validator_invalid.out 2>&1
STATUS=$?
set -e
if [ "$STATUS" -eq 0 ]; then
  echo "Expected non-zero exit for invalid pack"
  cat /tmp/nx_pack_validator_invalid.out
  exit 1
fi
if ! grep -q "\[ERROR\]" /tmp/nx_pack_validator_invalid.out; then
  echo "Expected ERROR output for invalid pack"
  cat /tmp/nx_pack_validator_invalid.out
  exit 1
fi

echo "[smoke] pack-validator: json output"
./build/pack-validator --json "$VALID_MIN" >/tmp/nx_pack_validator_json.out
if ! grep -q '"file": "'$VALID_MIN'"' /tmp/nx_pack_validator_json.out; then
  echo "Expected JSON output to include file"
  cat /tmp/nx_pack_validator_json.out
  exit 1
fi
if ! grep -q '"status": "OK"' /tmp/nx_pack_validator_json.out; then
  echo "Expected JSON output status OK"
  cat /tmp/nx_pack_validator_json.out
  exit 1
fi

echo "[smoke] pack-validator smoke tests passed"
