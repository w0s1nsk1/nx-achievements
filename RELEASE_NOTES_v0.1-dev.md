# NXAchievements v0.1-dev — release notes

## Overview
NXAchievements to eksperymentalny, lokalny framework achievementów dla Nintendo Switch homebrew users. Ten release skupia się na fundamencie: portable core, pack tooling i konsolowy viewer.

## Implemented in v0.1-dev
- C++17 portable core engine
- Achievement pack JSON loader + validation
- Local profile storage (`unlocked.json`)
- Condition evaluator (MVP types)
- PC CLI test runner (`nxachievements-test`)
- Pack validator CLI (`pack-validator`) z trybami `--verbose`, `--quiet`, `--json`
- Assert-based tests + smoke tests
- GitHub Actions CI
- Switch console viewer shell (`.nro`):
  - game list
  - game details
  - progress from local profile
  - errors screen for invalid packs

## Non-goals (current)
- No Nintendo service integration
- No Nintendo Switch Online replacement
- No memory scanning implementation
- No sysmodule implementation
- No overlay implementation
- No online sync implementation
- No piracy tooling / no DRM bypass / no cheat engine behavior

## Known limitations
- Viewer jest console-only (brak graficznego UI)
- Runtime auto-unlock detection nie jest zaimplementowany
- Pack ecosystem dopiero startuje (mało gotowych packów)
- Forwarder generation nie jest częścią repo

## Roadmap summary
- M3.x: polish viewer + community tooling
- M4: optional sync API design (future)
- M5: runtime research (docs/concepts)
- M6: experimental unlock runtime research
- M7: overlay research

## Experimental disclaimer
To jest **developer preview** (`v0.1-dev`).
API, formaty i UX mogą się zmieniać. Projekt jest aktywnie rozwijany i nie gwarantuje stabilności produkcyjnej.
