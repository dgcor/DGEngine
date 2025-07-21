cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# use ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache in ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(DGEngine.core VERSION 1.2.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules")

option(DGENGINE_BUILD_TEST_SUITE "Build Tests" FALSE)
option(DGENGINE_ENABLE_LOGGING "Enable Logging in release builds" FALSE)
option(DGENGINE_MOVIE_SUPPORT "Enable Movie support" TRUE)
option(DGENGINE_FALLBACK_TO_LOWERCASE "Enable falling back to all lowercase names if file is not found" TRUE)
option(DGENGINE_STATIC_CRT "Use static CRT library (Windows)" FALSE)

if(DGENGINE_MOVIE_SUPPORT)
    find_package(FFmpeg COMPONENTS avcodec avformat avutil swscale)
endif()

find_package(PhysFS CONFIG QUIET)
if(NOT PhysFS_FOUND)
    find_package(PhysFS MODULE REQUIRED)
    set(PHYSFS_MODULE_MODE TRUE)
endif()

find_package(SFML 2.6 COMPONENTS audio graphics CONFIG REQUIRED)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/spdlog" AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/spdlog/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog)
    set(spdlog_FOUND TRUE)
else()
    find_package(spdlog QUIET)
endif()

set(SOURCE_FILES
    src/Hooks.cpp
    src/Hooks.h
    src/Game/Action.h
    src/Game/ActionQueryable.h
    src/Game/Alignment.h
    src/Game/Anchor.h
    src/Game/AnimationFlags.h
    src/Game/AnimationInfo.h
    src/Game/AnimationType.h
    src/Game/BaseAnimation.cpp
    src/Game/BaseAnimation.h
    src/Game/BindingFlags.h
    src/Game/BlendMode.h
    src/Game/Event.cpp
    src/Game/Event.h
    src/Game/EventManager.cpp
    src/Game/EventManager.h
    src/Game/FadeInOut.cpp
    src/Game/FadeInOut.h
    src/Game/Formula.cpp
    src/Game/Formula.h
    src/Game/Formulas.h
    src/Game/Game.cpp
    src/Game/Game.h
    src/Game/GameInputEventManager.cpp
    src/Game/GameInputEventManager.h
    src/Game/GameQueryable.cpp
    src/Game/GameQueryable.h
    src/Game/IfCondition.cpp
    src/Game/IfCondition.h
    src/Game/IgnoreResource.h
    src/Game/InputEvent.cpp
    src/Game/InputEvent.h
    src/Game/Predicate.h
    src/Game/Queryable.cpp
    src/Game/Queryable.h
    src/Game/QueryObject.h
    src/Game/ResourceBundle.h
    src/Game/ResourceManager.cpp
    src/Game/ResourceManager.h
    src/Game/ShaderManager.cpp
    src/Game/ShaderManager.h
    src/Game/TextureInfo.h
    src/Game/TextureQueryable.h
    src/Game/UIObject.cpp
    src/Game/UIObject.h
    src/Game/Variable.h
    src/Game/VariableManager.cpp
    src/Game/VariableManager.h
    src/Game/VarOrPredicate.h
    src/Game/VarOrQueryObject.h
    src/Game/Actions/ActAction.h
    src/Game/Actions/ActAnimation.h
    src/Game/Actions/ActAudio.h
    src/Game/Actions/ActButton.h
    src/Game/Actions/ActCondition.h
    src/Game/Actions/ActCursor.h
    src/Game/Actions/ActDrawable.h
    src/Game/Actions/ActEvent.h
    src/Game/Actions/ActFade.h
    src/Game/Actions/ActFocus.h
    src/Game/Actions/ActFont.h
    src/Game/Actions/ActGame.h
    src/Game/Actions/ActImage.h
    src/Game/Actions/ActInputText.h
    src/Game/Actions/ActIO.h
    src/Game/Actions/ActionList.h
    src/Game/Actions/ActLoad.h
    src/Game/Actions/ActLoadingScreen.h
    src/Game/Actions/ActMenu.h
    src/Game/Actions/ActMount.h
    src/Game/Actions/ActMovie.h
    src/Game/Actions/ActPalette.h
    src/Game/Actions/ActPanel.h
    src/Game/Actions/ActRandom.h
    src/Game/Actions/ActResource.h
    src/Game/Actions/ActScrollable.h
    src/Game/Actions/ActShader.h
    src/Game/Actions/ActSound.h
    src/Game/Actions/ActText.h
    src/Game/Actions/ActVariable.h
    src/Game/Actions/ActVisibility.h
    src/Game/Drawables/Animation.cpp
    src/Game/Drawables/Animation.h
    src/Game/Drawables/BindableText.cpp
    src/Game/Drawables/BindableText.h
    src/Game/Drawables/BitmapButton.cpp
    src/Game/Drawables/BitmapButton.h
    src/Game/Drawables/BitmapText.cpp
    src/Game/Drawables/BitmapText.h
    src/Game/Drawables/Button.cpp
    src/Game/Drawables/Button.h
    src/Game/Drawables/Circle.cpp
    src/Game/Drawables/Circle.h
    src/Game/Drawables/DrawableText.h
    src/Game/Drawables/Image.cpp
    src/Game/Drawables/Image.h
    src/Game/Drawables/InputText.cpp
    src/Game/Drawables/InputText.h
    src/Game/Drawables/LoadingScreen.cpp
    src/Game/Drawables/LoadingScreen.h
    src/Game/Drawables/Menu.cpp
    src/Game/Drawables/Menu.h
    src/Game/Drawables/Movie2.cpp
    src/Game/Drawables/Movie2.h
    src/Game/Drawables/Panel.cpp
    src/Game/Drawables/Panel.h
    src/Game/Drawables/Rectangle.cpp
    src/Game/Drawables/Rectangle.h
    src/Game/Drawables/Scrollable.cpp
    src/Game/Drawables/Scrollable.h
    src/Game/Drawables/Shape.cpp
    src/Game/Drawables/Shape.h
    src/Game/Drawables/StringButton.cpp
    src/Game/Drawables/StringButton.h
    src/Game/Drawables/StringText.cpp
    src/Game/Drawables/StringText.h
    src/Game/Drawables/Text.cpp
    src/Game/Drawables/Text.h
    src/Game/Predicates/PredIO.h
    src/Game/Utils/CmdLineUtils.cpp
    src/Game/Utils/CmdLineUtils.h
    src/Game/Utils/FileUtils.cpp
    src/Game/Utils/FileUtils.h
    src/Game/Utils/GameUtils.cpp
    src/Game/Utils/GameUtils.h
    src/Game/Utils/ImageUtils.cpp
    src/Game/Utils/ImageUtils.h
    src/Game/Utils/TextUtils.cpp
    src/Game/Utils/TextUtils.h
    src/Game/Utils/UIObjectUtils.h
    src/Game/Utils/VarUtils.cpp
    src/Game/Utils/VarUtils.h
    src/Json/JsonParser.h
    src/Json/JsonUtils.cpp
    src/Json/JsonUtils.h
    src/Json/SaveUtils.cpp
    src/Json/SaveUtils.h
    src/Parser/ParseAction.cpp
    src/Parser/ParseAction.h
    src/Parser/ParseCommon.cpp
    src/Parser/ParseCommon.h
    src/Parser/ParseCursor.cpp
    src/Parser/ParseCursor.h
    src/Parser/ParseEvent.cpp
    src/Parser/ParseEvent.h
    src/Parser/ParseFile.cpp
    src/Parser/ParseFile.h
    src/Parser/ParseGameInputEvent.cpp
    src/Parser/ParseGameInputEvent.h
    src/Parser/ParseInputEvent.cpp
    src/Parser/ParseInputEvent.h
    src/Parser/ParseMountFile.cpp
    src/Parser/ParseMountFile.h
    src/Parser/ParsePredicate.cpp
    src/Parser/ParsePredicate.h
    src/Parser/Parser.cpp
    src/Parser/Parser.h
    src/Parser/ParserProperties.h
    src/Parser/ParseVariable.cpp
    src/Parser/ParseVariable.h
    src/Parser/Actions/ParseActionActions.cpp
    src/Parser/Actions/ParseActionActions.h
    src/Parser/Actions/ParseAnimationActions.cpp
    src/Parser/Actions/ParseAnimationActions.h
    src/Parser/Actions/ParseAudioActions.cpp
    src/Parser/Actions/ParseAudioActions.h
    src/Parser/Actions/ParseButtonActions.cpp
    src/Parser/Actions/ParseButtonActions.h
    src/Parser/Actions/ParseConditionActions.cpp
    src/Parser/Actions/ParseConditionActions.h
    src/Parser/Actions/ParseCursorActions.cpp
    src/Parser/Actions/ParseCursorActions.h
    src/Parser/Actions/ParseDrawableActions.cpp
    src/Parser/Actions/ParseDrawableActions.h
    src/Parser/Actions/ParseEventActions.cpp
    src/Parser/Actions/ParseEventActions.h
    src/Parser/Actions/ParseFadeActions.cpp
    src/Parser/Actions/ParseFadeActions.h
    src/Parser/Actions/ParseFocusActions.cpp
    src/Parser/Actions/ParseFocusActions.h
    src/Parser/Actions/ParseFontActions.cpp
    src/Parser/Actions/ParseFontActions.h
    src/Parser/Actions/ParseGameActions.cpp
    src/Parser/Actions/ParseGameActions.h
    src/Parser/Actions/ParseImageActions.cpp
    src/Parser/Actions/ParseImageActions.h
    src/Parser/Actions/ParseInputTextActions.cpp
    src/Parser/Actions/ParseInputTextActions.h
    src/Parser/Actions/ParseIOActions.cpp
    src/Parser/Actions/ParseIOActions.h
    src/Parser/Actions/ParseLoadActions.cpp
    src/Parser/Actions/ParseLoadActions.h
    src/Parser/Actions/ParseLoadingScreenActions.cpp
    src/Parser/Actions/ParseLoadingScreenActions.h
    src/Parser/Actions/ParseMenuActions.cpp
    src/Parser/Actions/ParseMenuActions.h
    src/Parser/Actions/ParseMountActions.cpp
    src/Parser/Actions/ParseMountActions.h
    src/Parser/Actions/ParseMovieActions.cpp
    src/Parser/Actions/ParseMovieActions.h
    src/Parser/Actions/ParsePaletteActions.cpp
    src/Parser/Actions/ParsePaletteActions.h
    src/Parser/Actions/ParsePanelActions.cpp
    src/Parser/Actions/ParsePanelActions.h
    src/Parser/Actions/ParseRandomActions.cpp
    src/Parser/Actions/ParseRandomActions.h
    src/Parser/Actions/ParseResourceActions.cpp
    src/Parser/Actions/ParseResourceActions.h
    src/Parser/Actions/ParseScrollableActions.cpp
    src/Parser/Actions/ParseScrollableActions.h
    src/Parser/Actions/ParseShaderActions.cpp
    src/Parser/Actions/ParseShaderActions.h
    src/Parser/Actions/ParseSoundActions.cpp
    src/Parser/Actions/ParseSoundActions.h
    src/Parser/Actions/ParseTextActions.cpp
    src/Parser/Actions/ParseTextActions.h
    src/Parser/Actions/ParseVariableActions.cpp
    src/Parser/Actions/ParseVariableActions.h
    src/Parser/Actions/ParseVisibilityActions.cpp
    src/Parser/Actions/ParseVisibilityActions.h
    src/Parser/Drawables/ParseAnimation.cpp
    src/Parser/Drawables/ParseAnimation.h
    src/Parser/Drawables/ParseButton.cpp
    src/Parser/Drawables/ParseButton.h
    src/Parser/Drawables/ParseCircle.cpp
    src/Parser/Drawables/ParseCircle.h
    src/Parser/Drawables/ParseDrawable.cpp
    src/Parser/Drawables/ParseDrawable.h
    src/Parser/Drawables/ParseImage.cpp
    src/Parser/Drawables/ParseImage.h
    src/Parser/Drawables/ParseInputText.cpp
    src/Parser/Drawables/ParseInputText.h
    src/Parser/Drawables/ParseLoadingScreen.cpp
    src/Parser/Drawables/ParseLoadingScreen.h
    src/Parser/Drawables/ParseMenu.cpp
    src/Parser/Drawables/ParseMenu.h
    src/Parser/Drawables/ParseMenuItem.cpp
    src/Parser/Drawables/ParseMenuItem.h
    src/Parser/Drawables/ParseMovie.cpp
    src/Parser/Drawables/ParseMovie.h
    src/Parser/Drawables/ParsePanel.cpp
    src/Parser/Drawables/ParsePanel.h
    src/Parser/Drawables/ParseRectangle.cpp
    src/Parser/Drawables/ParseRectangle.h
    src/Parser/Drawables/ParseScrollable.cpp
    src/Parser/Drawables/ParseScrollable.h
    src/Parser/Drawables/ParseShape.cpp
    src/Parser/Drawables/ParseShape.h
    src/Parser/Drawables/ParseText.cpp
    src/Parser/Drawables/ParseText.h
    src/Parser/Game/ParseGame.cpp
    src/Parser/Game/ParseGame.h
    src/Parser/Game/ParseIcon.cpp
    src/Parser/Game/ParseIcon.h
    src/Parser/Game/ParseSaveDir.cpp
    src/Parser/Game/ParseSaveDir.h
    src/Parser/Resources/ParseAudio.cpp
    src/Parser/Resources/ParseAudio.h
    src/Parser/Resources/ParseAudioCommon.cpp
    src/Parser/Resources/ParseAudioCommon.h
    src/Parser/Resources/ParseFileBytes.cpp
    src/Parser/Resources/ParseFileBytes.h
    src/Parser/Resources/ParseFont.cpp
    src/Parser/Resources/ParseFont.h
    src/Parser/Resources/ParseImageContainer.cpp
    src/Parser/Resources/ParseImageContainer.h
    src/Parser/Resources/ParsePalette.cpp
    src/Parser/Resources/ParsePalette.h
    src/Parser/Resources/ParseResource.h
    src/Parser/Resources/ParseShader.cpp
    src/Parser/Resources/ParseShader.h
    src/Parser/Resources/ParseSound.cpp
    src/Parser/Resources/ParseSound.h
    src/Parser/Resources/ParseTexture.cpp
    src/Parser/Resources/ParseTexture.h
    src/Parser/Resources/ParseTexturePack.cpp
    src/Parser/Resources/ParseTexturePack.h
    src/Parser/Resources/TexturePacks/ParseBitmapFontTexturePack.h
    src/Parser/Resources/TexturePacks/ParseCompositeTexturePack.h
    src/Parser/Resources/TexturePacks/ParseImageContainerTexturePack.cpp
    src/Parser/Resources/TexturePacks/ParseImageContainerTexturePack.h
    src/Parser/Resources/TexturePacks/ParseMultiTextureTexturePack.h
    src/Parser/Resources/TexturePacks/ParseSingleTextureTexturePack.cpp
    src/Parser/Resources/TexturePacks/ParseSingleTextureTexturePack.h
    src/Parser/Resources/TexturePacks/ParseStackedTexturePack.h
    src/Parser/Resources/TexturePacks/ParseTexturePackAnimatedTextures.h
    src/Parser/Resources/TexturePacks/ParseTexturePackGroups.cpp
    src/Parser/Resources/TexturePacks/ParseTexturePackGroups.h
    src/Parser/Resources/TexturePacks/ParseTexturePackIndexes.h
    src/Parser/Resources/TexturePacks/ParseTexturePackRects.h
    src/Parser/Resources/TexturePacks/ParseTexturePacks.cpp
    src/Parser/Resources/TexturePacks/ParseTexturePacks.h
    src/Parser/Utils/ParseUtils.cpp
    src/Parser/Utils/ParseUtils.h
    src/Parser/Utils/ParseUtilsKey.cpp
    src/Parser/Utils/ParseUtilsKey.h
    src/Parser/Utils/ParseUtilsVal.cpp
    src/Parser/Utils/ParseUtilsVal.h
    src/Resources/AudioSource.h
    src/Resources/BitmapFont.cpp
    src/Resources/BitmapFont.h
    src/Resources/CachedImagePack.cpp
    src/Resources/CachedImagePack.h
    src/Resources/FileBytes.h
    src/Resources/Font.h
    src/Resources/FreeTypeFont.h
    src/Resources/ImageContainer.h
    src/Resources/Palette.cpp
    src/Resources/Palette.h
    src/Resources/Shader.cpp
    src/Resources/Shader.h
    src/Resources/TexturePack.cpp
    src/Resources/TexturePack.h
    src/Resources/ImageContainers/LazyImageContainer.cpp
    src/Resources/ImageContainers/LazyImageContainer.h
    src/Resources/ImageContainers/SimpleImageContainer.cpp
    src/Resources/ImageContainers/SimpleImageContainer.h
    src/Resources/TexturePacks/AnimatedTexturePack.cpp
    src/Resources/TexturePacks/AnimatedTexturePack.h
    src/Resources/TexturePacks/AnimationGroup.cpp
    src/Resources/TexturePacks/AnimationGroup.h
    src/Resources/TexturePacks/BitmapFontTexturePack.cpp
    src/Resources/TexturePacks/BitmapFontTexturePack.h
    src/Resources/TexturePacks/CompositeTexturePack.cpp
    src/Resources/TexturePacks/CompositeTexturePack.h
    src/Resources/TexturePacks/GroupTexturePack.cpp
    src/Resources/TexturePacks/GroupTexturePack.h
    src/Resources/TexturePacks/ImageContainerTexturePack.cpp
    src/Resources/TexturePacks/ImageContainerTexturePack.h
    src/Resources/TexturePacks/IndexedTexturePack.cpp
    src/Resources/TexturePacks/IndexedTexturePack.h
    src/Resources/TexturePacks/MultiTexturePack.cpp
    src/Resources/TexturePacks/MultiTexturePack.h
    src/Resources/TexturePacks/RectTexturePack.cpp
    src/Resources/TexturePacks/RectTexturePack.h
    src/Resources/TexturePacks/StackedTexturePack.cpp
    src/Resources/TexturePacks/StackedTexturePack.h
    src/Resources/TexturePacks/TextureGroup.cpp
    src/Resources/TexturePacks/TextureGroup.h
    src/Resources/TexturePacks/TexturePackIndexWrapper.cpp
    src/Resources/TexturePacks/TexturePackIndexWrapper.h
    src/Resources/TexturePacks/TexturePackUtils.cpp
    src/Resources/TexturePacks/TexturePackUtils.h
    src/Resources/TexturePacks/TexturePackWrapper.h
    src/SFML/CompositeSprite.cpp
    src/SFML/CompositeSprite.h
    src/SFML/dr_wav.h
    src/SFML/Image2.h
    src/SFML/Music2.cpp
    src/SFML/Music2.h
    src/SFML/MusicLoops.cpp
    src/SFML/MusicLoops.h
    src/SFML/PhysFSStream.cpp
    src/SFML/PhysFSStream.h
    src/SFML/SFMLUtils.cpp
    src/SFML/SFMLUtils.h
    src/SFML/Sprite2.cpp
    src/SFML/Sprite2.h
    src/SFML/Surface.cpp
    src/SFML/Surface.h
    src/SFML/Text2.cpp
    src/SFML/Text2.h
    src/SFML/VertexArray2.cpp
    src/SFML/VertexArray2.h
    src/SFML/VertexShape.cpp
    src/SFML/VertexShape.h
    src/SFML/View2.cpp
    src/SFML/View2.h
    src/SFML/Wave2.cpp
    src/SFML/Wave2.h
    src/Utils/BinarySearch.h
    src/Utils/EasedValue.h
    src/Utils/EasingFunctions.h
    src/Utils/ElapsedTime.h
    src/Utils/FixedArray.h
    src/Utils/FixedMap.h
    src/Utils/Helper2D.h
    src/Utils/LoadLibrary.cpp
    src/Utils/LoadLibrary.h
    src/Utils/Log.h
    src/Utils/LRUCache.h
    src/Utils/Number.h
    src/Utils/NumberVector.h
    src/Utils/PairXY.h
    src/Utils/re.c
    src/Utils/re.h
    src/Utils/Random.cpp
    src/Utils/Random.h
    src/Utils/Regex.h
    src/Utils/StreamReader.h
    src/Utils/StringHash.cpp
    src/Utils/StringHash.h
    src/Utils/UnorderedStringMap.h
    src/Utils/Utils.cpp
    src/Utils/Utils.h
    src/Utils/Vector2D.h
)

