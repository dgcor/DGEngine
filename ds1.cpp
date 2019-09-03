#include "ds1.h"
#include <assert.h>
#include "gsl/gsl"
#include "PhysFSStream.h"
#include "StreamReader.h"
#include <sstream>

namespace DS1
{
    Decoder::Decoder(const std::string_view fileName)
    {
        std::vector<uint8_t> fileData;
        sf::PhysFSStream file(fileName.data());
        if (file.hasError())
          return;
        fileData.resize((size_t)file.getSize());
        file.read(fileData.data(), file.getSize());

        LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

        decode(fileStream);

        if (version < 9 || 13 < version)
          // FIXME: version 9 <= 13 causes crash here /w 4B remaining, why?
          assert(fileStream.remaining_size() == 0);
    }

    void Decoder::decode(LittleEndianStreamReader& fileStream)
    {
        fileStream.read(version);
        fileStream.read(width);
        width++;
        fileStream.read(height);
        height++;

        if (version < 8)
            act = 1;
        else
        {
            fileStream.read(act);
            act = std::min(act + 1, ACT_MAX);
        }

        if (version < 10)
            tagType = 0;
        else
            fileStream.read(tagType);

        if (version < 3)
            numFiles = 0;
        else
            fileStream.read(numFiles);

        files.reserve(numFiles);
        for (int i = 0; i < numFiles; i++)
        {
            std::stringstream ss;
            char c;
            do
            {
                fileStream.read(c);
                ss << c;
            } while (c != '\0');
            files.push_back(ss.str());
        }

        if (9 <= version && version <= 13)
            fileStream.skip(2 * sizeof(int32_t));

        if (version < 4)
        {
            numWalls = 1;
            numFloors = 1;
            numTags = 1;
            numShadows = 1;
        }
        else
        {
            fileStream.read(numWalls);
            if (version < 16)
                numFloors = 1;
            else
                fileStream.read(numFloors);

            numTags = (tagType == 1 || tagType == 2) ? 1 : 0;
            numShadows = 1;
        }

        if (version < 4)
        {
            numLayers = 5;
            layerStream[0] = WALL_OFFSET;
            layerStream[1] = FLOOR_OFFSET;
            layerStream[2] = ORIENT_OFFSET;
            layerStream[3] = TAG_OFFSET;
            layerStream[4] = SHADOW_OFFSET;
        }
        else
        {
            numLayers = 0;
            for (int i = 0; i < numWalls; i++)
            {
                layerStream[numLayers++] = WALL_OFFSET + i;
                layerStream[numLayers++] = ORIENT_OFFSET + i;
            }
            for (int i = 0; i < numFloors; i++)
                layerStream[numLayers++] = FLOOR_OFFSET + i;
            if (numShadows > 0)
                layerStream[numLayers++] = SHADOW_OFFSET;
            if (numTags > 0)
                layerStream[numLayers++] = TAG_OFFSET;
        }

        floorLine = width * numFloors;
        floorLen = floorLine * height;
        std::vector<int> floorOffset(numFloors);
        for (int i = 0; i < numFloors; i++)
            floorOffset[i] = i;

        shadowLine = width * numShadows;
        shadowLen = shadowLine * height;
        std::vector<int> shadowOffset(numShadows);
        for (int i = 0; i < numShadows; i++)
            shadowOffset[i] = i;

        tagLine = width * numTags;
        tagLen = tagLine * height;
        std::vector<int> tagOffset(numTags);
        for (int i = 0; i < numTags; i++)
            tagOffset[i] = i;

        wallLine = width * numWalls;
        wallLen = wallLine * height;
        std::vector<int> wallOffset(numWalls);
        std::vector<int> orientationOffset(numWalls);
        for (int i = 0; i < numWalls; i++)
            wallOffset[i] = orientationOffset[i] = i;

        for (int l = 0; l < numLayers; l++)
        {
            int layer = layerStream[l];
            if (layer >= MAX_LAYERS)
                assert(false); // Unknown layer
            else if (layer >= TAG_OFFSET)
            {
                layer -= TAG_OFFSET;
                tagOffset[layer] = readTags(fileStream, tagOffset[layer]);
            }
            else if (layer >= SHADOW_OFFSET)
            {
                layer -= SHADOW_OFFSET;
                shadowOffset[layer] = readShadows(fileStream, shadowOffset[layer]);
            }
            else if (layer >= FLOOR_OFFSET)
            {
                layer -= FLOOR_OFFSET;
                floorOffset[layer] = readFloors(fileStream, floorOffset[layer]);
            }
            else if (layer >= ORIENT_OFFSET)
            {
                layer -= ORIENT_OFFSET;
                orientationOffset[layer] = readOrientations(fileStream, orientationOffset[layer]);
            }
            else if (layer >= WALL_OFFSET)
            {
                layer -= WALL_OFFSET;
                wallOffset[layer] = readWalls(fileStream, wallOffset[layer]);
            }
            else
                assert(false); // Unknown layer
        }

        readObjects(fileStream);
        readGroups(fileStream);
        readPaths(fileStream);
    }

