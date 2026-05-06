# Architecture (MVP)

## Core components
1. **PackLoader**
   - loads and validates JSON packs
2. **ConditionEvaluator**
   - evaluates conditions against `MockState`
3. **ProfileStore**
   - loads/saves local unlock state (`unlocked.json`)
4. **AchievementEngine**
   - evaluates achievements and prevents duplicate unlocks

## Data flow
1. CLI loads pack
2. CLI loads profile (or starts empty)
3. CLI builds mock game state
4. Engine evaluates conditions
5. New unlocks are saved with timestamps

## Portability
Core is platform-independent C++17 and testable on PC.
