#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "DT1ImageContainer.h"
#include <assert.h>
#include "gsl/gsl"
#include "PhysFSStream.h"
#include <sstream>

namespace DT1
{
    static bool allZeros(uint8_t* data, size_t len)
    {
        for (size_t i = 0; i < len; i++)
            if (data[i] != 0)
                return false;
        return true;
    }

    Header::Header(LittleEndianStreamReader& fileStream)
    {
        static const int HEADER_SIZE = 276;
        uint8_t zeros[260];
        fileStream.read(version1);
        fileStream.read(version2);
        fileStream.read(zeros, sizeof(zeros));
        fileStream.read(numTiles);
        fileStream.read(tileOffset);
        assert(allZeros(zeros, sizeof(zeros)));
        assert(tileOffset == HEADER_SIZE);
    }

    int Tile::SUBTILE_INDEX[SUBTILE_SIZE][SUBTILE_SIZE] = {};
    int Tile::SUBTILE_OFFSET[NUM_SUBTILES][2] = {};

    /**
     * zeros3 is not all zeros all of the time. This might correlate with unknown. I need to figure
     * out what these variables mean, I'm fairly certain zeros3 is 3 ints, or at least the second
     * 4 bytes are used somewhere.
     *
     * data\global\tiles\expansion\Town\shrine.dt1
     * data\global\tiles\expansion\Town\trees.dt1
     *
     * unknown in above is same F6 62 FF 00  (16737014)
     * but zeros3 is different:
     * 00 00 00 00 00 00 00 00 00 00 00 00
     * 00 00 00 00 6C B0 08 0B 00 00 00 00
     *
     * data\global\tiles\expansion\Town\tent.dt1
     * F3 65 FF 00  (16737779)
     * 00 00 00 00 4C 00 37 0B 00 00 00 00
     */
    Tile::Tile(LittleEndianStreamReader& fileStream)
    {
        // Note: static init not thread safe..
        // TODO: Init these with C++ 17 constexpr if possible.
        static bool staticInitDone = false;
        if (!staticInitDone)
        {
            for (int y = 0, i = 0; y < SUBTILE_SIZE; y++)
                for (int x = 0; x < SUBTILE_SIZE; x++, i++)
                    SUBTILE_INDEX[x][y] = i;

            for (int y = 0, i = 0; y < SUBTILE_SIZE; y++) {
                int px = (WIDTH / 2) - (SUBTILE_WIDTH / 2) - (y * (SUBTILE_WIDTH / 2));
                int py = HEIGHT - SUBTILE_HEIGHT - (y * (SUBTILE_HEIGHT / 2));
                for (int x = 0; x < SUBTILE_SIZE; x++, i++)
                {
                    SUBTILE_OFFSET[i][0] = px;
                    SUBTILE_OFFSET[i][1] = py;
                    px += SUBTILE_WIDTH / 2;
                    py -= SUBTILE_HEIGHT / 2;
                }
            }

            staticInitDone = true;
        }

        uint8_t zeros1[4];
        uint8_t zeros2[7];
        uint8_t zeros3[12];
        fileStream.read(direction);
        fileStream.read(roofHeight);
        fileStream.read(soundIndex);
        fileStream.read(animated);
        fileStream.read(height);
        fileStream.read(width);
        fileStream.read(zeros1, sizeof(zeros1));
        orientation = Orientation(fileStream.read<int32_t>());
        fileStream.read(mainIndex);
        fileStream.read(subIndex);
        fileStream.read(rarity);
        fileStream.read(unknown, sizeof(unknown));
        fileStream.read(flags, sizeof(flags));
        fileStream.read(zeros2, sizeof(zeros2));
        fileStream.read(blockHeadersPointer);
        fileStream.read(blockDataLength);
        fileStream.read(numBlocks);
        fileStream.read(zeros3, sizeof(zeros3));
        id = createIndex(orientation.rawValue(), mainIndex, subIndex);
        assert(allZeros(zeros1, sizeof(zeros1)));
        assert(allZeros(zeros2, sizeof(zeros2)));
        // assert(allZeros(zeros3, sizeof(zeros3)));

        yOffset = 0;
        if (height < 0)
        {
            height = -height;
            yOffset = height;
        }

        // TODO: Check if this is needed?
        //if (orientation.isSpecial())
        //    width = WIDTH;
    }

