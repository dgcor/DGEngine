# Circle

A `circle` is a UI element with a radius and it can have a color, an outline or a texture.

### Example

```json
"circle": {
  "id": "circle",
  "position": ["center", "center"],
  "radius": 16,
  "color": "0xFF0000"
}
```

### Properties

Name               | Type    | Default | Description
------------------ | ------- | ------- | ----------------------------
`color`            | color   | white   | fill color
`outlineColor`     | color   | white   | border/outline color
`outlineThickness` | int     | 0       | border thickness
`radius`           | int     | 0       | radius
`texture`          | texture | null    | texture to use
`textureRect`      | intRect | [0,0]   | textureRect to use. If the property is present but invalid, the current window size is used

Properties in **bold** are required.

A circle's size is read only and is calculated from the radius.

### Examples

#### Circle with red fill

![](../img/circle-1.png)

```json
{
  "init": true,
  "circle": {
    "id": "circle",
    "position": ["center", "center"],
    "radius": 16,
    "color": "0xFF0000"
  }
}
```

#### Circle with black and white checkers pattern texture

![](../img/circle-2.png)

```json
{
  "init": true,
  "texture": {
    "id": "pattern",
    "color": [
      "0xFFFFFF", "0x000000",
      "0x000000", "0xFFFFFF"
    ],
    "size": [2, 2]
  },
  "circle": {
    "id": "circle",
    "position": ["center", "center"],
    "radius": 16,
    "texture": "pattern",
    "textureRect": [32, 32]
  }
}
```

#### Circle with stretched black and yellow checkers pattern texture

![](../img/circle-3.png)

```json
{
  "init": true,
  "texture": {
    "id": "pattern",
    "color": [
      "0xFFFF00", "0x000000",
      "0x000000", "0xFFFF00"
    ],
    "size": [2, 2]
  },
  "circle": {
    "id": "circle",
    "position": ["center", "center"],
    "radius": 16,
    "texture": "pattern",
    "textureRect": [8, 8]
  }
}
```

#### Circle with black and white checkers pattern texture, blue 2px border and yellow fill color

![](../img/circle-4.png)

```json
{
  "init": true,
  "texture": {
    "id": "pattern",
    "color": [
      "0xFFFFFF", "0x000000",
      "0x000000", "0xFFFFFF"
    ],
    "size": [2, 2]
  },
  "circle": {
    "id": "circle",
    "position": ["center", "center"],
    "radius": 16,
    "color": "0xFFFF00",
    "outlineColor": "0x0000FF",
    "outlineThickness": 2,
    "texture": "pattern",
    "textureRect": [32, 32]
  }
}
```
