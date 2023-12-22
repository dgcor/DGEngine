# Font

A `font` is a resource used to draw text. It can be either Bitmap or FreeType.

### Example

```json
"font": {
  "id": "liberationSerif",
  "file": "res/LiberationSerif-Regular.ttf"
}
```

### Properties

Name                    | Type  | Default | Description
----------------------- | ----- | ------- | ----------------------------
**`clone`**             | text  |         | clone from an existing id
**`file`**              | text  |         | resource file (FreeType)
`fontColor`             | color | white   | font color
`fontPalette`           | text  |         | font palette (Bitmap)
`fontSize`              | int   |         | font size (FreeType)
`fromId`                | text  |         | create an alias from an existing id
`horizontalSpaceOffset` | int   | 0       | horizontal space offset (Bitmap)
`newLine`               | int   |         | new line size (Bitmap)
`smooth`                | bool  | true    | smooth font textures (FreeType)
`space`                 | int   |         | space size (Bitmap)
`tab`                   | int   |         | tab size (Bitmap)
**`texturePack`**       | text  |         | texturePack to use for glyphs (Bitmap)
`verticalSpaceOffset`   | int   | 0       | vertical space offset (Bitmap)

Properties in **bold** are required.  

The `fromId` property is processed first, followed by `clone`.
A font requires a `file` (FreeType) or a `texturePack` (Bitmap). If both properties are set,
`texturePack` takes precedence.  

`fontColor`, `fontPalette` and `fontSize` are default values used in case the `text`
elements that uses the font don't specify these properties.  

Cloned fonts can override `fontColor`, `fontPalette` and `fontSize`.

### Examples

#### Bitmap font

```json
{
  "init": true,
  "texture": {
    "id": "font16g",
    "file": "ui_art/font16g.pcx",
    "split": "vertical",
    "pieces": 16,
    "mask": "0x00FF00"
  },
  "texturePack": {
    "id": "font16g",
    "type": "font",
    "texture": "font16g",
    "charSizeFile": "ui_art/font16.bin",
    "rows": 16,
    "cols": 16,
    "direction": "vertical"
  },
  "font": {
    "id": "font16g",
    "texturePack": "font16g"
  }
}
```

#### FreeType font

```json
{
  "init": true,
  "font": {
    "id": "liberationSerif",
    "file": "res/LiberationSerif-Regular.ttf"
  }
}
```

#### FreeType font with default yellow color

```json
{
  "init": true,
  "font": {
    "id": "liberationSerif",
    "file": "res/LiberationSerif-Regular.ttf",
    "fontColor": "0xFFFF00"
  }
}
```

#### Cloned FreeType font with default yellow color

```json
{
  "init": true,
  "font": [
    {
      "id": "liberationSerif",
      "file": "res/LiberationSerif-Regular.ttf",
      "fontSize": 24
    },
    {
      "id": "liberationSerifYellow",
      "clone": "liberationSerif",
      "fontColor": "0xFFFF00"
    }
  ]
}
```
