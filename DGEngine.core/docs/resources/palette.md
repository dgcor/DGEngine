# Palette

A `palette` is a resource used with textures to draw images.

### Example

```json
"palette": {
  "id": "town",
  "file": "levels/towndata/town.pal"
}
```

### Properties

Name          | Type | Default | Description
------------- | ---- | ------- | ----------------------------
**`clone`**   | text |         | clone from an existing id
`colorFormat` | text | RGB     | color format
**`file`**    | text |         | resource file
`fromId`      | text |         | create an alias from an existing id
**`palette`** | text |         | existing palette id
**`trnFile`** | text |         | TRN file to transform an existing palette
`trnStart`    | int  | 0       | TRN file position to start reading
`trnLength`   | int  | 256     | TRN file bytes to read

Properties in **bold** are required.  

The `fromId` property is processed first.
A palette requires a `file`, a `palette` and a `trnFile` or a `clone`.
If all of these properties are set, they are processed in this order.  

a palette file is a binary file where each byte is a color channel.
The default format is a binary file of 256 RGB colors (768 bytes).  

a palette file can also be a text file (.txt) with commas, tabs or spaces
separating the RGB values. The file should have 256 lines. Example:  

```
0, 0, 0
10, 20, 30
...
```

a `TRN` file is a 256 byte file with the index to use from an existing palette
as the new color for the new palette and the index in the new palette is the
`TRN` file position in bytes. If `trnStart` is > 0, colors outside the file are black.

### Examples

#### Load palette with inferred id town

```json
{
  "init": true,
  "palette": {
    "file": "levels/towndata/town.pal"
  }
}
```

#### Load palette from existing town palette with inferred id infra and apply transformation

```json
{
  "init": true,
  "palette": [
    {
      "file": "levels/towndata/town.pal"
    },
    {
      "palette": "town",
      "trnFile": "plrgfx/infra.trn"
    }
  ]
}
```

#### Clone palette from existing town palette and replace some colors on the cloned palette

```json
{
  "init": true,
  "palette": [
    {
      "file": "levels/towndata/town.pal"
    },
    {
      "id": "textBlue",
      "clone": "town"
    }
  ],
  "action": [
    { "name": "palette.replace", "id": "textBlue", "idSource": "town", "srcStart": 178, "size": 14, "dstStart": 240 },
    { "name": "palette.replace", "id": "textBlue", "idSource": "town", "srcStart": 189, "size": 2, "dstStart": 254 }
  ]
}
```
