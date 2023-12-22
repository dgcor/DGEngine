# TexturePack

A `texturePack` is a resource that stores a collection of images in GPU memory.
Textures can be accessed by index.  

`texturePack`s can be constructed from `imageContainer`s, `texture`s and `texturePacks`.  

When you `get` a texture using an `index` you can get a texture or a vector of textures.
In this last case, it means the final texture is the combination of all those textures.  

A `texturePack` can have multiple groups and directions. A simple texturePack will have
1 group and 1 direction.  

A `texturePack` for a player animation can have n groups and n animations. For example,
a player animation can have 2 groups (0 - standing and 1 - walking) and 8 directions, so
when the player is walking down, the group index will be 1 and the direction 0 or some
other value.

### Example

```json
"texturePack": {
  "id": "itemIcons",
  "imageContainer": "objcurs",
  "palette": "town"
}
```

### Properties

There are a number of different types of texturePacks:

#### BitmapFontTexturePack

Name                 | Type | Default    | Description
-------------------- | ---- | ---------- | ----------------------------
`charSizeFile`       | text |            | binary or text file with char widths as bytes or numbers
`charSizeFileStep`   | int  | 1          | number of bytes to advance when reading char widths in a loop
`charSizeFileStart`  | int  | 0          | start position to start reading char withds
`direction`          | text | horizontal | texture char direction. value can be `horizontal` or `vertical`
`fromId`             | text |            | create an alias from an existing id
**`imageContainer`** | text |            | imageContainer
`palette`            | text |            | texture palette
`newLine`            | int  | 0          | new line size. 0 to use value from texture index. -1 to calculate based on letter size
`space`              | int  | 0          | space size. 0 to use value from texture index. -1 to calculate based on letter size
`tab`                | int  | 0          | tab size. 0 to use value from texture index. -1 to calculate based on letter size
`cols`               | int  | 16         | number of char rows in texture
`rows`               | int  | 16         | number of char columns in texture
**`texture`**        | text |            | texture
**`type`**           | text |            | texturePack type `font`

Properties in **bold** are required.  

A font `texturePack` is a texturePack tailored for bitmap fonts, where you can specify
the size of the chars. If no `charSizeFile` is used, the widths will be calculated.  

If a `charSizeFile` is provided, the widths are only calculated if the size of the file
is not 128 bytes or 256 bytes and is smaller than 256 bytes. If the size of the file is
130 bytes or 258 bytes, then it is assumed the first 2 bytes are for `space` and `newLine`.
Otherwise, the space and newLine, if not overridden, will come from the `charSizeFile`.  

A `charSizeFile` can be a text file with 128/130/256/258 numbers separated by a newLine.  

If an `imageContainer` is used, all other properties are ignored except `palette` .  

Font `texturePack`s have 1 group, 1 direction and 1 texture.

#### CompositeTexturePack

Name                   | Type            | Default | Description
---------------------- | --------------- | ------- | ----------------------------
`directionLayerOrders` | array of intArr |         | array of arrays of direction indexes
`fromId`               | text            |         | create an alias from an existing id
`groups`               | array of json   |         | groups of composite texturePacks
**`texturePacks`**     | textArr         |         | array of texturePacks
**`type`**             | text            |         | texturePack type `composite`

Properties in **bold** are required.  

A composite `texturePack` is a texturePack that has multiple textures for each index.
The final texture is the combination of all those textures.  

A `group` of composite texturePacks creates a group by defining the `texturePacks`
element inside the group.

#### ImageContainerTexturePack

Name                 | Type   | Default | Description
-------------------- | ------ | ------- | ----------------------------
`fromId`             | text   |         | create an alias from an existing id
**`imageContainer`** | text   |         | imageContainer
`offset`             | intVec |         | global offset
`palette`            | text   |         | textures palette

Properties in **bold** are required.  

An `imageContainer` `texturePack` is a texturePack that gets the textures from an imageContainer.
Textures from `imageContainer`s are decoded on request and cached for future use.  

#### MultiImageContainerTexturePack

Name                 | Type          | Default | Description
-------------------- | ------------- | ------- | ----------------------------
`fromId`             | text          |         | create an alias from an existing id
**`imageContainer`** | array of text |         | imageContainers
`offset`             | intVec        |         | global offset
`palette`            | text          |         | textures palette

Properties in **bold** are required.  

Each imageContainer is treated as a group. The number of directions in each group comes
from the imageContainer.

#### MultiTexturePack

Name          | Type          | Default | Description
------------- | ------------- | ------- | ----------------------------
`fromId`      | text          |         | create an alias from an existing id
**`texture`** | array of json |         | array of texture objects
`offset`      | intVec        |         | global offset
`palette`     | text          |         | textures palette

#### texture properties

