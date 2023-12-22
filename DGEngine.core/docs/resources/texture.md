# Texture

A `texture` is a resource that stores an image in GPU memory.

### Example

```json
"texture": {
  "id": "background",
  "file": "background.png"
}
```

### Properties

Name       | Type    | Default     | Description
---------- | ------- | ----------- | ----------------------------
`color`    | intArr  |             | color array to create texture
**`file`** | text    |             | resource file
`fromId`   | text    |             | create an alias from an existing id
`mask`     | color   | transparent | color to replace with transparent
`pieces`   | int     | 1           | number of pieces to split texture into
`repeat`   | bool    | true        | repeat texture when textureRect is bigger than textute size
`size`     | intVec  |             | size of `color` array
`smooth`   | bool    | true        | smooth texture on stretch/scale
`split`    | text    |             | split texture into n `pieces`. value can be `horizontal` or `vertical`
`trim`     | intRect |             | trim texture outside rect. texture unchanged if rect is out of range

Properties in **bold** are required.  

Only file textures can be split into pieces and apply mask.
Both textures (file and color) can be trimmed.

### Examples

#### Texture with id automap

```json
{
  "init": true,
  "texture": {
    "file": "automap.png"
  }
}
```

#### Font texture with 256 char images split into a 16x16 image

The image has a green background, so we set the `mask` to green to ignore it.

```json
{
  "init": true,
  "texture": {
    "id": "font16g",
    "file": "ui_art/font16g.pcx",
    "split": "vertical",
    "pieces": 16,
    "mask": "0x00FF00"
  }
}
```

#### Color textures

```json
{
  "init": true,
  "texture": [
    {
      "id": "background",
      "color": "0x000000",
      "size": [1, 1]
    },
    {
      "id": "textWallBackground",
      "color": [
        "0x000000FF", "0x00000000",
        "0x00000000", "0x000000FF"
      ],
      "size": [2, 2]
    }
  ]
}
```

#### Trim texture

```json
{
  "init": true,
  "texture": {
    "id": "background",
    "file": "background.png",
    "trim": [0, 0, 640, 480]
  },
  "image": {
    "id": "img",
    "texture": "background",
    "background": true
  }
}
```
