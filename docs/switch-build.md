# Switch build (libnx)

## Requirements
- devkitPro
- devkitA64
- libnx

## Build
```bash
cd switch-viewer
make
```

## Output
- `switch-viewer/NXAchievements.nro`

## SD layout
```text
/switch/NXAchievements/NXAchievements.nro
/switch/NXAchievements/packs/
/switch/NXAchievements/profile/
```

Demo profile:
- copy `examples/profiles/unlocked.sample.json` to `/switch/NXAchievements/profile/unlocked.json`
