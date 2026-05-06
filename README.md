# NXAchievements

<!-- CI badge placeholder: replace OWNER/REPO when repository URL is known -->
<!-- ![CI](https://github.com/OWNER/REPO/actions/workflows/ci.yml/badge.svg) -->

A minimal, local achievement framework for Nintendo Switch CFW/Homebrew.

**Current preview:** `v0.1-dev`

> Projekt **offline-first**. Bez integracji z usługami Nintendo, bez NSO, bez auth bypassów, bez piractwa.

## State of the project
- Current state: **experimental developer preview** (`v0.1-dev`)
- Stable runtime unlock detection is **NOT** implemented
- Current focus: viewer + pack tooling + validation + docs

## MVP (ten milestone)
- C++17 core engine (portable)
- format packów JSON
- parser packów + walidacja
- model achievementów
- lokalny profil `unlocked.json`
- prosty evaluator warunków na mockowanym stanie
- CLI runner na PC
- testy assert-based
- przykładowy pack

## Struktura
- `core/` – silnik portable C++
- `tools/nxachievements-test/` – CLI tester na PC
- `tests/` – lekkie testy bez frameworka
- `examples/packs/` – przykładowe packi
- `docs/` – architektura i format
- `switch-viewer/` – prosty shell pod przyszłe libnx

## Build (CMake)
Wymagania: CMake 3.16+, kompilator z C++17.

```bash
cd NXAchievements
cmake -S . -B build
cmake --build build
```

Binarki:
- `build/nxachievements-test`
- `build/nxachievements-tests`
- `build/pack-validator`

## Build fallback (g++)
```bash
cd NXAchievements
g++ -std=c++17 -Icore/include -Icore/vendor \
  core/src/*.cpp tools/nxachievements-test/main.cpp \
  -o nxachievements-test
```

## Run CLI
```bash
./build/nxachievements-test \
  examples/packs/0100000000000000.json \
  ./unlocked.json
```

CLI odpali testowy stan mockowany i zapisze odblokowania do `unlocked.json`.

## Switch viewer demo profile
Dla szybkich screenshotów/demo:
- użyj `examples/profiles/unlocked.sample.json`
- skopiuj do: `/switch/NXAchievements/profile/unlocked.json`
- uruchom `switch-viewer/NXAchievements.nro`

## Run tests
```bash
./build/nxachievements-tests
```

## Validate packs
```bash
./build/pack-validator --verbose examples/packs/community/valid-minimal.json
./build/pack-validator --json examples/packs/community/unknown-fields.json
```

Scenariusze testowe:
- unlock when conditions match
- no duplicate unlock on second run
- no unlock when conditions fail
- malformed JSON error handling
- unknown condition type handling
- profile file creation when missing

## Development workflow
```bash
# configure
cmake -S . -B build

# build
cmake --build build

# test
./build/nxachievements-tests

# run example
mkdir -p tmp
./build/nxachievements-test examples/packs/0100000000000000.json ./tmp/unlocked.json

# pack-validator smoke
./tests/pack-validator-smoke.sh
```

## Screenshots (placeholders)

### Switch viewer screenshots
- _TODO: add Game List screenshot_
- _TODO: add Game Details screenshot_
- _TODO: add Errors screen screenshot_

### Pack validator CLI screenshots
- _TODO: add `pack-validator --verbose` output screenshot_
- _TODO: add `pack-validator --json` output screenshot_

### Demo profile screenshots
- _TODO: add screenshot with sample `unlocked.sample.json` loaded_

## Security note
NXAchievements does not implement:
- any Nintendo authentication bypass,
- any integration with eShop/NSO/matchmaking/telemetry,
- any piracy-related functionality.
