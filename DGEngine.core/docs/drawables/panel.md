# Panel

A `panel` is a UI element that contains other drawables.  

The most common use case is to group many UI elements inside a panel so you can show
or hide multiple drawables by showing or hiding the panel.

### Example

```json
"panel": {
  "id": "panel1"
}
```

### Properties

Name                | Type | Default | Description
------------------- | ---- | ------- | ----------------------------
`relativePositions` | bool | false   | use relative positions

Properties in **bold** are required.  

When `relativePositions` is true, the elements inside the panel use the panel's position
as the origin, so a drawabl at position [10,10] would be drawn at the panel's current
position + [10,10]. This only applies when creating the panel. Anchoring a panel
has no effect on its elements.

### Examples

#### Circle inside a panel drawn at position 0,0

```json
{
  "init": true,
  "panel": {
    "id": "panel1",
    "position": [200, 200]
  },
  "circle": {
    "id": "circle",
    "panel": "panel1",
    "radius": 16,
    "color": "0xFF0000"
  }
}
```

#### Circle inside a panel drawn at the top right corner

```json
{
  "init": true,
  "panel": {
    "id": "panel1",
    "position": [608, 0],
    "relativePositions": true
  },
  "circle": {
    "id": "circle",
    "panel": "panel1",
    "radius": 16,
    "color": "0xFF0000"
  }
}
```
