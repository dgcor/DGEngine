#include <iostream>
#include <sstream>
#include "FileUtils.h"
#include "ImageContainers/DT1ImageContainer.h"
#include <physfs.h>
#include "PhysFSStream.h"
#include "StreamReader.h"
#include "ds1.h"
#include <set>

static void strReplace(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main(int argc, char* argv[])
{
    FileUtils::initPhysFS(argv[0]);

    bool mounted = FileUtils::mount("../../d2data", "", true);
    printf("Mount %s\n", mounted ? "successful" : "failed");

#if true

    auto ds1Filename = "data/global/tiles/ACT1/TOWN/TownN1.ds1";
    //auto ds1Filename = "data/global/tiles/ACT1/CATACOMB/andy3.ds1";
    auto paletteFilename = "data/global/palette/ACT1/pal.dat";

    auto ds1 = DS1::Decoder(ds1Filename);

    std::vector<std::string> dt1Filenames;
    for (const auto& filename : ds1.files)
    {
        auto newFilename = FileUtils::getFileNameWithoutExt(filename) + ".dt1";
        strReplace(newFilename, "\\", "/");
        strReplace(newFilename, "/d2/data/", "/data/");
        printf("%s\n", newFilename.c_str());
        dt1Filenames.push_back(newFilename);
    }

    std::vector<DT1ImageContainer> dt1s;
    for (const auto& filename : dt1Filenames)
        dt1s.emplace_back(filename);

    sf::PhysFSStream paletteFile(paletteFilename);
    if (paletteFile.hasError())
    {
        printf("Palette load failed\n");
        return EXIT_FAILURE;
    }
    std::vector<uint8_t> paletteData;
    paletteData.resize((size_t)paletteFile.getSize());
    paletteFile.read(paletteData.data(), paletteFile.getSize());
    LittleEndianStreamReader paletteStream(paletteData.data(), paletteData.size());

    PaletteArray palette;
    for (auto& c : palette)
    {
        paletteStream.read(c.b);
        paletteStream.read(c.g);
        paletteStream.read(c.r);
        c.a = 255;
    }

    sf::Image2 lvlImage;
    lvlImage.create(
            (DT1::Tile::WIDTH / 2) * (ds1.width + ds1.height),
            (DT1::Tile::HEIGHT / 2) * (ds1.width + ds1.height),
            sf::Color::Transparent);

    auto drawTile = [&](int32_t id, int x, int y, const std::set<int>& orientations) {
        int xPixel = (DT1::Tile::WIDTH / 2) * (x - y + (ds1.height - 1));
        int yPixel = (DT1::Tile::HEIGHT / 2) * (x + y);
        for (auto& dt1 : dt1s)
        {
            const auto& tileOptions = dt1.getTilesById(id);
            if (tileOptions.size() > 0)
            {
                const auto& tiles = dt1.getTiles();
                // TODO: Use tile.rarity and random to select...
                auto selectedTileIndex = tileOptions[0];
                const auto& tile = tiles[selectedTileIndex];
                if (orientations.count(tile.orientation.rawValue()) != 0)
                {
                    // yPixel += tile.roofHeight;
                    auto info = DT1ImageContainer::ImageInfo();
                    auto tileImage = dt1.get(selectedTileIndex, &palette, info);
                    lvlImage.copy(tileImage, xPixel, yPixel, sf::IntRect(0, 0, 0, 0), true);
                    break;
                }
            }
        }
    };

    auto drawTiles = [&](std::map<int, DS1::Cell> cells, int increment,
            int offset, const std::set<int>& orientations) {
        int index = offset;
        for (int y = 0; y < ds1.height; y++)
        {
            for (int x = 0; x < ds1.width; x++)
            {
                if (cells.count(index) != 0)
                {
                    auto id = cells.at(index).id;
                    drawTile(id, x, y, orientations);
                }
                index += increment;
            }
            index += increment;
        }
    };

    // Draw lower walls
    for (int i = 0; i < ds1.numWalls; i++)
        drawTiles(ds1.walls, ds1.numWalls, i, {
                DT1::Orientation::LOWER_LEFT_WALL,
                DT1::Orientation::LOWER_RIGHT_WALL,
                DT1::Orientation::LOWER_NORTH_CORNER_WALL,
                DT1::Orientation::LOWER_SOUTH_CORNER_WALL });

    // Draw floors
    for (int i = 0; i < ds1.numFloors; i++)
        drawTiles(ds1.floors, ds1.numFloors, i, { DT1::Orientation::FLOOR });

    // Draw shadows
    // TODO: Draws shadows everywhere, must be more to it...
    //for (int i = 0; i < ds1.numShadows; i++)
    //    drawTiles(ds1.shadows, ds1.numShadows, i, { DT1::Orientation::SHADOW });

    // TODO: Draw walkable

    // Draw walls
    for (int i = 0; i < ds1.numWalls; i++)
        drawTiles(ds1.walls, ds1.numWalls, i, {
                DT1::Orientation::LEFT_WALL,
                DT1::Orientation::LEFT_NORTH_CORNER_WALL,
                DT1::Orientation::LEFT_END_WALL,
                DT1::Orientation::LEFT_WALL_DOOR,
                DT1::Orientation::RIGHT_WALL,
                DT1::Orientation::RIGHT_NORTH_CORNER_WALL,
                DT1::Orientation::RIGHT_END_WALL,
                DT1::Orientation::RIGHT_WALL_DOOR,
                DT1::Orientation::SOUTH_CORNER_WALL,
                DT1::Orientation::PILLAR,
                DT1::Orientation::TREE });

    // Draw roofs
    for (int i = 0; i < ds1.numWalls; i++)
        drawTiles(ds1.walls, ds1.numWalls, i, { DT1::Orientation::ROOF });

    // TODO: Draw objects

    // Draw special...
    // TODO: probably not right, doesn't seem to draw anything..
//    for (const auto& objectPair : ds1.objects)
//    {
//        const auto& object = objectPair.second;
//        drawTile(object.id, object.x, object.x, {
//                DT1::Orientation::SPECIAL_10,
//                DT1::Orientation::SPECIAL_11 });
//    }

    bool saved = lvlImage.saveToFile("test.jpg");
    printf("Save %s\n", saved ? "successful" : "failed");

#else
    auto dt1Filename = "data/global/tiles/ACT1/TOWN/fence.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/floor.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/objects.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/trees.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/CAVES/cave.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/Monastry/facade.dt1";

    auto container = DT1ImageContainer(dt1Filename);

    sf::PhysFSStream paletteFile("data/global/palette/ACT1/pal.dat");
    if (paletteFile.hasError())
    {
        printf("Palette load failed\n");
        return EXIT_FAILURE;
    }
    std::vector<uint8_t> paletteData;
    paletteData.resize((size_t)paletteFile.getSize());
    paletteFile.read(paletteData.data(), paletteFile.getSize());
    LittleEndianStreamReader paletteStream(paletteData.data(), paletteData.size());

    PaletteArray palette;
    for (auto& c : palette)
    {
        paletteStream.read(c.b);
        paletteStream.read(c.g);
        paletteStream.read(c.r);
        c.a = 255;
    }

    for (int i = 0; i < container.size(); i++)
    {
        auto info = DT1ImageContainer::ImageInfo();
        auto image = container.get(i, &palette, info);
        std::stringstream ss;
        ss << "test/test" << i << ".png";
        bool saved = image.saveToFile(ss.str());
        printf("Save #%d %s\n", i, saved ? "successful" : "failed");
    }
#endif

    printf("Done\n");

    FileUtils::deinitPhysFS();
    return EXIT_SUCCESS;
}
