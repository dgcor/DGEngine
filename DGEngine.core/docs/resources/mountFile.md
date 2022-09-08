# MountFile

A `mountFile` is a resource used to mount folders and/or archives to load resources from.

### Example

```json
"mountFile": {
  "file": "gamefiles.zip"
}
```

### Properties

Name         | Type | Default | Description
------------ | ---- | ------- | ----------------------------
`append`     | bool | true    | append the mounted path to the existing mounted paths
**`file`**   | text |         | archive path
`mount`      | text |         | path to mount archive into
`unmount`    | bool | false   | unmount archive
`useSaveDir` | bool | false   | prepend the save dir to the file instead of the game dir

Properties in **bold** are required.  

Mounted archives are processed from last to first by default. If a file exists
in both archives, it's loaded from the last registered archive.  

When you mount/unmount an archive, DGEngine will try to mount the archive as-is. If it fails to mount
the archive, it will try and mount the same archive without a file extension and with a different extension.
Default file extensions processed: `zip` and `7z`, in this order.

### Examples

#### Mount 2 archives where the last archive overrides the first

```json
{
  "init": true,
  "mountFile": [
    { "file": "gamefiles.zip" },
    { "file": "gamefiles.mod.zip" }
  ]
}
```

#### Mount .temp folder in the current save directory

```json
{
  "init": true,
  "mountFile": {
    "file": ".temp",
    "append": false,
    "useSaveDir": true
  }
}
```
