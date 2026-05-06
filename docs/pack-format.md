# Pack format (JSON)

Ścieżka docelowa na Switchu:
`/switch/NXAchievements/packs/<title_id>.json`

## Required fields
### Root (required)
- `schema_version` (int)
- `title_id` (string)
- `game` (object)
  - `name` (string)
  - `developer` (string)
- `achievements` (array)

### Achievement (required)
- `id` (string, unique)
- `name` (string)
- `badge` (enum)
- `conditions` (array)

### Condition (required)
- `type` (enum)
- `key` (string)
- `value` (depends on type)

## Optional fields
- Root:
  - `supported_versions` (array)
- Achievement:
  - `description` (string, default `""`)
  - `hidden` (bool, default `false`)

## Supported badge tiers
- `leaf`
- `compass`
- `bonfire`
- `star`

## Supported condition types (MVP)
- `state_equals`
- `state_greater_than`
- `state_less_than`
- `state_bit_set`

## Example valid pack
```json
{
  "schema_version": 1,
  "title_id": "0100000000000000",
  "game": {
    "name": "Example Game",
    "developer": "Example Developer"
  },
  "supported_versions": [
    {
      "game_version": "1.0.0",
      "build_id": "EXAMPLEBUILDID"
    }
  ],
  "achievements": [
    {
      "id": "first_steps",
      "name": "First Steps",
      "description": "Start your first adventure.",
      "badge": "leaf",
      "hidden": false,
      "conditions": [
        {
          "type": "state_equals",
          "key": "has_started_game",
          "value": true
        }
      ]
    }
  ]
}
```

## Example invalid pack
```json
{
  "title_id": "0100000000000000",
  "game": { "name": "Broken", "developer": "Example" },
  "achievements": [
    {
      "name": "Missing ID",
      "badge": "diamond",
      "conditions": [
        { "key": "has_started_game", "value": true }
      ]
    }
  ]
}
```

Invalid because:
- missing `schema_version`
- achievement missing `id`
- unsupported badge tier `diamond`
- condition missing `type`