    sf::Image2 Tile::decode(const PaletteArray* palette) const
    {
        sf::Image2 img;
        img.create(width, height, sf::Color::Transparent);

        for (const auto& block : blocks)
        {
            int x0 = block.x;
            int y0 = block.y >= 0 ? block.y : yOffset + block.y;
            const uint8_t* data = block.colormap.data();
            int length = block.length;
            int format = block.format;
            if (format == 0x0001)
                drawIsometricBlock(img, palette, x0, y0, data, length);
            else
                drawRLEBlock(img, palette, x0, y0, data, length);
        }

        return img;
    }

    void Tile::drawIsometricBlock(sf::Image2& img, const PaletteArray* palette, int x0, int y0, const uint8_t* data, int length) const
    {
        static const int X_JUMP[] = { 14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14 };
        static const int PIXEL_WIDTH[] = { 4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4 };

        assert(length == 256);

        int y = 0, i = 0;
        while (length > 0)
        {
            int x = X_JUMP[y];
            int n = PIXEL_WIDTH[y];
            length -= n;
            while (n > 0) {
                img.setPixel(x0 + x, y0 + y, ImageContainer::getColor(data[i], palette));
                i++;
                x++;
                n--;
            }
            y++;
        }
    }

    void Tile::drawRLEBlock(sf::Image2& img, const PaletteArray* palette, int x0, int y0, const uint8_t* data, int length) const
    {
        int i = 0, x = 0, y = 0;
        while (length > 0)
        {
            int b1 = data[i] & 0xFF;
            int b2 = data[i + 1] & 0xFF;
            i += 2;
            length -= 2;
            if (b1 > 0 || b2 > 0)
            {
                x += b1;
                length -= b2;
                while (b2 > 0)
                {
                    img.setPixel(x0 + x, y0 + y, ImageContainer::getColor(data[i], palette));
                    i++;
                    x++;
                    b2--;
                }
            }
            else
            {
                x = 0;
                y++;
            }
        }
    }

    static const int MAIN_INDEX_OFFSET = 16;
    static const int MAIN_INDEX_BITS = 0xFF;
    static const int SUB_INDEX_OFFSET = 8;
    static const int SUB_INDEX_BITS = 0xFF;
    static const int ORIENTATION_OFFSET = 0;
    static const int ORIENTATION_BITS = 0xFF;

    int Tile::createIndex(int orientation, int mainIndex, int subIndex)
    {
        return (mainIndex & MAIN_INDEX_BITS) << MAIN_INDEX_OFFSET
             | (subIndex & SUB_INDEX_BITS) << SUB_INDEX_OFFSET
             | (orientation & ORIENTATION_BITS) << ORIENTATION_OFFSET;
    }

    int Tile::getMainIndex(int index) { return (index >> MAIN_INDEX_OFFSET) & MAIN_INDEX_BITS; }

    int Tile::getSubIndex(int index) { return (index >> SUB_INDEX_OFFSET) & SUB_INDEX_BITS; }

    int Tile::getOrientation(int index) { return (index >> ORIENTATION_OFFSET) & ORIENTATION_BITS; }

    Block::Block(LittleEndianStreamReader& fileStream)
    {
        uint8_t zeros1[2];
        uint8_t zeros2[2];
        fileStream.read(x);
        fileStream.read(y);
        fileStream.read(zeros1, sizeof(zeros1));
        fileStream.read(gridX);
        fileStream.read(gridY);
        fileStream.read(format);
        fileStream.read(length);
        fileStream.read(zeros2, sizeof(zeros2));
        fileStream.read(fileOffset);
        assert(allZeros(zeros1, sizeof(zeros1)));
        assert(allZeros(zeros2, sizeof(zeros2)));
    }

