# ImageContainer

An `imageContainer` is a resource that contains a collection of images.
They are used by `texturePack`s to get textures from them.

### Example

```json
"imageContainer": {
  "id": "PentSpin",
  "file": "data/PentSpin.cel"
}
```

### Properties

Name         | Type   | Default | Description
------------ | ------ | ------- | ----------------------------
**`file`**   | text   |         | resource file
`fileBytes`  | text   |         | resource file from `file` resource
`fromId`     | text   |         | create an alias from an existing id
`frames`     | intVec |         | number of frames
`directions` | int    |         | number of directions
`blendMode`  | int    |         | blend mode to use when drawing

Properties in **bold** are required.  

### Examples

#### ImageContainer

```json
{
  "init": true,
  "imageContainer": {
    "id": "PentSpin",
    "file": "data/PentSpin.cel"
  }
}
```

#### ImageContainer with 8 frames

```json
{
  "init": true,
  "imageContainer": {
    "file": "data/image1.png"
    "frames": 8
  }
}
```

#### ImageContainer from file

```json
{
  "init": true,
  "file": {
    "id": "PentSpin",
    "file": "data/PentSpin.cel"
  },
  "imageContainer": {
    "id": "PentSpin",
    "fileBytes": "PentSpin"
  }
}
```
