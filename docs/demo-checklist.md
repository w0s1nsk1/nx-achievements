# Demo checklist (v0.1-dev)

## 1) Build PC tools
```bash
cmake -S . -B build
cmake --build build
./build/nxachievements-tests
./tests/pack-validator-smoke.sh
```

## 2) Build Switch viewer
```bash
cd switch-viewer
make
```
Output: `switch-viewer/NXAchievements.nro`

## 3) SD card layout
```text
/switch/NXAchievements/NXAchievements.nro
/switch/NXAchievements/packs/
/switch/NXAchievements/profile/
```

## 4) Copy demo data
- Packs: `examples/packs/0100000000000000.json`
- Optional community packs: `examples/packs/community/*.json`
- Demo profile: `examples/profiles/unlocked.sample.json` -> `/switch/NXAchievements/profile/unlocked.json`

## 5) Capture screenshots
- Game list with progress
- Game details with [✓]/[ ]
- Errors screen (with invalid pack)

## 6) Known limitations
- Console-only UI
- No online sync
- No sysmodule/overlay
- No memory scanning
