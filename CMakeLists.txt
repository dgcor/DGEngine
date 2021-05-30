cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)

project(DGEngine)

add_subdirectory(DGEngine.core)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    add_definitions(-Wall -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "GCC version must be at least 11!")
    endif()
    if(NOT BEOS)
        add_definitions(-Wall)
    endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

option(DIABLO_FORMAT_SUPPORT "Enable Diablo 1-2 file format support" TRUE)
option(MPQ_SUPPORT "Enable physfs MPQ support" TRUE)
option(INTERNAL_STORMLIB "Use internal StormLib for MPQ support" TRUE)
option(EXTERNAL_STORMLIB "Use external StormLib for MPQ support" FALSE)
option(DYNAMIC_STORMLIB "Use external StormLib dll for MPQ support" TRUE)

if(MOVIE_SUPPORT)
    find_package(FFmpeg COMPONENTS avcodec avformat avutil swscale)
endif()
find_package(PhysFS REQUIRED)
find_package(SFML 2.5 REQUIRED system window graphics network audio)

include_directories(src)

set(SOURCE_FILES
    src/Main.cpp
    src/CmdLineUtils2.cpp
    src/CmdLineUtils2.h
    src/Dun.cpp
    src/Dun.h
    src/Game2.cpp
    src/Game2.h
    src/GameUtils2.cpp
    src/GameUtils2.h
    src/Pcx.cpp
    src/Pcx.h
    src/RegisterHooks.cpp
    src/RegisterHooks.h
    src/ShaderManager2.cpp
    src/ShaderManager2.h
    src/TileSet.cpp
    src/TileSet.h
    src/Actions/ActItem.h
    src/Actions/ActLevel.h
    src/Actions/ActLevelObject.h
    src/Actions/ActPlayer.h
    src/Actions/ActQuest.h
    src/Game/Classifier.cpp
    src/Game/Classifier.h
    src/Game/Classifiers.h
    src/Game/ColorLevelLayer.cpp
    src/Game/ColorLevelLayer.h
    src/Game/FlagsVector.h
    src/Game/fsa.h
    src/Game/GameHashes.h
    src/Game/GameProperties.cpp
    src/Game/GameProperties.h
    src/Game/Inventories.h
    src/Game/Inventory.cpp
    src/Game/Inventory.h
    src/Game/Item.cpp
    src/Game/Item.h
    src/Game/ItemClass.cpp
    src/Game/ItemClass.h
    src/Game/ItemLocation.h
    src/Game/Level.cpp
    src/Game/Level.h
    src/Game/LevelBase.cpp
    src/Game/LevelBase.h
    src/Game/LevelCell.cpp
    src/Game/LevelCell.h
    src/Game/LevelFlags.h
    src/Game/LevelLayer.h
    src/Game/LevelMap.cpp
    src/Game/LevelMap.h
    src/Game/LevelObject.cpp
    src/Game/LevelObject.h
    src/Game/LevelObjectClass.cpp
    src/Game/LevelObjectClass.h
    src/Game/LevelObjectClassDefaults.h
    src/Game/LevelSurface.cpp
    src/Game/LevelSurface.h
    src/Game/LightMap.h
    src/Game/LightSource.h
    src/Game/PathFinder.cpp
    src/Game/PathFinder.h
    src/Game/Player.cpp
    src/Game/Player.h
    src/Game/PlayerBase.cpp
    src/Game/PlayerBase.h
    src/Game/PlayerClass.cpp
    src/Game/PlayerClass.h
    src/Game/Quest.cpp
    src/Game/Quest.h
    src/Game/SimpleLevelObject.cpp
    src/Game/SimpleLevelObject.h
    src/Game/SimpleLevelObjectClass.h
    src/Game/Spell.cpp
    src/Game/Spell.h
    src/Game/stlastar.h
    src/Game/TextureLevelLayer.cpp
    src/Game/TextureLevelLayer.h
    src/Game/TilesetLevelLayer.cpp
    src/Game/TilesetLevelLayer.h
    src/Game/Save/SaveItem.cpp
    src/Game/Save/SaveItem.h
    src/Game/Save/SaveLevel.cpp
    src/Game/Save/SaveLevel.h
    src/Game/Save/SavePlayer.cpp
    src/Game/Save/SavePlayer.h
    src/Game/Save/SaveSimpleLevelObject.cpp
    src/Game/Save/SaveSimpleLevelObject.h
    src/Parser/ParseAction2.cpp
    src/Parser/ParseAction2.h
    src/Parser/ParseClassifier.cpp
    src/Parser/ParseClassifier.h
    src/Parser/ParseFile2.cpp
    src/Parser/ParseFile2.h
    src/Parser/ParseItem.cpp
    src/Parser/ParseItem.h
    src/Parser/ParseItemClass.cpp
    src/Parser/ParseItemClass.h
    src/Parser/ParseLevel.cpp
    src/Parser/ParseLevel.h
    src/Parser/ParseLevelObject.cpp
    src/Parser/ParseLevelObject.h
    src/Parser/ParseLevelObjectClass.cpp
    src/Parser/ParseLevelObjectClass.h
    src/Parser/ParsePanel2.cpp
    src/Parser/ParsePanel2.h
    src/Parser/ParsePlayer.cpp
    src/Parser/ParsePlayer.h
    src/Parser/ParsePlayerClass.cpp
    src/Parser/ParsePlayerClass.h
    src/Parser/ParsePredicate2.cpp
    src/Parser/ParsePredicate2.h
    src/Parser/ParseQuest.cpp
    src/Parser/ParseQuest.h
    src/Parser/ParseSpell.cpp
    src/Parser/ParseSpell.h
    src/Parser/ParseTexture2.cpp
    src/Parser/ParseTexture2.h
    src/Parser/ParseTexturePack2.cpp
    src/Parser/ParseTexturePack2.h
    src/Parser/Utils/ParseUtilsGameKey.cpp
    src/Parser/Utils/ParseUtilsGameKey.h
    src/Parser/Utils/ParseUtilsGameVal.cpp
    src/Parser/Utils/ParseUtilsGameVal.h
    src/Predicates/PredItem.h
    src/Predicates/PredLevelObject.h
    src/Predicates/PredPlayer.h
    src/SFML/GradientCircle.cpp
    src/SFML/GradientCircle.h
    src/TexturePacks/CachedTexturePack2.cpp
    src/TexturePacks/CachedTexturePack2.h
    src/TexturePacks/CompositeTexturePack2.cpp
    src/TexturePacks/CompositeTexturePack2.h
    src/TexturePacks/IndexedTexturePack2.cpp
    src/TexturePacks/IndexedTexturePack2.h
    src/TexturePacks/StackedTexturePack2.cpp
    src/TexturePacks/StackedTexturePack2.h
)

