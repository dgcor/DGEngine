# InputEvent

An `inputEvent` is a key/value pair that maps 1-n keys to an `action` or `gameEvent`.

### Example

```json
"inputEvent": {
  "key": "esc",
  "action": { "name": "button.click", "id": "back" }
}
```

### Properties

Name            | Type          | Default | Description
--------------- | ------------- | ------- | ----------------------------
**`gameEvent`** | text or array |         | gameEvent name to use as key
**`key`**       | text or array |         | keyboard key to use as key
`keyUp`         | bool          | false   | trigger on key/scancode release
**`scancode`**  | text or array |         | keyboard scancode to use as key
`action`        | action        | null    | action to use as value
`event`         | text          |         | event name to use as value

Properties in **bold** are required.  

Setting the action or event to `null` clears the `inputEvent`.  

`event` is a text name of an action that a specific game entity handles.
The entities created must be coded to handle events. They're useful to
process keys and key combinations while they are pressed, such as the combination
of directional keys to move a player on a level.

### Examples

#### InputEvent using keyboard key

```json
{
  "init": true,
  "inputEvent": {
    "key": "esc",
    "action": { "name": "game.close" }
  }
}
```

#### InputEvent using gameInputEvent for keys enter, space, esc and scancode q

```json
{
  "init": true,
  "gameInputEvent": [
    { "name": "enter", "key": ["enter", " "] },
    { "name": "esc", "key": "esc", "scancode": "q" }
  ],
  "inputEvent": {
    "gameInputEvent": ["enter", "esc"],
    "action": { "name": "game.close" }
  }
}
```

#### InputEvent mapping keys to events (level class should handle these event names)

```json
{
  "init": true,
  "inputEvent": [
    {
      "key": "up",
      "scancode": "w",
      "event": "level.up"
    },
    {
      "key": "down",
      "scancode": s",
      "event": "level.down"
    },
    {
      "key": "left",
      "scancode": "a",
      "event": "level.left"
    },
    {
      "key": "right",
      "scancode": "d",
      "event": "level.right"
    }
  ]
}
```

#### InputEvent mapping gameInputEvents to events (level class should handle these event names)

```json
{
  "init": true,
  "gameInputEvent": [
    { "name": "up", "key": "up", "scancode": "w" },
    { "name": "down", "key": "down", "scancode": "s" },
    { "name": "left", "key": "left", "scancode": "a" },
    { "name": "right", "key": "right", "scancode": "d" }
  ],
  "inputEvent": [
    {
      "gameInputEvent": "up",
      "event": "level.up"
    },
    {
      "gameInputEvent": "down",
      "event": "level.down"
    },
    {
      "gameInputEvent": "left",
      "event": "level.left"
    },
    {
      "gameInputEvent": "right",
      "event": "level.right"
    }
  ]
}
```
