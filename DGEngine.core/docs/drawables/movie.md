# Movie

A `movie` is a UI element that plays a movie, including its audio. When movie support is disabled,
playing a movie simply shows a black rectangle and calls the `onComplete` action.

### Example

```json
"movie": {
  "id": "movie1",
  "file": "resources/movie.avi"
}
```

### Properties

Name              | Type    | Default      | Description
----------------- | ------- | ------------ | ----------------------------
**`file`**        | text    |              | movie file
`onComplete`      | text    |              | on complete action
`volume`          | int     | music volume | audio volume

Properties in **bold** are required.  

### Examples

#### Play movie

```json
{
  "init": true,
  "movie": {
    "id": "movie1",
    "file": "resources/movie.avi",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```

#### Play movie at full screen

```json
{
  "init": true,
  "movie": {
    "id": "movie1",
    "replaceVars": "value",
    "file": "resources/movie.avi",
    "size": ["|game.refSize.x|", "|game.refSize.y|"],
    "anchor": "all",
    "onComplete": {
      "name": "game.close"
    }
  }
}
```
