cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)

# use ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache in ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(DGEngine)

if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/../DGEngine.core")
    add_subdirectory(${PROJECT_SOURCE_DIR}/../DGEngine.core DGEngine.core)
else()
    add_subdirectory(DGEngine.core)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules")

option(DIABLO_FORMAT_SUPPORT "Enable Diablo 1-2 file format support" TRUE)
option(MPQ_SUPPORT "Enable physfs MPQ support" TRUE)
option(INTERNAL_STORMLIB "Use internal StormLib for MPQ support" TRUE)
option(EXTERNAL_STORMLIB "Use external StormLib for MPQ support" FALSE)
option(DYNAMIC_STORMLIB "Use external StormLib dll for MPQ support" TRUE)

if(MOVIE_SUPPORT)
    find_package(FFmpeg COMPONENTS avcodec avformat avutil swscale)
endif()
find_package(PhysFS REQUIRED)
find_package(SFML 2.5 COMPONENTS audio graphics REQUIRED)

include_directories(src)

set(SOURCE_FILES
    src/Main.cpp
    src/RegisterHooks.cpp
    src/RegisterHooks.h
    src/Game/Classifier.cpp
    src/Game/Classifier.h
    src/Game/Classifiers.h
    src/Game/Game2.cpp
    src/Game/Game2.h
    src/Game/GameHashes.h
    src/Game/LightMap.h
    src/Game/LightSource.h
    src/Game/ShaderManager2.cpp
    src/Game/ShaderManager2.h
    src/Game/Actions/ActItem.h
    src/Game/Actions/ActLevel.h
    src/Game/Actions/ActLevelObject.h
    src/Game/Actions/ActPlayer.h
    src/Game/Actions/ActQuest.h
    src/Game/Inventory/Inventories.h
    src/Game/Inventory/Inventory.cpp
    src/Game/Inventory/Inventory.h
    src/Game/Item/Item.cpp
    src/Game/Item/Item.h
    src/Game/Item/ItemClass.cpp
    src/Game/Item/ItemClass.h
    src/Game/Item/ItemLevelObject.cpp
    src/Game/Item/ItemLevelObject.h
    src/Game/Item/ItemLocation.h
    src/Game/Item/ItemSave.cpp
    src/Game/Item/ItemSave.h
    src/Game/Level/FlagsVector.h
    src/Game/Level/fsa.h
    src/Game/Level/Level.cpp
    src/Game/Level/Level.h
    src/Game/Level/LevelBase.cpp
    src/Game/Level/LevelBase.h
    src/Game/Level/LevelCell.cpp
    src/Game/Level/LevelCell.h
    src/Game/Level/LevelDraw.cpp
    src/Game/Level/LevelDraw.h
    src/Game/Level/LevelDrawable.h
    src/Game/Level/LevelDrawableManager.cpp
    src/Game/Level/LevelDrawableManager.h
    src/Game/Level/LevelFlags.h
    src/Game/Level/LevelInputManager.cpp
    src/Game/Level/LevelInputManager.h
    src/Game/Level/LevelItem.cpp
    src/Game/Level/LevelItem.h
    src/Game/Level/LevelLayer.h
    src/Game/Level/LevelMap.cpp
    src/Game/Level/LevelMap.h
    src/Game/Level/LevelObjectManager.cpp
    src/Game/Level/LevelObjectManager.h
    src/Game/Level/LevelQuest.cpp
    src/Game/Level/LevelQuest.h
    src/Game/Level/LevelSave.cpp
    src/Game/Level/LevelSave.h
    src/Game/Level/LevelSurface.cpp
    src/Game/Level/LevelSurface.h
    src/Game/Level/LevelUIObject.cpp
    src/Game/Level/LevelUIObject.h
    src/Game/Level/PathFinder.cpp
    src/Game/Level/PathFinder.h
    src/Game/Level/stlastar.h
    src/Game/Level/LevelLayers/ColorLevelLayer.cpp
    src/Game/Level/LevelLayers/ColorLevelLayer.h
    src/Game/Level/LevelLayers/TextureLevelLayer.cpp
    src/Game/Level/LevelLayers/TextureLevelLayer.h
    src/Game/Level/LevelLayers/TilesetLevelLayer.cpp
    src/Game/Level/LevelLayers/TilesetLevelLayer.h
    src/Game/LevelObject/LevelObject.cpp
    src/Game/LevelObject/LevelObject.h
    src/Game/LevelObject/LevelObjectClass.cpp
    src/Game/LevelObject/LevelObjectClass.h
    src/Game/LevelObject/LevelObjectClassDefaults.h
    src/Game/LevelObject/LevelObjectQueryable.cpp
    src/Game/LevelObject/LevelObjectQueryable.h
    src/Game/Player/Player.cpp
    src/Game/Player/Player.h
    src/Game/Player/PlayerBase.cpp
    src/Game/Player/PlayerBase.h
    src/Game/Player/PlayerClass.cpp
    src/Game/Player/PlayerClass.h
    src/Game/Player/PlayerInventories.cpp
    src/Game/Player/PlayerInventories.h
    src/Game/Player/PlayerLevelObject.cpp
    src/Game/Player/PlayerLevelObject.h
    src/Game/Player/PlayerMove.cpp
    src/Game/Player/PlayerMove.h
    src/Game/Player/PlayerProperties.cpp
    src/Game/Player/PlayerProperties.h
    src/Game/Player/PlayerSave.cpp
    src/Game/Player/PlayerSave.h
    src/Game/Player/PlayerSpells.cpp
    src/Game/Player/PlayerSpells.h
    src/Game/Predicates/PredItem.h
    src/Game/Predicates/PredLevelObject.h
    src/Game/Predicates/PredPlayer.h
    src/Game/Properties/AnimationSpeed.h
    src/Game/Properties/InventoryPosition.h
    src/Game/Properties/LevelObjValue.h
    src/Game/Properties/PlayerAnimation.h
    src/Game/Properties/PlayerDirection.cpp
    src/Game/Properties/PlayerDirection.h
    src/Game/Properties/PlayerInventory.h
    src/Game/Properties/PlayerItemMount.h
    src/Game/Properties/PlayerStatus.h
    src/Game/Quest/Quest.cpp
    src/Game/Quest/Quest.h
    src/Game/SimpleLevelObject/SimpleLevelObject.cpp
    src/Game/SimpleLevelObject/SimpleLevelObject.h
    src/Game/SimpleLevelObject/SimpleLevelObjectClass.h
    src/Game/SimpleLevelObject/SimpleLevelObjectLevelObject.cpp
    src/Game/SimpleLevelObject/SimpleLevelObjectLevelObject.h
    src/Game/SimpleLevelObject/SimpleLevelObjectSave.cpp
    src/Game/SimpleLevelObject/SimpleLevelObjectSave.h
    src/Game/Spell/Spell.cpp
    src/Game/Spell/Spell.h
    src/Game/Spell/SpellInstance.cpp
    src/Game/Spell/SpellInstance.h
    src/Game/Utils/CmdLineUtils2.cpp
    src/Game/Utils/CmdLineUtils2.h
    src/Game/Utils/GameUtils2.cpp
    src/Game/Utils/GameUtils2.h
    src/Parser/ParseAction2.cpp
    src/Parser/ParseAction2.h
    src/Parser/ParseFile2.cpp
    src/Parser/ParseFile2.h
    src/Parser/ParsePredicate2.cpp
    src/Parser/ParsePredicate2.h
    src/Parser/Actions/ParseItemActions.cpp
    src/Parser/Actions/ParseItemActions.h
    src/Parser/Actions/ParseLevelActions.cpp
    src/Parser/Actions/ParseLevelActions.h
    src/Parser/Actions/ParseLevelObjectActions.cpp
    src/Parser/Actions/ParseLevelObjectActions.h
    src/Parser/Actions/ParsePlayerActions.cpp
    src/Parser/Actions/ParsePlayerActions.h
    src/Parser/Actions/ParseQuestActions.cpp
    src/Parser/Actions/ParseQuestActions.h
    src/Parser/Classifier/ParseClassifier.cpp
    src/Parser/Classifier/ParseClassifier.h
    src/Parser/Drawables/ParsePanel2.cpp
    src/Parser/Drawables/ParsePanel2.h
    src/Parser/Item/ParseItem.cpp
    src/Parser/Item/ParseItem.h
    src/Parser/Item/ParseItemClass.cpp
    src/Parser/Item/ParseItemClass.h
    src/Parser/Level/ParseLevel.cpp
    src/Parser/Level/ParseLevel.h
    src/Parser/Level/ParseLevelAutoMap.cpp
    src/Parser/Level/ParseLevelAutoMap.h
    src/Parser/Level/ParseLevelLayer.cpp
    src/Parser/Level/ParseLevelLayer.h
    src/Parser/Level/ParseLevelMap.cpp
    src/Parser/Level/ParseLevelMap.h
    src/Parser/LevelObject/ParseLevelObject.cpp
    src/Parser/LevelObject/ParseLevelObject.h
    src/Parser/LevelObject/ParseLevelObjectClass.cpp
    src/Parser/LevelObject/ParseLevelObjectClass.h
    src/Parser/LevelObject/ParseLevelObjectClassCommon.cpp
    src/Parser/LevelObject/ParseLevelObjectClassCommon.h
    src/Parser/LevelObject/ParseLevelObjectCommon.cpp
    src/Parser/LevelObject/ParseLevelObjectCommon.h
    src/Parser/Player/ParsePlayer.cpp
    src/Parser/Player/ParsePlayer.h
    src/Parser/Player/ParsePlayerClass.cpp
    src/Parser/Player/ParsePlayerClass.h
    src/Parser/Player/ParsePlayerInventories.cpp
    src/Parser/Player/ParsePlayerInventories.h
    src/Parser/Player/ParsePlayerSpells.cpp
    src/Parser/Player/ParsePlayerSpells.h
    src/Parser/Quest/ParseQuest.cpp
    src/Parser/Quest/ParseQuest.h
    src/Parser/Resources/ParseTexture2.cpp
    src/Parser/Resources/ParseTexture2.h
    src/Parser/Resources/ParseTexturePack2.cpp
    src/Parser/Resources/ParseTexturePack2.h
    src/Parser/Resources/TexturePacks/ParseCompositeTexturePack2.cpp
    src/Parser/Resources/TexturePacks/ParseCompositeTexturePack2.h
    src/Parser/Resources/TexturePacks/ParseImageContainerTexturePack2.cpp
    src/Parser/Resources/TexturePacks/ParseImageContainerTexturePack2.h
    src/Parser/Spell/ParseSpell.cpp
    src/Parser/Spell/ParseSpell.h
    src/Parser/Utils/ParseUtilsGameKey.cpp
    src/Parser/Utils/ParseUtilsGameKey.h
    src/Parser/Utils/ParseUtilsGameVal.cpp
    src/Parser/Utils/ParseUtilsGameVal.h
    src/Resources/Dun.cpp
    src/Resources/Dun.h
    src/Resources/Pcx.cpp
    src/Resources/Pcx.h
    src/Resources/TexturePacks/CompositeTexturePack2.cpp
    src/Resources/TexturePacks/CompositeTexturePack2.h
    src/Resources/TexturePacks/ImageContainerTexturePack2.cpp
    src/Resources/TexturePacks/ImageContainerTexturePack2.h
    src/Resources/TexturePacks/IndexedTexturePack2.cpp
    src/Resources/TexturePacks/IndexedTexturePack2.h
    src/Resources/TexturePacks/MultiImageContainerTexturePack2.cpp
    src/Resources/TexturePacks/MultiImageContainerTexturePack2.h
    src/Resources/TexturePacks/StackedTexturePack2.cpp
    src/Resources/TexturePacks/StackedTexturePack2.h
    src/Resources/TileBlock.h
    src/Resources/TileSet.cpp
    src/Resources/TileSet.h
    src/SFML/GradientCircle.cpp
    src/SFML/GradientCircle.h
    src/Utils/iterator_tpl.h
)