    int Decoder::readTags(LittleEndianStreamReader& fileStream, int offset)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                fileStream.read(tags[offset]);
                offset += numTags;
            }
        }

        return offset;
    }

    int Decoder::readShadows(LittleEndianStreamReader& fileStream, int offset)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                assert(shadows.count(offset) == 0);
                shadows.emplace(std::piecewise_construct,
                        std::forward_as_tuple(offset),
                        std::forward_as_tuple(fileStream, DT1::Orientation::SHADOW));
                offset += numShadows;
            }
        }

        return offset;
    }

    int Decoder::readFloors(LittleEndianStreamReader& fileStream, int offset)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                assert(floors.count(offset) == 0);
                floors.emplace(std::piecewise_construct,
                        std::forward_as_tuple(offset),
                        std::forward_as_tuple(fileStream, DT1::Orientation::FLOOR));
                offset += numFloors;
            }
        }

        return offset;
    }

    int Decoder::readOrientations(LittleEndianStreamReader& fileStream, int offset)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int32_t orientation;
                assert(walls.count(offset) != 0);
                Cell& wall = walls.at(offset);
                fileStream.read(orientation);
                wall.setOrientation(version < 7 ? ORIENTATION_TABLE[orientation] : orientation);
                offset += numWalls;

                if (DT1::Orientation(wall.orientation).isSpecial())
                    specials[wall.id] = std::pair<int, int>(x, y);
            }
        }

        return offset;
    }

    int Decoder::readWalls(LittleEndianStreamReader& fileStream, int offset)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                assert(walls.count(offset) == 0);
                walls.emplace(std::piecewise_construct,
                        std::forward_as_tuple(offset),
                        std::forward_as_tuple(fileStream, DT1::Orientation::FLOOR));
                offset += numWalls;
            }
        }

        return offset;
    }

    void Decoder::readObjects(LittleEndianStreamReader& fileStream)
    {
        if (version < 2)
            numObjects = 0;
        else
            fileStream.read(numObjects);

        for (int i = 0; i < numObjects; i++)
        {
            objects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(i),
                    std::forward_as_tuple(fileStream, version));
            // Check Object in DS1 bounds.
            assert(objects.at(i).x >= 0 && objects.at(i).x < width * DT1::Tile::SUBTILE_SIZE);
            assert(objects.at(i).y >= 0 && objects.at(i).y < height * DT1::Tile::SUBTILE_SIZE);
        }
    }

    void Decoder::readGroups(LittleEndianStreamReader& fileStream)
    {
        if (version >= 12 && (tagType == 1 || tagType == 2))
        {
            if (version >= 18)
                fileStream.skip(sizeof(int32_t));
            fileStream.read(numGroups);
            groups.reserve(numGroups);
            for (int i = 0; i < numGroups; i++)
                groups.emplace_back(fileStream, version);
        }
        else
            numGroups = 0;
    }

    void Decoder::readPaths(LittleEndianStreamReader& fileStream)
    {
        if (version >= 14 && fileStream.remaining_size() >= sizeof(numPaths))
        {
            fileStream.read(numPaths);
            paths.reserve(numPaths);
            for (int i = 0; i < numPaths; i++)
                paths.emplace_back(fileStream, objects, version);
        }
        else
            numPaths = 0;
    }
}
