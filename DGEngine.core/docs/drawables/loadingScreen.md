# LoadingScreen

A `loadingScreen` is a UI element that is shown when the game is loading. No other drawable
is drawn when a loading screen is active.

### Example

```json
"loadingScreen": {
  "texture": "cut2",
  "size": [533, 22],
  "anchor": "none",
  "progressBarOffset": [53, 37],
  "color": "0x700901",
  "onComplete": {
    "name": "game.close"
  }
}
```

### Properties

Name                | Type    | Default      | Description
------------------- | ------- | ------------ | ----------------------------
`texture`           | text    |              | texture
`texturePack`       | text    |              | texturePack used for loading screen animation
`frames`            | intVec  | [min,max]    | texturePack frame range
`textureRect`       | intRect | texture size | teture rect. size of the loadingScreen
`color`             | text    | transparent  | progress bar color
`progressBarOffset` | text    |              | progress bar position offset
`size`              | text    |              | progress bar size
`onComplete`        | text    |              | on complete action

Properties in **bold** are required.  

When both `texture` and `texturePack` are set, `texture` takes precedence.  

If `texturePack` is set, the animation is updated based on the current loading screen progress.

### Examples

#### LoadingScreen using texture and progress bar

```json
{
  "init": true,
  "loadingScreen": {
    "texture": "loadingTexture",
    "size": [533, 22],
    "anchor": "none",
    "progressBarOffset": [53, 37],
    "color": "0x700901",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```

#### LoadingScreen using texture and progress bar

```json
{
  "init": true,
  "loadingScreen": {
    "texturePack": "loadingAnimation",
    "size": [533, 22],
    "anchor": "none",
    "progressBarOffset": [53, 37],
    "color": "0x700901",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```

#### LoadingScreen using only progress bar

```json
{
  "init": true,
  "loadingScreen": {
    "size": [533, 22],
    "anchor": "none",
    "progressBarOffset": [53, 37],
    "color": "0x700901",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```

#### Empty loadingScreen

```json
{
  "init": true,
  "loadingScreen": {
    "anchor": "none",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```