if(FFmpeg_FOUND)
    SET(SOURCE_FILES ${SOURCE_FILES}
        src/SFML/sfeMovie/AudioStream.cpp
        src/SFML/sfeMovie/AudioStream.hpp
        src/SFML/sfeMovie/AVFunc.cpp
        src/SFML/sfeMovie/AVFunc.hpp
        src/SFML/sfeMovie/Demuxer.cpp
        src/SFML/sfeMovie/Demuxer.hpp
        src/SFML/sfeMovie/Movie.cpp
        src/SFML/sfeMovie/Movie.hpp
        src/SFML/sfeMovie/MovieImpl.cpp
        src/SFML/sfeMovie/MovieImpl.hpp
        src/SFML/sfeMovie/Stream.cpp
        src/SFML/sfeMovie/Stream.hpp
        src/SFML/sfeMovie/StreamSelection.hpp
        src/SFML/sfeMovie/Timer.cpp
        src/SFML/sfeMovie/Timer.hpp
        src/SFML/sfeMovie/TimerPriorities.hpp
        src/SFML/sfeMovie/VideoStream.cpp
        src/SFML/sfeMovie/VideoStream.hpp
    )
endif()

if(DGENGINE_STATIC_CRT AND WIN32)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -stdlib=libc++)
    target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.1)
        message(FATAL_ERROR "GCC version must be at least 11.1!")
    endif()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /wd4250 /wd4996)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(FFmpeg_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${FFmpeg_INCLUDES})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FFmpeg_LIBRARIES})
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC DGENGINE_MOVIE_STUB=1)
endif()

if(PHYSFS_MODULE_MODE)
    target_include_directories(${PROJECT_NAME} PUBLIC ${PHYSFS_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PHYSFS_LIBRARY})
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC $<IF:$<TARGET_EXISTS:PhysFS::PhysFS>,PhysFS::PhysFS,PhysFS::PhysFS-static>)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio sfml-graphics)

if(DGENGINE_FALLBACK_TO_LOWERCASE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DGENGINE_FALLBACK_TO_LOWERCASE=1)
endif()

if(DGENGINE_ENABLE_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DGENGINE_ENABLE_LOGGING=1)
endif()

if(spdlog_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DGENGINE_HAS_LOGGING=1)
    target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(DGENGINE_BUILD_TEST_SUITE)
    enable_testing()
    add_subdirectory(test)
endif()
