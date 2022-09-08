# Menu

A `menu` is a UI element that presents a list of `StringButton`s and automatically positions
them based on the menu's position, size and drawing area of the buttons.  

String button properties are defined in the menu object and used for each button in the menu.

### Example

```json

"menu": {
  "id": "menu1",
  "font": "font42g",
  "position": [319, 192],
  "sound": "titlslct",
  "focus": true,
  "focusSound": "titlemov",
  "items": [
    {
      "text": "Play Game",
      "onClick": {
        "name": "load",
        "file": "game/load.json"
      }
    },
    {
      "text": "Exit Game",
      "onClick": {
        "name": "game.close"
      }
    }
  ]
}
```

### Properties

Name                      | Type          | Default | Description
------------------------- | ------------- | ------- | ----------------------------
**`<button properties>`** |               |         | string button properties, except actions
**`items`**               | json or array |         | string button items
`onScrollDown`            | text          |         | scroll down action
`onScrollUp`              | bool          |         | scroll up action
`verticalPad`             | int           | 0       | vertical pad between items
`visibleItems`            | int           | 0       | number of visible items in menu. 0 to show all

Properties in **bold** are required. Only required `button` properties are required.  

Each item will use the button properties. There are some additional properties that
can be set for each item to override the default values:

Name               | Type   | Default   | Description
------------------ | ------ | --------- | ----------------------------
`<button actions>` |        |           | string button actions
`enable`           | int    | <default> | enable button
`executeAction`    | text   |           | action to execute after parsing button item
`load`             | text   |           | load items query
`position`         | intVec | <default> | fixed position
`text`             | text   |           | button text

The indexes for the current menu item `!menuIdx!` are set for each item that's processed.
These values are replaced before parsing the item.  

The `load` property must query for a property that is implemented in the game's
`getQueryableList` function. A list of `variable`s or `Queryable`s is returned.  

There are 3 `load` variables that load properties: `game.saveDirs`, `game.dirs`, `game.files`.  

Load       | Parameter | Description
---------- | --------- | ----------------------------
`dirs`     | path      | list dirs in path
`files`    | path      | list files in path
`saveDirs` |           | list dirs in save folder path

The indexes for the current item `!listIdx!` and current menu item `!menuIdx!` are set
for each `Queryable` item that's processed. These values are replaced before parsing the item.
If the returned values are `variable`s, the `!listItem!` holds the variable value.  

If the `load` property is set, there are some additional properties that can be set:

Name      | Type | Default | Description
--------- | ---- | ------- | ----------------------------
`exclude` | json |         | exclude filter
`include` | json |         | include filter

If both `exclude` and `include` are set, `exclude` takes precedence.  

the filter properties have these properties:

Name           | Type              | Default | Description
-------------- | ----------------- | ------- | ----------------------------
if             | text              | ==      | compare operator. `==`, `!=`, `>`, `>=`, `<`, `<=`
**`property`** | text              |         | query property to filter
`value`        | variable or array |         | value to compare

Properties in **bold** are required.

The property to query implements the `Queryable` interface.

### Examples

#### Menu

```json
{
  "init": true,
  "menu": {
    "id": "menu1",
    "font": "font42g",
    "position": [319, 192],
    "sound": "titlslct",
    "focus": true,
    "focusSound": "titlemov",
    "items": [
      {
        "text": "Play Game",
        "onClick": {
          "name": "load",
          "file": "game/load.json"
        }
      },
      {
        "text": "Exit Game",
        "onClick": {
          "name": "game.close"
        }
      }
    ]
  }
}
```

#### Menu that loads all the dirs in the save dir and a "New Hero" menu item

```json
{
  "init": true,
  "menu": {
    "id": "menuLoadGame",
    "font": "font24g",
    "position": [423, 260],
    "horizontalAlign": "center",
    "visibleItems": 6,
    "size": [270, 164],
    "items": [
      {
        "load": "game.saveDirs",
        "text": "!listItem!",
        "onClick": { "name": "load", "file": ["ui/loadCharInfo.json", "%focus.text%"] }
      },
      {
        "text": "New Hero",
        "onClick": { "name": "load", "file": "ui/charInfoDefault.json" }
      }
    ]
  }
}
```

#### Menu that loads player items's recharge cost property

```json
{
  "init": true,
  "menu": {
    "id": "menuPrices",
    "font": "smaltext",
    "sound": "titlslct",
    "position": [571, 94],
    "horizontalAlign": "right",
    "horizontalSpaceOffset": 1,
    "anchor": "none",
    "focusOnClick": false,
    "visibleItems": 4,
    "verticalPad": 38,
    "items": {
      "load": "currentLevel.currentPlayer.inventory.stash",
      "text": "%rechargeCost%"
    }
  }
}
```

#### Menu that filters only for books and potions

```json
{
  "init": true,
  "menu": {
    "id": "mainMenu",
    "font": "smaltext",
    "sound": "titlslct",
    "position": [52, 94],
    "horizontalSpaceOffset": 1,
    "verticalSpaceOffset": 2,
    "anchor": "none",
    "focus": true,
    "focusOnClick": false,
    "focusSound": "titlemov",
    "visibleItems": 4,
    "size": [518, 186],
    "verticalPad": 14,
    "onScrollDown": "focus.moveDown",
    "onScrollUp": "focus.moveUp",
    "items": [
      {
        "load": "currentLevel.currentPlayer.inventory.stash",
        "include": {
          "property": "itemType",
          "value": ["Book", "Potion"]
        },
        "text": "%name%\n",
        "onClick": {
          "name": "load",
          "file": ["towners/adria/sell/confirm.json", 2, "!listIdx!"]
        },
        "executeAction": [
          {
            "name": "variable.set",
            "values": {
              "itemOwnerId": "hero",
              "itemInv": 2,
              "itemIndex": "!listIdx!",
              "menuIndex": "!menuIdx!"
            }
          },
          "setMenuItemText"
        ]
      }
    ]
  }
}
```
