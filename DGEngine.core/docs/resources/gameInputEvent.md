# GameInputEvent

A `gameInputEvent` is a key/value pair that maps a key to a number of `inputEvent`s (key, scancode).  

A `gameInputEvent` is useful to aggregate all of a game's possible actions in a single file
so that if you want to change the default `enter` key to something else, you don't have to change
multiple files.

### Example

```json
"gameInputEvent": {
  "name": "enter",
  "key": ["enter", " "]
}
```

### Properties

Name            | Type          | Default | Description
--------------- | ------------- | ------- | ----------------------------
**`key`**       | text or array |         | keyboard key to use as key
`keyUp`         | bool          | false   | trigger on key/scancode release
**`name`**      | text          |         | keyboard key to use as key
**`scancode`**  | text or array |         | keyboard scancode to use as key

Properties in **bold** are required.  

### Examples

#### GameInputEvent associating multiple keyboard keys to the close action

```json
{
  "init": true,
  "gameInputEvent": {
    "name": "enter",
    "key": ["enter", " ", "esc"]
  },
  "inputEvent": {
    "gameInputEvent": "enter",
    "action": { "name": "game.close" }
  }
}
```
