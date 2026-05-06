# Schema versioning

## `schema_version`
`schema_version` określa wersję formatu packa JSON. Obecnie wspierana: `1`.

## Backward compatibility
- Loader powinien akceptować znane wersje schema.
- Nieznane pola traktujemy jako warning (non-fatal), jeśli nie psują required fields.
- Zmiany breaking powinny podnosić `schema_version`.

## Evolving condition types
- Nowe typy warunków dodajemy wstecznie-kompatybilnie, gdy możliwe.
- Stare packi bez nowych typów muszą działać bez zmian.
- Narzędzie walidujące powinno zgłaszać nieznane typy jako błąd dla aktualnej wersji schema.

## Migration expectations
- Dla nowej wersji schema przygotować:
  - changelog różnic,
  - guide migracji,
  - przykłady before/after,
  - aktualizację pack-validatora.
