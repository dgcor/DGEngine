# Scrollable

A `scrollable` is a UI element that draws another UI element and scrolls it within an area
until it reaches the end of the scroll area.  

The most common use case is to scroll a long `text` element. You can scroll multiple drawables
by using a `panel`.

### Example

```json
"scrollable": {
  "id": "scroll1",
  "drawable": "creditsText",
  "refresh": 50,
  "anchor": "none",
  "position": [30, 115],
  "size": [580, 250],
  "onComplete": {
    "name": "game.close"
  }
}
```

### Properties

Name           | Type  | Default | Description
-------------- | ----- | ------- | ----------------------------
**`drawable`** | text  |         | drawable id to scroll
`refresh`      | time  | 50      | scroll refresh rate
`offset`       | int   | 0       | starting offset for scrolling area
`onComplete`   | text  |         | action to execite when scroll finishes
`pause`        | bool  | false   | pause scroll

Properties in **bold** are required.  

The scrolling area is set with `position` and `size`.  

The `drawable` that gets scrolled is removed from the main UI loop and is
drawn by the scrollable element instead.

### Examples

#### Scroll a circle

```json
{
  "init": true,
  "circle": {
    "id": "circle",
    "radius": 16,
    "color": "0xFF0000"
  },
  "scrollable": {
    "id": "creditsScroll",
    "drawable": "circle",
    "refresh": 50,
    "anchor": "none",
    "position": [30, 115],
    "size": [580, 250],
    "onComplete": {
      "name": "game.close"
    }
  }
}
```

#### Scroll a circle from the middle of the scrolling area

```json
{
  "init": true,
  "circle": {
    "id": "circle",
    "radius": 16,
    "color": "0xFF0000"
  },
  "scrollable": {
    "id": "creditsScroll",
    "drawable": "circle",
    "offset": -200,
    "refresh": 50,
    "anchor": "none",
    "position": [30, 115],
    "size": [580, 250],
    "onComplete": {
      "name": "game.close"
    }
  }
}
```