    int Orientation::getDirection() const
    {
        switch (value)
        {
            case LEFT_WALL:
            case LEFT_END_WALL:
            case LEFT_WALL_DOOR:
                return 1;
            case RIGHT_WALL:
            case RIGHT_END_WALL:
            case RIGHT_WALL_DOOR:
                return 2;
            case FLOOR:
            case RIGHT_NORTH_CORNER_WALL:
            case LEFT_NORTH_CORNER_WALL:
            case PILLAR:
            case TREE:
                return 3;
            case SOUTH_CORNER_WALL:
                return 4;
            case ROOF:
                return 5;
            case LOWER_LEFT_WALL:
                return 6;
            case LOWER_RIGHT_WALL:
                return 7;
            case LOWER_NORTH_CORNER_WALL:
                return 8;
            case LOWER_SOUTH_CORNER_WALL:
                return 9;
            default:
                assert(false);
                return 0;
        }
    }

    bool Orientation::isFloor() const
    {
        switch (value)
        {
            case FLOOR:
                return true;
            default:
                return false;
        }
    }

    bool Orientation::isRoof() const
    {
        switch (value)
        {
            case ROOF:
                return true;
            default:
                return false;
        }
    }

    bool Orientation::isWall() const
    {
        switch (value)
        {
            case LEFT_WALL:
            case RIGHT_WALL:
            case RIGHT_NORTH_CORNER_WALL:
            case LEFT_NORTH_CORNER_WALL:
            case LEFT_END_WALL:
            case RIGHT_END_WALL:
            case SOUTH_CORNER_WALL:
            case LEFT_WALL_DOOR:
            case RIGHT_WALL_DOOR:
            case PILLAR:
            case TREE:
            case LOWER_LEFT_WALL:
            case LOWER_RIGHT_WALL:
            case LOWER_NORTH_CORNER_WALL:
            case LOWER_SOUTH_CORNER_WALL:
                return true;
            default:
                return false;
        }
    }

    bool Orientation::isSpecial() const
    {
        switch (value)
        {
            case SPECIAL_10:
            case SPECIAL_11:
                return true;
            default:
                return false;
        }
    }
}

DT1ImageContainer::DT1ImageContainer(const std::string_view fileName)
{
    std::vector<uint8_t> fileData;
    sf::PhysFSStream file(fileName.data());
    if (file.hasError())
        return;
    fileData.resize((size_t)file.getSize());
    file.read(fileData.data(), file.getSize());

    LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

    header = DT1::Header(fileStream);
    assert(header.version1 == 0x7);
    assert(header.version2 == 0x6);

    tiles.reserve(header.numTiles);
    for (int i = 0; i < header.numTiles; i++)
    {
        tiles.emplace_back(fileStream);
        tilesById[tiles[i].id].push_back(i);
    }

    for (auto& tile : tiles)
    {
        assert(tile.blockHeadersPointer == fileStream.position());
        tile.blocks.reserve(tile.numBlocks);
        for (int i = 0; i < tile.numBlocks; i++)
            tile.blocks.emplace_back(fileStream);

        for (auto& blockHeader : tile.blocks)
        {
            assert(tile.blockHeadersPointer + blockHeader.fileOffset == fileStream.position());
            blockHeader.colormap.resize(blockHeader.length);
            fileStream.read(blockHeader.colormap.data(), blockHeader.length);
        }
    }

    assert(fileStream.remaining_size() == 0);
}

sf::Image2 DT1ImageContainer::get(uint32_t index,
    const PaletteArray* palette, ImageInfo& imgInfo) const
{
    if (index >= tiles.size())
        return {};

    imgInfo.offset = {};
    imgInfo.absoluteOffset = false;
    imgInfo.blendMode = blendMode;

    return tiles[index].decode(palette);
}

#endif
