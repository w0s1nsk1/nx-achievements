# Runtime unlock detection research (concept only)

> Status: **research-only**
>
> - Not implemented
> - Not part of `v0.1-dev`
> - Unstable area
> - May never be fully universal across all games

## Purpose
Ten dokument opisuje możliwe kierunki przyszłej automatycznej detekcji unlocków w runtime, bez implementacji na obecnym etapie.

## Possible future architectures

### 1) Polling-based memory checks
- Okresowe odczyty wybranych adresów/struktur stanu gry
- Plusy: prosty model
- Minusy: koszt CPU, ryzyko false positives, wrażliwe na update gry

### 2) Event-driven hooks
- Reakcja na zdarzenia systemowe / punkty wykonania (hooki)
- Plusy: potencjalnie mniejszy overhead
- Minusy: duża złożoność, wysokie ryzyko niestabilności

### 3) Emulator-assisted workflows
- Budowa/kalibracja warunków w emulatorze, potem mapowanie do runtime
- Plusy: szybszy R&D cycle dla pack authorów
- Minusy: różnice emulator vs real hardware

### 4) Save-based detection
- Wnioskowanie unlocków na podstawie danych save (snapshot/diff)
- Plusy: bez live memory hooks
- Minusy: opóźniona detekcja, nie każdy warunek da się odzyskać z save

### 5) Hybrid approaches
- Mix save-based + lekkie runtime checks + pack hints
- Plusy: kompromis dokładność/stabilność
- Minusy: większa złożoność całego pipeline

## Risks and complexity

### Memory layout drift
- Patche gry zmieniają layout pamięci
- Każdy update może unieważnić wskaźniki/offsety

### Build ID / version fragmentation
- Wiele buildów jednej gry = wiele map stanu
- Wysoki koszt utrzymania packów per wersja

### Anti-crash concerns
- Błędne odczyty/hooki mogą destabilizować proces gry
- Konieczne bezpieczne guardy i fail-safe

### Performance
- Zbyt częste checki = overhead CPU
- Overlay/notifier + runtime checks mogą kumulować koszt

### False positives / false negatives
- Zła interpretacja stanu może odblokować osiągnięcie błędnie
- Albo nie odblokować mimo spełnionego warunku

### Memory permissions
- Odczyt pamięci może wymagać uprawnień/kontekstu niedostępnego wszędzie
- Ograniczenia platformowe i stabilność API

### Portability
- To, co działa dla jednej gry/builda, nie musi działać dla innych
- Trudno osiągnąć model „write once, run everywhere”

## Conceptual future components (non-implementation)

- **Runtime daemon/sysmodule** (concept): orkiestracja odczytów i evaluacji
- **Game state providers**: adaptery per-gra/per-build do ekspozycji stanu
- **Condition evaluators**: rozszerzony evaluator warunków runtime
- **Overlay notifier**: opcjonalne powiadomienie o unlocku
- **Save import/export bridge**: most między profilem local i analizą save

## Tooling research notes (future)

- Pointer scanners (R&D only)
- Memory inspectors (R&D only)
- Pack authoring helpers (mapowanie key -> źródło danych)
- Emulator debugging workflows (trace/diff/state snapshots)

## Explicit warnings
- Ten obszar nie jest zaimplementowany.
- Nie jest częścią `v0.1-dev`.
- Może pozostać eksperymentalny przez dłuższy czas.
- Nie ma gwarancji uniwersalności między grami.

## Scope boundaries (still in force)
- Brak implementacji memory scanning w obecnym milestone
- Brak implementacji sysmodule
- Brak implementacji overlay
- Brak cheat framework
- Brak integracji z usługami Nintendo