if(DIABLO_FORMAT_SUPPORT)
    add_definitions(-DDGENGINE_DIABLO_FORMAT_SUPPORT)

    SET(SOURCE_FILES ${SOURCE_FILES}
        src/Game/Level/LevelHelper.cpp
        src/Game/Level/LevelHelper.h
        src/Parser/Resources/ParseImageContainer2.cpp
        src/Parser/Resources/ParseImageContainer2.h
        src/Resources/DS1.cpp
        src/Resources/DS1.h
        src/Resources/ImageContainers/CELImageContainer.cpp
        src/Resources/ImageContainers/CELImageContainer.h
        src/Resources/ImageContainers/CL2ImageContainer.cpp
        src/Resources/ImageContainers/CL2ImageContainer.h
        src/Resources/ImageContainers/DC6ImageContainer.cpp
        src/Resources/ImageContainers/DC6ImageContainer.h
        src/Resources/ImageContainers/DCCImageContainer.cpp
        src/Resources/ImageContainers/DCCImageContainer.h
        src/Resources/ImageContainers/DT1ImageContainer.cpp
        src/Resources/ImageContainers/DT1ImageContainer.h
        src/Resources/Min.cpp
        src/Resources/Min.h
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

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -stdlib=libc++)
    target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.1)
        message(FATAL_ERROR "GCC version must be at least 11.1!")
    endif()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /GF /EHsc /W3 /wd4250 /wd4996)
    target_link_options(${PROJECT_NAME} PRIVATE /OPT:ICF /OPT:REF)
endif()

if(FFmpeg_FOUND)
    include_directories(${FFmpeg_INCLUDES})
    target_link_libraries(${PROJECT_NAME} ${FFmpeg_LIBRARIES})
endif()

include_directories(${PHYSFS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PHYSFS_LIBRARY} sfml-audio sfml-graphics)

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
