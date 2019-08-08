#pragma once

#ifndef NO_DIABLO_FORMAT_SUPPORT
#include <cstdint>
#include "ImageContainer.h"
#include "StreamReader.h"
#include <string_view>

// DT1 decoding code based on Diablo 2 Engine Riiablo by collinsmith
// https://github.com/collinsmith/riiablo
namespace DT1
{
    class Header
    {
    public:
        int32_t version1;
        int32_t version2;
        int32_t numTiles;
        int32_t tileOffset;
        Header() = default;
        Header(LittleEndianStreamReader& fileStream);
    };

    class Orientation
    {
    private:
        int32_t value;

    public:
        /** Floors */
        static constexpr int FLOOR = 0;
        /** Left Wall */
        static constexpr int LEFT_WALL = 1;
        /** Right Wall */
        static constexpr int RIGHT_WALL = 2;
        /** Right part of north corner wall */
        static constexpr int RIGHT_NORTH_CORNER_WALL = 3;
        /** Left part of north corner wall */
        static constexpr int LEFT_NORTH_CORNER_WALL = 4;
        /** Left end wall */
        static constexpr int LEFT_END_WALL = 5;
        /** Right end wall */
        static constexpr int RIGHT_END_WALL = 6;
        /** South corner wall */
        static constexpr int SOUTH_CORNER_WALL = 7;
        /** Left wall with door */
        static constexpr int LEFT_WALL_DOOR = 8;
        /** Right wall with door */
        static constexpr int RIGHT_WALL_DOOR = 9;
        /** Special Cell */
        static constexpr int SPECIAL_10 = 10;
        /** Special Cell */
        static constexpr int SPECIAL_11 = 11;
        /** Pillars; columns and standalone objects */
        static constexpr int PILLAR = 12;
        /** Shadows */
        static constexpr int SHADOW = 13;
        /** Trees */
        static constexpr int TREE = 14;
        /** Roofs */
        static constexpr int ROOF = 15;
        /** Lower walls equivalent to Orientation 1
        *  @see #LEFT_WALL */
        static constexpr int LOWER_LEFT_WALL = 16;
        /** Lower walls equivalent to Orientation 2
        *  @see #RIGHT_WALL */
        static constexpr int LOWER_RIGHT_WALL = 17;
        /** Lower walls equivalent to Orientation 3 and 4
        *  @see #RIGHT_NORTH_CORNER_WALL
        *  @see #LEFT_NORTH_CORNER_WALL */
        static constexpr int LOWER_NORTH_CORNER_WALL = 18;
        /** Lower walls equivalent to Orientation 7
        *  @see #SOUTH_CORNER_WALL */
        static constexpr int LOWER_SOUTH_CORNER_WALL = 19;

        Orientation() = default;
        Orientation(int32_t value_) { value = value_; }
        int32_t rawValue() const { return value; }
        int getDirection() const;
        bool isFloor() const;
        bool isRoof() const;
        bool isWall() const;
        bool isSpecial() const;;
    };

    class Block
    {
    public:
        int16_t x;
        int16_t y;
        uint8_t gridX;
        uint8_t gridY;
        int16_t format;
        int32_t length;
        int32_t fileOffset;

        std::vector<uint8_t> colormap;

        Block() = default;
        Block(LittleEndianStreamReader& fileStream);
    };

    class Tile
    {
    private:
        void drawIsometricBlock(sf::Image2& img, const PaletteArray* palette, int x0, int y0, const uint8_t* data, int length) const;
        void drawRLEBlock(sf::Image2& img, const PaletteArray* palette, int x0, int y0, const uint8_t* data, int length) const;

    public:
        static constexpr int WIDTH = 160;
        static constexpr int HEIGHT = 80;

        static constexpr int SUBTILE_SIZE = 5;
        static constexpr int NUM_SUBTILES = SUBTILE_SIZE * SUBTILE_SIZE;

        static constexpr int SUBTILE_WIDTH = WIDTH / SUBTILE_SIZE;
        static constexpr int SUBTILE_HEIGHT = HEIGHT / SUBTILE_SIZE;

        // TODO: Init these with C++ 17 constexpr if possible.
        static int SUBTILE_INDEX[SUBTILE_SIZE][SUBTILE_SIZE];
        static int SUBTILE_OFFSET[NUM_SUBTILES][2];

        static constexpr int FLAG_BLOCK_WALK = 1 << 0;
        static constexpr int FLAG_BLOCK_LIGHT_LOS = 1 << 1;
        static constexpr int FLAG_BLOCK_JUMP = 1 << 2;
        static constexpr int FLAG_BLOCK_PLAYER_WALK = 1 << 3;
        static constexpr int FLAG_BLOCK_UNKNOWN1 = 1 << 4;
        static constexpr int FLAG_BLOCK_LIGHT = 1 << 5;
        static constexpr int FLAG_BLOCK_UNKNOWN2 = 1 << 6;
        static constexpr int FLAG_BLOCK_UNKNOWN3 = 1 << 7;

        int32_t direction;
        int16_t roofHeight;
        uint8_t soundIndex;
        uint8_t animated;
        int32_t height;
        int32_t width;
        Orientation orientation;
        int32_t mainIndex;
        int32_t subIndex;
        int32_t rarity; // frame index if animated
        uint8_t unknown[4];
        uint8_t flags[NUM_SUBTILES];
        int32_t blockHeadersPointer;
        int32_t blockDataLength;
        int32_t numBlocks;
        int32_t id;

        std::vector<Block> blocks;

        Tile() = default;
        Tile(LittleEndianStreamReader& fileStream);

        sf::Image2 decode(const PaletteArray* palette) const;

        static int createIndex(int orientation, int mainIndex, int subIndex);
        static int getMainIndex(int index);
        static int getSubIndex(int index);
        static int getOrientation(int index);
    };
}

class DT1ImageContainer : public ImageContainer
{
private:
    std::vector<uint8_t> fileData;
    BlendMode blendMode{ BlendMode::Alpha };
    DT1::Header header;
    std::vector<DT1::Tile> tiles;

public:
    DT1ImageContainer(const std::string_view fileName);

    virtual BlendMode getBlendMode() const noexcept { return blendMode; }
    virtual void setBlendMode(BlendMode blendMode_) noexcept { blendMode = blendMode_; }

    virtual sf::Image2 get(uint32_t index,
        const PaletteArray* palette, ImageInfo& imgInfo) const;

    virtual uint32_t size() const noexcept { return tiles.size(); }

    // TODO: Make sure getDirections should always return 1.
    virtual uint32_t getDirections() const noexcept { return 1; }
};

#endif