Name            | Type   | Default   | Description
--------------- | ------ | --------- | ----------------------------
`direction`     | text   | horiontal | texture direction. value can be `horizontal` or `vertical`
`frames`        | intVec | [1,1]     | frame range
**`id`**        | text   |           | texture id
`offset`        | intVec |           | texture offset
`startIndex`    | int    | 0         | start index for the first texture

Properties in **bold** are required.  

A `MultiTexturePack` is a texturePack that uses a number of `texture`s. Each `texture`
object is treated as a texture group and has the same properties as a `SingleTexturePack`.

#### SingleTexturePack

Name            | Type                   | Default   | Description
--------------- | ---------------------- | --------- | ----------------------------
`direction`     | text                   | horiontal | texture direction. value can be `horizontal` or `vertical`
`frames`        | intVec                 | [1,1]     | frame range
`fromId`        | text                   |           | create an alias from an existing id
`offset`        | intVec                 |           | global offset
`palette`       | text                   |           | textures palette
`startIndex`    | int                    | 0         | start index for the first texture
**`texture`**   | text                   |           | texture

Properties in **bold** are required.  

A `SingleTexturePack` is a `MultiTexturePack` with only 1 `texture`.

#### StackedTexturePack

Name               | Type          | Default | Description
------------------ | ------------- | ------- | ----------------------------
`fromId`           | text          |         | create an alias from an existing id
**`texturePacks`** | text or array |         | array of texturePacks

Properties in **bold** are required.  

A stacked `texturePack` is simply a collection of texture packs.
When you get a texture from a StackedTexturePack, the list of texturePacks
is processed until a valid texture is returned from one of them.  

Usually, the texturePacks don't have overlapping indexes.

### Common properties

All `texturePack`s with the exception of `CompositeTexturePack`s and `StackedTexturePack`s
can have these properties defined.  

The order in which these are defined affects the output. if `groups` come after `rects`,
then the frame range defined in the groups will go from [0, rectSize].

#### Groups

Name         | Type          | Default | Description
------------ | ------------- | ------- | ----------------------------
**`groups`** | array of json |         | group objects

Properties in **bold** are required.  

The `groups` array/object can define json objects with the following properties for each group:

Name            | Type                   | Default   | Description
--------------- | ---------------------- | --------- | ----------------------------
`animationType` | text                   | looped    | animation type (looped, backAndForth, playOnce)
`directions`    | int or array of intArr | 0         | number of directions or array with direction ranges (size is number of directions)
`range`         | intVec                 | [min,max] | frame range
`refresh`       | time                   | 0         | texturePack animation refresh rate

#### Rects

Name              | Type  | Default | Description
----------------- | ----- | ------- | ----------------------------
`absoluteOffsets` | bool  | false   | use absolute or relative offsets
`invertOffsets`   | bool  | false   | invert offsets. [5,6] becomes [-5,-6]
**`rects`**       | array |         | array of rect objects or intRects

Properties in **bold** are required.  

The `rects` array can define json objects with the following properties for each rect:

Name      | Type    | Default | Description
--------- | ------- | ------- | ----------------------------
`index`   | int     | 0       | original texture index to apply the rect to
`offset`  | intVec  |         | offset to apply to the texture
`rect`    | intRect |         | texture rect to use

The global `offset`, if defined, gets added to each offset.

#### Indexes

Name                       | Type                      | Default | Description
-------------------------- | ------------------------- | ------- | ----------------------------
`onlyUseIndexed`           | bool                      | true    | only use indexed texture. false to use both new and old indexes
`reverseMapIndex`          | bool                      | false   | reverse index mapping
**`textureIndexes`**       | intArr or array of intVec |         | array of indexes
**`textureIndexRange`**    | intVec                    |         | index range
`textureIndexRangeStart`   | intVec                    | -1      | index range to start mapping to (if >= 0)
**`utf8Indexes`**          | text                      |         | text with indexes (usually a section of the ASCII table)
**`utf8IndexFile`**        | text                      |         | file to load text with indexes

One of the properties in **bold** is required.  

You can define indexes from utf8 text and `textureIndexes` at the same time.
They are applied in that order.  

Utf8 indexes and `textureIndexes` defined as `intArr` map the number (or char -> int)
to the current number of indexes. Ex: ` textureIndexes: [10,11,12] ` will map texture index 10
to texture 0, texture index 11 to texture 1 and texture index 12 to texture 2.  

If you pass an array of `intVec` to `textureIndexes`, ex: ` textureIndexes: [[5,0],[6,1],[7,2]] `,
you will map texture index 5 to texture 0, texture index 6 to texture 1 and texture index 7 to texture 2.  

If you set `textureIndexRange` and `textureIndexes` is not set, the range defined is mapped
to the current number of indexes. Ex: ` textureIndexRange: [10, 11] ` will map texture index 10
to texture 0 and texture index 11 to texture 1. The end index is inclusive.  

You can also specify the `textureIndexRange` with a start index lower than the stop index.
Ex: ` textureIndexRange: [11, 10] ` will map texture index 11 to texture 0 and texture index 10
to texture 1. If the texturePack already has indexes mapped, it will start at the next free index.  

