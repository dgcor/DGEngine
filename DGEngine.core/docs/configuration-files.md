# Configuration Files

DGEngine use JSON files to create user interfaces and everything that happens in-game.
These JSON files are not 100% valid JSON files, since you can have repeated elements (not standard JSON).
They are processed in order.  

Configuration properties are divided into categories:

* Game Configuration
* Resources
* Drawables
* Actions

## Json types

Type     | Description                              | Examples
-------- | ---------------------------------------- | ----------------------------
action   | action object (json) or action id (text) | `{ "acion": { "name": "game.close" } }`, `{ "acion": "close" }`
bool     | boolean                                  | `{ "bool": true }`
double   | decimal number                           | `{ "double": 10.0 }`, `{ "double": -5.12 }`
enum     | enum value (text) or values (json array) | `{ "enum": "top" }`, `{ "enum": ["top", "left"] }`
id       | id value (text). 0-9, a-z, A-Z or _      | `{ "id": "img1" }`
int      | number                                   | `{ "int": 10 }`, `{ "int": -5 }`
intArr   | json array with n ints                   | `{ "intArr": [0, 1, 2, 3, 4, 5, 6, 7] }`
intRect  | json array with 4 ints                   | `{ "intRect": [0, 0, 100, 100] }`
intVec   | json array with 2 ints                   | `{ "intVec": [0, 1] }`
json     | json element                             | `{ "json": { "id": "img1", "file": "img1.png" } }`
text     | utf8 string                              | `{ "text": "value" }`, `{ "text": "" }`, `{ "text": "Hello\nWorld" }`
textArr  | json array with n utf8 strings           | `{ "textArr": ["value1", "value2", "value3"] }`
time     | miliseconds (int) or seconds (double)    | `{ "time": 50 }` is equivalent to `{ "time": 0.05 }`
variable | game variable. bool, int, double or text | `{ "variable": 50 }`, `{ "variable": 2.5 }`, `{ "variable": "text" }`

## Game Configuration

[Drawables](drawables/game.md)

## Resources

[Resources](resources/readme.md)

## Drawables

[Drawables](drawables/readme.md)

## Actions

[Actions](actions/readme.md)
