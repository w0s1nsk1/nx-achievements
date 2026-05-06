# Switch Viewer UI Flow (M2)

To jest dokument przepływu UI dla `switch-viewer`.
Brak implementacji sysmodule/overlay/memory scanning.

## Current implementation status (M3 in progress)
Aktualnie działa konsolowy viewer `.nro`, który:
- ładuje profil z `/switch/NXAchievements/profile/unlocked.json` (brak pliku = pusty profil),
- skanuje `/switch/NXAchievements/packs/` pod kątem `*.json`,
- ładuje packi przez wspólny `PackLoader` z portable core,
- sortuje gry alfabetycznie po nazwie,
- pokazuje progress per gra: unlocked/total + procent,
- ma trzy ekrany: `GameListScreen`, `GameDetailsScreen`, `ErrorsScreen`.

### Controls
#### Game list
- `UP/DOWN` wybór gry
- `A` wejście w szczegóły
- `X` ekran błędów packów
- `B` lub `PLUS` wyjście

#### Game details
- `UP/DOWN` przewijanie listy achievementów (paginacja)
- `B` powrót do listy
- `PLUS` wyjście

#### Errors
- `UP/DOWN` przewijanie błędów
- `B` powrót do listy
- `PLUS` wyjście

### Rendering decisions
- Achievementy są wyświetlane w kolejności z packa (pack order preserved), żeby zachować przewidywalność i zgodność z autorem packa.
- Status achievementu: `[✓]` / `[ ]`, badge tier, nazwa, opis.

## 1) Home
- App title: NXAchievements
- Krótki status profilu (np. total unlocked / total achievements)
- Przyciski:
  - Browse games
  - Settings
  - Sync (placeholder, disabled)

## 2) Game list
- Lista gier na podstawie packów JSON z `/switch/NXAchievements/packs/`
- Każda pozycja:
  - nazwa gry
  - procent ukończenia
  - badge progress (opcjonalnie)
- Akcja: wejście w szczegóły gry

## 3) Game details
- Nazwa gry, title ID
- Liczba odblokowanych / wszystkich
- Lista achievementów:
  - nazwa
  - opis (lub ukryte)
  - status lock/unlock
  - badge tier
- Akcja: wejście w szczegóły achievementu

## 4) Achievement details
- Pełna nazwa + opis
- Badge tier
- Status: locked/unlocked
- Unlock timestamp (jeśli odblokowany)
- W przyszłości: debug info warunków (opcjonalne)

## 5) Settings
- Ścieżki danych (read-only info)
- Reset local cache/profile (future, z potwierdzeniem)
- About + wersja app

## 6) Sync placeholder
- Tylko ekran informacyjny:
  - "Online sync is not available in MVP"
- Brak integracji online na tym etapie

---

## Navigation sketch
Home -> Game list -> Game details -> Achievement details
Home -> Settings
Home -> Sync placeholder
