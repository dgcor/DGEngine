#include <iostream>
#include <sstream>
#include "FileUtils.h"
#include "ImageContainers/DT1ImageContainer.h"
#include <physfs.h>
#include "PhysFSStream.h"
#include "StreamReader.h"

int main(int argc, char* argv[])
{
    FileUtils::initPhysFS(argv[0]);

    bool mounted = FileUtils::mount("../../d2data", "", true);
    printf("Mount %s\n", mounted ? "successful" : "failed");

    auto dt1Filename = "data/global/tiles/ACT1/TOWN/fence.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/floor.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/objects.dt1";
    // auto dt1Filename = "data/global/tiles/ACT1/TOWN/trees.dt1";

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
        ss << "test" << i << ".png";
        bool saved = image.saveToFile(ss.str());
        printf("Save #%d %s\n", i, saved ? "successful" : "failed");
    }

    printf("Done\n");

    FileUtils::deinitPhysFS();
    return EXIT_SUCCESS;
}
