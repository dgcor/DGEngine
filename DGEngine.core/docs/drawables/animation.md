# Animation

An `animation` is a UI element with a `texturePack` to provide the different images and refresh rate.
An animation inherits from `image` and can have a color and an outline.

### Example

```json
"animation": {
  "id": "anim",
  "texturePack": "logo"
}
```

### Properties

Name              | Type    | Default               | Description
----------------- | ------- | --------------------- | ----------------------------
`animationType`   | text    | looped                | animation type (looped, backAndForth, playOnce)
`color`           | color   | white                 | fill color
`directionIndex`  | int     | -1                    | texturePack direction index
`enableOutline`   | bool    | false                 | enable outline (sprite shader)
`frames`          | int     | 1 or texturePack size | number of frames
`groupIndex`      | int     | -1                    | texturePack group index
`outline`         | color   | transparent           | outline color (sprite shader)
`outlineIgnore`   | color   | transparent           | outline ignore color (sprite shader)
`pause`           | bool    | false                 | pause state
`refresh`         | time    | 50                    | refresh rate
**`texture`**     | texture | null                  | texture to use
**`texturePack`** | texture | null                  | texturePack to use

Properties in **bold** are required.  

An animation requires a `texture` or a `texturePack`. If both properties are set, `texture` takes precedence.  

All animations use the game's current sprite shader, which applies palettes and outline effects.

### Examples

#### Animation

```json
{
  "init": true,
  "animation": {
    "id": "anim",
    "texturePack": "logo",
    "refresh": 50
  }
}
```