if(DIABLO_FORMAT_SUPPORT)
    add_definitions(-DDGENGINE_DIABLO_FORMAT_SUPPORT)

    SET(SOURCE_FILES ${SOURCE_FILES}
        src/DS1.cpp
        src/DS1.h
        src/Min.cpp
        src/Min.h
        src/Game/LevelHelper.cpp
        src/Game/LevelHelper.h
        src/ImageContainers/CELImageContainer.cpp
        src/ImageContainers/CELImageContainer.h
        src/ImageContainers/CL2ImageContainer.cpp
        src/ImageContainers/CL2ImageContainer.h
        src/ImageContainers/DC6ImageContainer.cpp
        src/ImageContainers/DC6ImageContainer.h
        src/ImageContainers/DCCImageContainer.cpp
        src/ImageContainers/DCCImageContainer.h
        src/ImageContainers/DT1ImageContainer.cpp
        src/ImageContainers/DT1ImageContainer.h
        src/Parser/ParseImageContainer2.cpp
        src/Parser/ParseImageContainer2.h
    )

    if(MPQ_SUPPORT)
        add_definitions(-DPHYSFS_MPQ_SUPPORT)

        SET(SOURCE_FILES ${SOURCE_FILES}
            src/PhysFSArchiverMPQ.cpp
            src/PhysFSArchiverMPQ.h
        )

        if(EXTERNAL_STORMLIB)
            add_definitions(-DPHYSFS_EXTERNAL_STORMLIB)
        elseif(INTERNAL_STORMLIB)
            add_definitions(-DPHYSFS_INTERNAL_STORMLIB)

            SET(SOURCE_FILES ${SOURCE_FILES}
                src/StormLib/FileStream.cpp
                src/StormLib/FileStream.h
                src/StormLib/SBaseCommon.cpp
                src/StormLib/SBaseFileTable.cpp
                src/StormLib/SCompression.cpp
                src/StormLib/SFileGetFileInfo.cpp
                src/StormLib/SFileOpenArchive.cpp
                src/StormLib/SFileOpenFileEx.cpp
                src/StormLib/SFileReadFile.cpp
                src/StormLib/StormCommon.h
                src/StormLib/StormLib.h
                src/StormLib/StormPort.h
                src/StormLib/pklib/explode.c
                src/StormLib/pklib/pklib.h
            )
        endif()

        if(DYNAMIC_STORMLIB)
            add_definitions(-DPHYSFS_DYNAMIC_STORMLIB)
        endif()
    endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(FFmpeg_FOUND)
    include_directories(${FFmpeg_INCLUDES})
    target_link_libraries(${PROJECT_NAME} ${FFmpeg_LIBRARIES})
endif()

include_directories(${PHYSFS_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${PHYSFS_LIBRARY} ${SFML_LIBRARIES})

target_link_libraries(${PROJECT_NAME} DGEngine.core)

if(DIABLO_FORMAT_SUPPORT AND MPQ_SUPPORT)
    if(EXTERNAL_STORMLIB)
        target_link_libraries(${PROJECT_NAME} storm)
    endif()
    if(DYNAMIC_STORMLIB AND UNIX)
        target_link_libraries(${PROJECT_NAME} dl)
    endif()
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
