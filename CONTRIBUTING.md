# Contributing to NXAchievements

## Scope
NXAchievements is a local, offline-first achievement framework for Nintendo Switch homebrew users.

## MVP boundaries (important)
- No piracy-related features
- No Nintendo service/auth bypass
- No NSO/eShop/matchmaking/telemetry integration
- No sysmodule/overlay/memory scanning in MVP

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run tests
```bash
./build/nxachievements-tests
./tests/pack-validator-smoke.sh
```

## Run example
```bash
mkdir -p tmp
./build/nxachievements-test examples/packs/0100000000000000.json ./tmp/unlocked.json
```

## Proposing achievement packs
- Validate before PR: `./build/pack-validator --verbose <your-pack.json>`
- Keep IDs stable and descriptions clear
- Avoid spoiler-heavy text for hidden achievements
