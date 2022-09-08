# Audio

An `audio` is a resource that plays sound using an audio stream.

### Example

```json
"audio": {
  "id": "background",
  "file": "music/background.wav",
  "play": true
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

### Examples

#### Audio
