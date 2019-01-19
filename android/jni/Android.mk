LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions
LOCAL_CFLAGS    := -DUSE_SFML_MOVIE_STUB

LOCAL_MODULE    := dgengine

LOCAL_SRC_FILES := Main.cpp
LOCAL_SRC_FILES += Alignment.h
LOCAL_SRC_FILES += Anchor.h
LOCAL_SRC_FILES += Animation.cpp
LOCAL_SRC_FILES += Animation.h
LOCAL_SRC_FILES += AnimationType.h
LOCAL_SRC_FILES += AudioSource.h
LOCAL_SRC_FILES += BaseAnimation.cpp
LOCAL_SRC_FILES += BaseAnimation.h
LOCAL_SRC_FILES += BitmapButton.cpp
LOCAL_SRC_FILES += BitmapButton.h
LOCAL_SRC_FILES += BitmapFont.cpp
LOCAL_SRC_FILES += BitmapFont.h
LOCAL_SRC_FILES += BitmapText.cpp
LOCAL_SRC_FILES += BitmapText.h
LOCAL_SRC_FILES += Button.cpp
LOCAL_SRC_FILES += Button.h
LOCAL_SRC_FILES += CachedImagePack.cpp
LOCAL_SRC_FILES += CachedImagePack.h
LOCAL_SRC_FILES += Circle.cpp
LOCAL_SRC_FILES += Circle.h
LOCAL_SRC_FILES += DrawableText.h
LOCAL_SRC_FILES += Dun.cpp
LOCAL_SRC_FILES += Dun.h
LOCAL_SRC_FILES += Event.cpp
LOCAL_SRC_FILES += Event.h
LOCAL_SRC_FILES += EventManager.h
LOCAL_SRC_FILES += FadeInOut.cpp
LOCAL_SRC_FILES += FadeInOut.h
LOCAL_SRC_FILES += FileUtils.cpp
LOCAL_SRC_FILES += FileUtils.h
LOCAL_SRC_FILES += Font.h
LOCAL_SRC_FILES += FreeTypeFont.h
LOCAL_SRC_FILES += Game.cpp
LOCAL_SRC_FILES += Game.h
LOCAL_SRC_FILES += GameUtils.cpp
LOCAL_SRC_FILES += GameUtils.h
LOCAL_SRC_FILES += IfCondition.cpp
LOCAL_SRC_FILES += IfCondition.h
LOCAL_SRC_FILES += IgnoreResource.h
LOCAL_SRC_FILES += Image.cpp
LOCAL_SRC_FILES += Image.h
LOCAL_SRC_FILES += ImageUtils.cpp
LOCAL_SRC_FILES += ImageUtils.h
LOCAL_SRC_FILES += InputEvent.h
LOCAL_SRC_FILES += InputText.cpp
LOCAL_SRC_FILES += InputText.h
LOCAL_SRC_FILES += LoadingScreen.cpp
LOCAL_SRC_FILES += LoadingScreen.h
LOCAL_SRC_FILES += Menu.cpp
LOCAL_SRC_FILES += Menu.h
LOCAL_SRC_FILES += Min.cpp
LOCAL_SRC_FILES += Min.h
LOCAL_SRC_FILES += Movie2.cpp
LOCAL_SRC_FILES += Movie2.h
LOCAL_SRC_FILES += Palette.cpp
LOCAL_SRC_FILES += Palette.h
LOCAL_SRC_FILES += Pcx.cpp
LOCAL_SRC_FILES += Pcx.h
LOCAL_SRC_FILES += PhysFSStream.cpp
LOCAL_SRC_FILES += PhysFSStream.h
LOCAL_SRC_FILES += Queryable.h
LOCAL_SRC_FILES += Rectangle.cpp
LOCAL_SRC_FILES += Rectangle.h
LOCAL_SRC_FILES += ResourceManager.cpp
LOCAL_SRC_FILES += ResourceManager.h
LOCAL_SRC_FILES += ScrollableText.cpp
LOCAL_SRC_FILES += ScrollableText.h
LOCAL_SRC_FILES += Shaders.cpp
LOCAL_SRC_FILES += Shaders.h
LOCAL_SRC_FILES += Sol.cpp
LOCAL_SRC_FILES += Sol.h
LOCAL_SRC_FILES += StreamReader.h
LOCAL_SRC_FILES += StringButton.cpp
LOCAL_SRC_FILES += StringButton.h
LOCAL_SRC_FILES += StringText.cpp
LOCAL_SRC_FILES += StringText.h
LOCAL_SRC_FILES += Text.cpp
LOCAL_SRC_FILES += Text.h
LOCAL_SRC_FILES += TextureInfo.h
LOCAL_SRC_FILES += TextUtils.cpp
LOCAL_SRC_FILES += TextUtils.h
LOCAL_SRC_FILES += TileSet.cpp
LOCAL_SRC_FILES += TileSet.h
LOCAL_SRC_FILES += UIObject.h
LOCAL_SRC_FILES += Variable.cpp
LOCAL_SRC_FILES += Variable.h
LOCAL_SRC_FILES += VarOrPredicate.h
LOCAL_SRC_FILES += Actions/ActAction.h
LOCAL_SRC_FILES += Actions/ActAudio.h
LOCAL_SRC_FILES += Actions/ActButton.h
LOCAL_SRC_FILES += Actions/ActCondition.h
LOCAL_SRC_FILES += Actions/ActCursor.h
LOCAL_SRC_FILES += Actions/ActDrawable.h
LOCAL_SRC_FILES += Actions/ActEvent.h
LOCAL_SRC_FILES += Actions/ActFade.h
LOCAL_SRC_FILES += Actions/ActFocus.h
LOCAL_SRC_FILES += Actions/ActFont.h
LOCAL_SRC_FILES += Actions/ActGame.h
LOCAL_SRC_FILES += Actions/ActImage.h
LOCAL_SRC_FILES += Actions/ActInputText.h
LOCAL_SRC_FILES += Actions/ActIO.h
LOCAL_SRC_FILES += Actions/Action.h
LOCAL_SRC_FILES += Actions/ActiontList.h
LOCAL_SRC_FILES += Actions/ActItem.h
LOCAL_SRC_FILES += Actions/ActLevel.h
LOCAL_SRC_FILES += Actions/ActLevelObject.h
LOCAL_SRC_FILES += Actions/ActLoad.h
LOCAL_SRC_FILES += Actions/ActLoadingScreen.h
LOCAL_SRC_FILES += Actions/ActMenu.h
LOCAL_SRC_FILES += Actions/ActMovie.h
LOCAL_SRC_FILES += Actions/ActPalette.h
LOCAL_SRC_FILES += Actions/ActPlayer.h
LOCAL_SRC_FILES += Actions/ActQuest.h
LOCAL_SRC_FILES += Actions/ActRandom.h
LOCAL_SRC_FILES += Actions/ActResource.h
LOCAL_SRC_FILES += Actions/ActSound.h
LOCAL_SRC_FILES += Actions/ActText.h
LOCAL_SRC_FILES += Actions/ActVariable.h
LOCAL_SRC_FILES += Actions/ActVisibility.h
LOCAL_SRC_FILES += endian/big_endian.hpp
LOCAL_SRC_FILES += endian/is_big_endian.hpp
LOCAL_SRC_FILES += endian/little_endian.hpp
LOCAL_SRC_FILES += endian/stream_reader.hpp
LOCAL_SRC_FILES += endian/stream_writer.hpp
LOCAL_SRC_FILES += endian/detail/helpers.hpp
LOCAL_SRC_FILES += endian/detail/stream.hpp
LOCAL_SRC_FILES += Game/Classifier.cpp
LOCAL_SRC_FILES += Game/Classifier.h
LOCAL_SRC_FILES += Game/Classifiers.h
LOCAL_SRC_FILES += Game/Formula.cpp
LOCAL_SRC_FILES += Game/Formula.h
LOCAL_SRC_FILES += Game/Formulas.h
LOCAL_SRC_FILES += Game/fsa.h
LOCAL_SRC_FILES += Game/GameHashes.h
LOCAL_SRC_FILES += Game/GameProperties.cpp
LOCAL_SRC_FILES += Game/GameProperties.h
LOCAL_SRC_FILES += Game/Inventories.h
LOCAL_SRC_FILES += Game/Inventory.cpp
LOCAL_SRC_FILES += Game/Inventory.h
LOCAL_SRC_FILES += Game/Item.cpp
LOCAL_SRC_FILES += Game/Item.h
LOCAL_SRC_FILES += Game/ItemClass.cpp
LOCAL_SRC_FILES += Game/ItemClass.h
LOCAL_SRC_FILES += Game/ItemLocation.h
LOCAL_SRC_FILES += Game/ItemXY.h
LOCAL_SRC_FILES += Game/Level.cpp
LOCAL_SRC_FILES += Game/Level.h
LOCAL_SRC_FILES += Game/LevelCell.cpp
LOCAL_SRC_FILES += Game/LevelCell.h
LOCAL_SRC_FILES += Game/LevelHelper.cpp
LOCAL_SRC_FILES += Game/LevelHelper.h
LOCAL_SRC_FILES += Game/LevelMap.cpp
LOCAL_SRC_FILES += Game/LevelMap.h
LOCAL_SRC_FILES += Game/LevelObject.cpp
LOCAL_SRC_FILES += Game/LevelObject.h
LOCAL_SRC_FILES += Game/LevelObjectClass.cpp
LOCAL_SRC_FILES += Game/LevelObjectClass.h
LOCAL_SRC_FILES += Game/LevelObjectClassDefaults.h
LOCAL_SRC_FILES += Game/LightMap.h
LOCAL_SRC_FILES += Game/LightSource.h
LOCAL_SRC_FILES += Game/MapCoord.h
LOCAL_SRC_FILES += Game/Number.h
LOCAL_SRC_FILES += Game/PairXY.h
LOCAL_SRC_FILES += Game/PathFinder.cpp
LOCAL_SRC_FILES += Game/PathFinder.h
LOCAL_SRC_FILES += Game/Player.cpp
LOCAL_SRC_FILES += Game/Player.h
LOCAL_SRC_FILES += Game/PlayerClass.cpp
LOCAL_SRC_FILES += Game/PlayerClass.h
LOCAL_SRC_FILES += Game/Quest.cpp
LOCAL_SRC_FILES += Game/Quest.h
LOCAL_SRC_FILES += Game/SimpleLevelObject.cpp
LOCAL_SRC_FILES += Game/SimpleLevelObject.h
LOCAL_SRC_FILES += Game/SimpleLevelObjectClass.h
LOCAL_SRC_FILES += Game/Spell.cpp
LOCAL_SRC_FILES += Game/Spell.h
LOCAL_SRC_FILES += Game/stlastar.h
LOCAL_SRC_FILES += Game/Save/SaveItem.cpp
LOCAL_SRC_FILES += Game/Save/SaveItem.h
LOCAL_SRC_FILES += Game/Save/SaveLevel.cpp
LOCAL_SRC_FILES += Game/Save/SaveLevel.h
LOCAL_SRC_FILES += Game/Save/SavePlayer.cpp
LOCAL_SRC_FILES += Game/Save/SavePlayer.h
LOCAL_SRC_FILES += Game/Save/SaveProperties.h
LOCAL_SRC_FILES += Game/Save/SaveSimpleLevelObject.cpp
LOCAL_SRC_FILES += Game/Save/SaveSimpleLevelObject.h
LOCAL_SRC_FILES += Game/Save/SaveUtils.cpp
LOCAL_SRC_FILES += Game/Save/SaveUtils.h
LOCAL_SRC_FILES += gsl/gsl
LOCAL_SRC_FILES += gsl/gsl_algorithm
LOCAL_SRC_FILES += gsl/gsl_assert
LOCAL_SRC_FILES += gsl/gsl_byte
LOCAL_SRC_FILES += gsl/gsl_util
LOCAL_SRC_FILES += gsl/multi_span
LOCAL_SRC_FILES += gsl/pointers
LOCAL_SRC_FILES += gsl/span
LOCAL_SRC_FILES += gsl/string_span
LOCAL_SRC_FILES += ImageContainers/CelBaseImageContainer.cpp
LOCAL_SRC_FILES += ImageContainers/CelBaseImageContainer.h
LOCAL_SRC_FILES += ImageContainers/CelImageContainer.cpp
LOCAL_SRC_FILES += ImageContainers/CelImageContainer.h
LOCAL_SRC_FILES += ImageContainers/Cl2ImageContainer.cpp
LOCAL_SRC_FILES += ImageContainers/Cl2ImageContainer.h
LOCAL_SRC_FILES += ImageContainers/ImageContainer.h
LOCAL_SRC_FILES += ImageContainers/SimpleImageContainer.cpp
LOCAL_SRC_FILES += ImageContainers/SimpleImageContainer.h
LOCAL_SRC_FILES += Json/JsonParser.h
LOCAL_SRC_FILES += Json/JsonUtils.cpp
LOCAL_SRC_FILES += Json/JsonUtils.h
LOCAL_SRC_FILES += Parser/ParseAction.cpp
LOCAL_SRC_FILES += Parser/ParseAction.h
LOCAL_SRC_FILES += Parser/ParseAnimation.cpp
LOCAL_SRC_FILES += Parser/ParseAnimation.h
LOCAL_SRC_FILES += Parser/ParseAudio.cpp
LOCAL_SRC_FILES += Parser/ParseAudio.h
LOCAL_SRC_FILES += Parser/ParseAudioCommon.cpp
LOCAL_SRC_FILES += Parser/ParseAudioCommon.h
LOCAL_SRC_FILES += Parser/ParseButton.cpp
LOCAL_SRC_FILES += Parser/ParseButton.h
LOCAL_SRC_FILES += Parser/ParseCircle.cpp
LOCAL_SRC_FILES += Parser/ParseCircle.h
LOCAL_SRC_FILES += Parser/ParseCondition.cpp
LOCAL_SRC_FILES += Parser/ParseCondition.h
LOCAL_SRC_FILES += Parser/ParseCursor.cpp
LOCAL_SRC_FILES += Parser/ParseCursor.h
LOCAL_SRC_FILES += Parser/ParseEvent.cpp
LOCAL_SRC_FILES += Parser/ParseEvent.h
LOCAL_SRC_FILES += Parser/ParseFile.cpp
LOCAL_SRC_FILES += Parser/ParseFile.h
LOCAL_SRC_FILES += Parser/ParseFont.cpp
LOCAL_SRC_FILES += Parser/ParseFont.h
LOCAL_SRC_FILES += Parser/ParseIcon.cpp
LOCAL_SRC_FILES += Parser/ParseIcon.h
LOCAL_SRC_FILES += Parser/ParseImage.cpp
LOCAL_SRC_FILES += Parser/ParseImage.h
LOCAL_SRC_FILES += Parser/ParseImageContainer.cpp
LOCAL_SRC_FILES += Parser/ParseImageContainer.h
LOCAL_SRC_FILES += Parser/ParseInputText.cpp
LOCAL_SRC_FILES += Parser/ParseInputText.h
LOCAL_SRC_FILES += Parser/ParseKeyboard.cpp
LOCAL_SRC_FILES += Parser/ParseKeyboard.h
LOCAL_SRC_FILES += Parser/ParseLoadingScreen.cpp
LOCAL_SRC_FILES += Parser/ParseLoadingScreen.h
LOCAL_SRC_FILES += Parser/ParseMenu.cpp
LOCAL_SRC_FILES += Parser/ParseMenu.h
LOCAL_SRC_FILES += Parser/ParseMenuButton.cpp
LOCAL_SRC_FILES += Parser/ParseMenuButton.h
LOCAL_SRC_FILES += Parser/ParseMountFile.cpp
LOCAL_SRC_FILES += Parser/ParseMountFile.h
LOCAL_SRC_FILES += Parser/ParseMovie.cpp
LOCAL_SRC_FILES += Parser/ParseMovie.h
LOCAL_SRC_FILES += Parser/ParsePalette.cpp
LOCAL_SRC_FILES += Parser/ParsePalette.h
LOCAL_SRC_FILES += Parser/ParsePredicate.cpp
LOCAL_SRC_FILES += Parser/ParsePredicate.h
LOCAL_SRC_FILES += Parser/Parser.cpp
LOCAL_SRC_FILES += Parser/Parser.h
LOCAL_SRC_FILES += Parser/ParseRectangle.cpp
LOCAL_SRC_FILES += Parser/ParseRectangle.h
LOCAL_SRC_FILES += Parser/ParserProperties.h
LOCAL_SRC_FILES += Parser/ParseScrollableText.cpp
LOCAL_SRC_FILES += Parser/ParseScrollableText.h
LOCAL_SRC_FILES += Parser/ParseSound.cpp
LOCAL_SRC_FILES += Parser/ParseSound.h
LOCAL_SRC_FILES += Parser/ParseText.cpp
LOCAL_SRC_FILES += Parser/ParseText.h
LOCAL_SRC_FILES += Parser/ParseTexture.cpp
LOCAL_SRC_FILES += Parser/ParseTexture.h
LOCAL_SRC_FILES += Parser/ParseTexturePack.cpp
LOCAL_SRC_FILES += Parser/ParseTexturePack.h
LOCAL_SRC_FILES += Parser/ParseVariable.cpp
LOCAL_SRC_FILES += Parser/ParseVariable.h
LOCAL_SRC_FILES += Parser/Game/ParseClassifier.cpp
LOCAL_SRC_FILES += Parser/Game/ParseClassifier.h
LOCAL_SRC_FILES += Parser/Game/ParseItem.cpp
LOCAL_SRC_FILES += Parser/Game/ParseItem.h
LOCAL_SRC_FILES += Parser/Game/ParseItemClass.cpp
LOCAL_SRC_FILES += Parser/Game/ParseItemClass.h
LOCAL_SRC_FILES += Parser/Game/ParseLevel.cpp
LOCAL_SRC_FILES += Parser/Game/ParseLevel.h
LOCAL_SRC_FILES += Parser/Game/ParseLevelObject.cpp
LOCAL_SRC_FILES += Parser/Game/ParseLevelObject.h
LOCAL_SRC_FILES += Parser/Game/ParseLevelObjectClass.cpp
LOCAL_SRC_FILES += Parser/Game/ParseLevelObjectClass.h
LOCAL_SRC_FILES += Parser/Game/ParsePlayer.cpp
LOCAL_SRC_FILES += Parser/Game/ParsePlayer.h
LOCAL_SRC_FILES += Parser/Game/ParsePlayerClass.cpp
LOCAL_SRC_FILES += Parser/Game/ParsePlayerClass.h
LOCAL_SRC_FILES += Parser/Game/ParseQuest.cpp
LOCAL_SRC_FILES += Parser/Game/ParseQuest.h
LOCAL_SRC_FILES += Parser/Game/ParseSpell.cpp
LOCAL_SRC_FILES += Parser/Game/ParseSpell.h
LOCAL_SRC_FILES += Parser/Utils/ParseUtils.cpp
LOCAL_SRC_FILES += Parser/Utils/ParseUtils.h
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsIdx.cpp
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsIdx.h
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsKey.cpp
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsKey.h
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsVal.cpp
LOCAL_SRC_FILES += Parser/Utils/ParseUtilsVal.h
LOCAL_SRC_FILES += Predicates/Predicate.h
LOCAL_SRC_FILES += Predicates/PredIO.h
LOCAL_SRC_FILES += Predicates/PredItem.h
LOCAL_SRC_FILES += Predicates/PredLevelObject.h
LOCAL_SRC_FILES += Predicates/PredPlayer.h
LOCAL_SRC_FILES += rapidjson/allocators.h
LOCAL_SRC_FILES += rapidjson/document.h
LOCAL_SRC_FILES += rapidjson/encodedstream.h
LOCAL_SRC_FILES += rapidjson/encodings.h
LOCAL_SRC_FILES += rapidjson/filereadstream.h
LOCAL_SRC_FILES += rapidjson/filewritestream.h
LOCAL_SRC_FILES += rapidjson/fwd.h
LOCAL_SRC_FILES += rapidjson/istreamwrapper.h
LOCAL_SRC_FILES += rapidjson/memorybuffer.h
LOCAL_SRC_FILES += rapidjson/memorystream.h
LOCAL_SRC_FILES += rapidjson/ostreamwrapper.h
LOCAL_SRC_FILES += rapidjson/pointer.h
LOCAL_SRC_FILES += rapidjson/prettywriter.h
LOCAL_SRC_FILES += rapidjson/rapidjson.h
LOCAL_SRC_FILES += rapidjson/reader.h
LOCAL_SRC_FILES += rapidjson/schema.h
LOCAL_SRC_FILES += rapidjson/stream.h
LOCAL_SRC_FILES += rapidjson/stringbuffer.h
LOCAL_SRC_FILES += rapidjson/writer.h
LOCAL_SRC_FILES += rapidjson/error/en.h
LOCAL_SRC_FILES += rapidjson/error/error.h
LOCAL_SRC_FILES += rapidjson/internal/biginteger.h
LOCAL_SRC_FILES += rapidjson/internal/diyfp.h
LOCAL_SRC_FILES += rapidjson/internal/dtoa.h
LOCAL_SRC_FILES += rapidjson/internal/ieee754.h
LOCAL_SRC_FILES += rapidjson/internal/itoa.h
LOCAL_SRC_FILES += rapidjson/internal/meta.h
LOCAL_SRC_FILES += rapidjson/internal/pow10.h
LOCAL_SRC_FILES += rapidjson/internal/regex.h
LOCAL_SRC_FILES += rapidjson/internal/stack.h
LOCAL_SRC_FILES += rapidjson/internal/strfunc.h
LOCAL_SRC_FILES += rapidjson/internal/strtod.h
LOCAL_SRC_FILES += rapidjson/internal/swap.h
LOCAL_SRC_FILES += rapidjson/msinttypes/inttypes.h
LOCAL_SRC_FILES += rapidjson/msinttypes/stdint.h
LOCAL_SRC_FILES += SFML/Image2.h
LOCAL_SRC_FILES += SFML/Music2.cpp
LOCAL_SRC_FILES += SFML/Music2.h
LOCAL_SRC_FILES += SFML/MusicLoops.cpp
LOCAL_SRC_FILES += SFML/MusicLoops.h
LOCAL_SRC_FILES += SFML/SFMLUtils.cpp
LOCAL_SRC_FILES += SFML/SFMLUtils.h
LOCAL_SRC_FILES += SFML/Sprite2.cpp
LOCAL_SRC_FILES += SFML/Sprite2.h
LOCAL_SRC_FILES += SFML/View2.cpp
LOCAL_SRC_FILES += SFML/View2.h
LOCAL_SRC_FILES += TexturePacks/BitmapFontTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/BitmapFontTexturePack.h
LOCAL_SRC_FILES += TexturePacks/CachedTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/CachedTexturePack.h
LOCAL_SRC_FILES += TexturePacks/IndexedTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/IndexedTexturePack.h
LOCAL_SRC_FILES += TexturePacks/RectTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/RectTexturePack.h
LOCAL_SRC_FILES += TexturePacks/SimpleTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/SimpleTexturePack.h
LOCAL_SRC_FILES += TexturePacks/TexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/TexturePack.h
LOCAL_SRC_FILES += TexturePacks/VectorTexturePack.cpp
LOCAL_SRC_FILES += TexturePacks/VectorTexturePack.h
LOCAL_SRC_FILES += Utils/EasingFunctions.h
LOCAL_SRC_FILES += Utils/FixedMap.h
LOCAL_SRC_FILES += Utils/Helper2D.h
LOCAL_SRC_FILES += Utils/iterator_tpl.h
LOCAL_SRC_FILES += Utils/LRUCache.h
LOCAL_SRC_FILES += Utils/NumberVector.h
LOCAL_SRC_FILES += Utils/ReverseIterable.h
LOCAL_SRC_FILES += Utils/Utils.cpp
LOCAL_SRC_FILES += Utils/Utils.h

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_SHARED_LIBRARIES += libphysfs
LOCAL_SHARED_LIBRARIES += libc++_shared
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)
$(call import-module,physfs)
