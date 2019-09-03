#pragma once

#include <cstdint>
#include "ImageContainers/DT1ImageContainer.h"
#include "StreamReader.h"
#include <string_view>
#include <map>

// DS1 decoding code based on Diablo 2 Engine Riiablo by collinsmith
// https://github.com/collinsmith/riiablo
namespace DS1
{
    class Path;

    class Cell
    {
    private:
        static constexpr int MAIN_INDEX_OFFSET = 20;
        static constexpr int MAIN_INDEX_BITS = 0x3F;

        static constexpr int SUB_INDEX_OFFSET = 8;
        static constexpr int SUB_INDEX_BITS = 0xFF;

        static constexpr int MAIN_INDEX_MASK = MAIN_INDEX_BITS << MAIN_INDEX_OFFSET; // 0x03F00000
        static constexpr int SUB_INDEX_MASK = SUB_INDEX_BITS << SUB_INDEX_OFFSET;  // 0x0000FF00
        static constexpr int UNWALKABLE_MASK = 0x00020000;
        static constexpr int HIDDEN_MASK = 0x80000000;
        static constexpr int FLOOR_UNWALK_MASK = 0x000000FF;

    public:
        int32_t id;
        int32_t value;

        int16_t mainIndex;
        int16_t subIndex;
        int16_t orientation;

        Cell(LittleEndianStreamReader& fileStream, int orient)
        {
            fileStream.read(value);
            mainIndex = (int16_t) ((value >> MAIN_INDEX_OFFSET) & MAIN_INDEX_BITS);
            subIndex = (int16_t) ((value >> SUB_INDEX_OFFSET) & SUB_INDEX_BITS);
            orientation = (int16_t)orient;
            updateIndex();
        }

        int prop4() const { return (value >> 24) & 0xFF; }
        int prop3() const { return (value >> 16) & 0xFF; }
        int prop2() const { return (value >> 8) & 0xFF; }
        int prop1() const { return (value) & 0xFF; }

        void setOrientation(int orient)
        {
            orientation = (int16_t)orient;
            updateIndex();
        }

        void updateIndex() { id = DT1::Tile::createIndex(orientation, mainIndex, subIndex); }
    };

    class Object
    {
    public:
        static constexpr int DYNAMIC_TYPE = 1;
        static constexpr int STATIC_TYPE  = 2;

        int32_t type;
        int32_t id;
        int32_t x;
        int32_t y;
        int32_t flags;
        Path* path;

        Object(LittleEndianStreamReader& fileStream, int32_t version)
        {
            fileStream.read(type);
            fileStream.read(id);
            fileStream.read(x);
            fileStream.read(y);
            if (version < 6)
                flags = 0;
            else
                fileStream.read(flags);
            path = nullptr;
        }
    };

    class Group
    {
    public:
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
        int32_t unk;

        Group(LittleEndianStreamReader& fileStream, int32_t version)
        {
            if (fileStream.remaining_size() >= sizeof(x))
                fileStream.read(x);
            if (fileStream.remaining_size() >= sizeof(y))
                fileStream.read(y);
            if (fileStream.remaining_size() >= sizeof(width))
                fileStream.read(width);
            if (fileStream.remaining_size() >= sizeof(height))
               fileStream.read(height);
            if (version >= 13)
                if (fileStream.remaining_size() >= sizeof(unk))
                    fileStream.read(unk);
        }
    };


    class Path
    {
    private:
        class Point
        {
        public:
            int32_t x;
            int32_t y;
            int32_t action;

            Point(LittleEndianStreamReader& fileStream, int32_t version)
            {
                fileStream.read(x);
                fileStream.read(y);
                if (version < 15)
                    action = 1;
                else
                    fileStream.read(action);
            }
        };

    public:
        int32_t numPoints;
        std::vector<Point> points;
        int32_t x;
        int32_t y;

        Path(LittleEndianStreamReader& fileStream, std::map<int, Object>& objects, int32_t version)
        {
            fileStream.read(numPoints);
            points.reserve(numPoints);
            fileStream.read(x);
            fileStream.read(y);

            Object* object = nullptr;
            for (int i = 0; i < objects.size(); i++)
            {
                if (objects.count(i) == 0)
                    continue;
                Object& tmp = objects.at(i);
                if (tmp.x != x || tmp.y != y)
                    continue;
                // assert(object == nullptr); // More than one object is located at path position
                object = &tmp;
            }

            if (object == nullptr)
            {
                int skip = (version >= 15 ? 3 : 2) * sizeof(int32_t);
                for (int p = 0; p < numPoints; p++)
                    fileStream.skip(skip);

                return;
            }

            for (int p = 0; p < numPoints; p++)
                points.emplace_back(fileStream, version);
            object->path = this;
        }
    };

    class Decoder
    {
    private:
        static constexpr int ACT_MAX = 5;

        static constexpr int16_t ORIENTATION_TABLE[] = {
            0x00, 0x01, 0x02, 0x01, 0x02, 0x03, 0x03, 0x05, 0x05, 0x06,
            0x06, 0x07, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x10, 0x11, 0x12, 0x14
        };

        static constexpr int MAX_WALLS = 4;
        static constexpr int MAX_FLOORS = 2;
        static constexpr int MAX_SHADOWS = 1;
        static constexpr int MAX_TAGS = 1;

        static constexpr int WALL_OFFSET = 0;
        static constexpr int ORIENT_OFFSET = WALL_OFFSET + MAX_WALLS;
        static constexpr int FLOOR_OFFSET = ORIENT_OFFSET + MAX_WALLS;
        static constexpr int SHADOW_OFFSET = FLOOR_OFFSET + MAX_FLOORS;
        static constexpr int TAG_OFFSET = SHADOW_OFFSET + MAX_SHADOWS;
        static constexpr int MAX_LAYERS = TAG_OFFSET + MAX_TAGS;

        void decode(LittleEndianStreamReader& fileStream);
        int readTags(LittleEndianStreamReader& fileStream, int offset);
        int readShadows(LittleEndianStreamReader& fileStream, int offset);
        int readFloors(LittleEndianStreamReader& fileStream, int offset);
        int readOrientations(LittleEndianStreamReader& fileStream, int offset);
        int readWalls(LittleEndianStreamReader& fileStream, int offset);
        void readObjects(LittleEndianStreamReader& fileStream);
        void readGroups(LittleEndianStreamReader& fileStream);
        void readPaths(LittleEndianStreamReader& fileStream);

    public:
        int32_t version;
        int32_t width;
        int32_t height;
        int32_t act;
        int32_t tagType;

        int32_t numFiles;
        std::vector<std::string> files;

        int32_t numWalls;
        int32_t numFloors;
        int32_t numTags;
        int32_t numShadows;

        int32_t numLayers;
        int32_t layerStream[MAX_LAYERS];

        int32_t wallLine, wallLen;
        std::map<int, Cell> walls;

        int32_t floorLine, floorLen;
        std::map<int, Cell> floors;

        int32_t shadowLine, shadowLen;
        std::map<int, Cell> shadows;

        int32_t tagLine, tagLen;
        std::map<int, int32_t> tags;

        int32_t numObjects;
        std::map<int, Object> objects;

        int32_t numGroups;
        std::vector<Group> groups;

        int32_t numPaths;
        std::vector<Path> paths;

        std::map<int, std::pair<int, int>> specials;

        std::pair<int, int> find(int id) { return specials.at(id); }

        Decoder(const std::string_view fileName);
        int getAct() const { return act; }
    };
}
