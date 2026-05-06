# Creating achievement packs

## Recommended workflow
1. Skopiuj `examples/packs/community/valid-minimal.json` jako punkt startowy.
2. Uzupełnij `title_id`, metadata gry i achievementy.
3. Uruchom walidację przed PR:
   ```bash
   ./build/pack-validator --verbose <twoj-pack.json>
   ```
4. Sprawdź pack w `nxachievements-test` na mockowanym stanie.
5. Otwórz PR z krótkim opisem założeń achievementów.

## Naming conventions
- `id`: krótkie, stabilne, lowercase + `_` (np. `first_boss`)
- `name`: krótki, czytelny tytuł
- `description`: konkretna, bez lania wody

## Writing style guidelines
- Opisy jasne i testowalne.
- Unikaj niejednoznacznych warunków.
- Dla hidden achievementów trzymaj neutralne opisy bez spoilerów.

## Hidden achievements recommendations
- Używaj `hidden: true` dla fabularnych momentów i twistów.
- Nie zdradzaj krytycznych spoilerów w `name/description`.

## Spoiler policy
- Unikaj spoiler-heavy descriptions.
- Jeżeli achievement dotyczy finału, preferuj ogólny opis.

## Future condition types (TODO)
Potencjalne kierunki rozwoju (bez implementacji w MVP):
- multiple AND/OR groups
- timed conditions
- counters
- progression chains
- save-based conditions
- runtime memory hooks (future research only)