If `reverseMapIndex` is `true` then `textureIndexes` are mapped in reverse order.
Ex: ` textureIndexRange: [11, 10] ` will map texture index 0 to texture 11 and texture index 1
to texture 10. Only valid for `utf8Indexes`, `textureIndexes` (intArr) and `textureIndexRange`.  

To specify a different index for `textureIndexRange`, set `textureIndexRangeStart` to a number
above 0.

#### Animated textures

Name                       | Type                      | Default     | Description
-------------------------- | ------------------------- | ----------- | ----------------------------
**`animatedTextures`**     | array                     |             | animated texture json object
`index`                    | int                       | first index | animated texture index
`indexes`                  | intArr or array of intVec |             | array of indexes. first index is used as the animated texture index
`refresh`                  | time                      | 50          | animation refresh rate

Properties in **bold** are required.  

TexturePacks can be used for animations or level tiles. When used for animations, all textures
in the texture pack are part of the animation or animations, in case there are groups and directions.
For level tiles texturePacks, the tiles are fixed and each index is instead mapped to each tile
in the level. `animatedTextures` allows you to define level tiles that animate.

### Examples

#### BitmapFontTexturePack

```json
{
  "init": true,
  "texturePack": {
    "id": "font6",
    "type": "font",
    "texture": "font6",
    "charSizeFile": "data/local/font/LATIN/font6.tbl",
    "charSizeFileStart": 15,
    "charSizeFileStep": 14,
    "newLine": 7,
    "direction": "horizontal"
  }
}
```

#### CompositeTexturePack with 8 directions

```json
{
  "init": true,
  "texturePack": {
    "id": "roguePlateAxe",
    "type": "composite",
    "texturePacks": [
      "f_battle_axe",
      "f_plate_boots",
      "f_plate_greaves",
      "f_chain_gloves",
      "f_plate_cuirass",
      "f_plate_helm"
    ],
    "directionLayerOrders": [
      [0, 1, 2, 3, 4, 5, 6],
      [0, 1, 2, 3, 4, 5, 6],
      [0, 1, 2, 3, 4, 5, 6],
      [0, 1, 2, 3, 4, 5, 6],
      [1, 2, 3, 4, 5, 6, 0],
      [1, 2, 3, 4, 5, 6, 0],
      [1, 2, 3, 4, 5, 6, 0],
      [1, 2, 3, 4, 5, 6, 0]
    ]
  }
}
```

#### CompositeTexturePack with 2 groups

```json
{
  "init": true,
  "texturePack": {
    "id": "sorceressNormal",
    "type": "composite",
    "groups": [
      {
        "texturePacks": [
          "SOHDLITNUHTH",
          "SOTRLITNUHTH",
          "SOLGLITNUHTH",
          "SORALITNUHTH",
          "SOLALITNUHTH",
          "SOSHKITNUHTH",
          "SOS1LITNUHTH",
          "SOS2LITNUHTH"
        ]
      },
      {
        "texturePacks": [
          "SOHDLITTWHTH",
          "SOTRLITTWHTH",
          "SOLGLITTWHTH",
          "SORALITTWHTH",
          "SOLALITTWHTH",
          "SOSHKITTWHTH",
          "SOS1LITTWHTH",
          "SOS2LITTWHTH"
        ]
      }
    ]
  }
}
```

#### ImageContainerTexturePack

```json
{
  "init": true,
  "imageContainer": {
    "file": "towners/animals/cow.cel"
  },
  "texturePack": {
    "id": "cow",
    "imageContainer": "cow",
    "palette": "town"
  }
}
```

#### StackedTexturePack with 2 texturePacks

```json
{
  "init": true,
  "imageContainer": [
    {
      "id": "basewalls",
      "file": "data/global/tiles/ACT1/CATACOMB/basewalls.dt1"
    },
    {
      "id": "floor",
      "file": "data/global/tiles/ACT1/CATACOMB/floor.dt1"
    }
  ],
  "texturePack": [
    {
      "id": "basewalls",
      "imageContainer": "basewalls",
      "palette": "level"
    },
    {
      "id": "floor",
      "imageContainer": "floor",
      "palette": "level"
    },
    {
      "id": "catacombs",
      "texturePacks": [
        "basewalls",
        "floor"
      ]
    }
  ]
}
```

#### TexturePack with 4 textures defined using rects

```json
{
  "init": true,
  "texturePack": {
    "id": "automap",
    "texture": "automap",
    "rects": [
      { "rect": [516,  0, 17,  9], "offset": [ 0, -8] },
      { "rect": [516,  9, 17,  9], "offset": [-8, -4] },
      { "rect": [516, 18, 17,  9], "offset": [ 0, -4] },
      { "rect": [516, 27, 17,  9], "offset": [ 8, -4] }
    ]
  }
}
```
