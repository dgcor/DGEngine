#pragma once

#include "Actions/Action.h"
#include "Animation.h"
#include "AudioSource.h"
#include "Font.h"
#include "Game/Level.h"
#include "IgnoreResource.h"
#include "ImageContainers/ImageContainer.h"
#include <list>
#include <memory>
#include "Palette.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "SFML/Music2.h"
#include "SFML/MusicLoops.h"
#include "ShaderManager.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "UIObject.h"
#include <unordered_map>
#include "Utils/ReverseIterable.h"
#include <vector>

class Button;

struct ResourceBundle
{
	struct CompareKeyEvent
	{
		size_t operator()(const sf::Event::KeyEvent& obj) const noexcept
		{
			return ((obj.code << 4) |
				(obj.alt << 0) |
				(obj.control << 1) |
				(obj.shift << 2) |
				(obj.system << 3));
		}
		bool operator()(const sf::Event::KeyEvent& obj1, const sf::Event::KeyEvent& obj2) const noexcept
		{
			return (obj1.code == obj2.code &&
				obj1.alt == obj2.alt &&
				obj1.control == obj2.control &&
				obj1.shift == obj2.shift &&
				obj1.system == obj2.system);
		}
	};

	ResourceBundle() noexcept {}
	ResourceBundle(const std::string& id_) :id(id_) {}

	IgnoreResource ignore{ IgnoreResource::None };
	std::string id;
	std::shared_ptr<Action> anyKeyAction;
	std::unordered_map<std::string, std::shared_ptr<Action>> actions;
	std::unordered_map<sf::Event::KeyEvent, std::shared_ptr<Action>, CompareKeyEvent, CompareKeyEvent> keyboardActions;
	std::unordered_map<std::string, Font> fonts;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
	std::unordered_map<std::string, AudioSource> audioSources;
	std::unordered_map<std::string, std::shared_ptr<sf::Music2>> songs;
	std::unordered_map<std::string, std::shared_ptr<Palette>> palettes;
	std::unordered_map<std::string, std::shared_ptr<ImageContainer>> imageContainers;
	std::unordered_map<std::string, std::shared_ptr<TexturePack>> texturePacks;

	std::unordered_map<std::string, std::shared_ptr<UIObject>> drawableIds;
	std::vector<UIObject*> drawables;
	std::vector<std::shared_ptr<Button>> focusButtons;
	size_t focusIdx{ 0 };
};

class ResourceManager
{
private:
	std::vector<ResourceBundle> resources;
	ShaderManager shaders;
	std::vector<std::shared_ptr<Image>> cursors;
	std::list<sf::Sound> playingSounds;
	Level* currentLevel{ nullptr };
	size_t currentLevelResourceIdx{ 0 };

	void clearCurrentLevel() noexcept
	{
		if (currentLevelResourceIdx + 1 > resources.size())
		{
			currentLevel = nullptr;
			currentLevelResourceIdx = 0;
		}
	}

	bool addFont(ResourceBundle& res,
		const std::string& key, const Font& obj);
	bool addTexture(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<sf::Texture>& obj);
	bool addAudioSource(ResourceBundle& res,
		const std::string& key, const AudioSource& obj);
	bool addSong(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<sf::Music2>& obj);
	bool addPalette(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<Palette>& obj);
	bool addImageContainer(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<ImageContainer>& obj);
	bool addTexturePack(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<TexturePack>& obj);

	void addDrawable(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<UIObject>& obj);

public:
	using iterator = std::vector<ResourceBundle>::iterator;
	using const_iterator = std::vector<ResourceBundle>::const_iterator;
	using reverse_iterator = std::vector<ResourceBundle>::reverse_iterator;
	using const_reverse_iterator = std::vector<ResourceBundle>::const_reverse_iterator;

	iterator begin() noexcept { return resources.begin(); }
	iterator end() noexcept { return resources.end(); }
	const_iterator begin() const noexcept { return resources.begin(); }
	const_iterator end() const noexcept { return resources.end(); }
	const_iterator cbegin() const noexcept { return resources.cbegin(); }
	const_iterator cend() const noexcept { return resources.cend(); }
	reverse_iterator rbegin() noexcept { return resources.rbegin(); }
	reverse_iterator rend() noexcept { return resources.rend(); }
	const_reverse_iterator rbegin() const noexcept { return resources.rbegin(); }
	const_reverse_iterator rend() const noexcept { return resources.rend(); }
	const_reverse_iterator crbegin() const noexcept { return resources.crbegin(); }
	const_reverse_iterator crend() const noexcept { return resources.crend(); }

	ResourceManager() noexcept : resources(1, ResourceBundle()) {}

	ShaderManager& Shaders() { return shaders; };
	const ShaderManager& Shaders() const { return shaders; };

	void setCurrentLevel(Level* level) noexcept
	{
		currentLevel = level;
		currentLevelResourceIdx = resources.size() - 1;
	}
	void addResource(const std::string& id);
	void popResource();
	void popResource(const std::string& id);
	void popAllResources(const std::string& id);
	void popAllResources(bool popBaseResources);
	void ignoreResources(const std::string& id, IgnoreResource ignore) noexcept;
	void ignoreTopResource(IgnoreResource ignore);
	bool resourceExists(const std::string_view id) const noexcept;
	void moveResourceToTop(const std::string& id);

	Image* getCursor() const;
	Level* getCurrentLevel() const noexcept { return currentLevel; }
	Level* getLevel(const std::string& id) const noexcept
	{
		if (id.empty() == true)
		{
			return currentLevel;
		}
		return getDrawable<Level>(id);
	}
	void addCursor(const std::shared_ptr<Image>& cursor_) { cursors.push_back(cursor_); }
	void popCursor(bool popAll = false);
	void popAllCursors() noexcept { cursors.clear(); }
	size_t cursorCount() const noexcept { return cursors.size(); }

	void setKeyboardAction(const sf::Event::KeyEvent& key, const std::shared_ptr<Action>& obj);
	void setAction(const std::string& key, const std::shared_ptr<Action>& obj);

	bool addFont(const std::string& key, const Font& obj,
		const std::string_view resourceId = {});
	bool addTexture(const std::string& key, const std::shared_ptr<sf::Texture>& obj,
		const std::string_view resourceId = {});
	bool addAudioSource(const std::string& key, const AudioSource& obj,
		const std::string_view resourceId = {});
	bool addSong(const std::string& key, const std::shared_ptr<sf::Music2>& obj,
		const std::string_view resourceId = {});
	bool addPalette(const std::string& key, const std::shared_ptr<Palette>& obj,
		const std::string_view resourceId = {});
	bool addImageContainer(const std::string& key, const std::shared_ptr<ImageContainer>& obj,
		const std::string_view resourceId = {});
	bool addTexturePack(const std::string& key, const std::shared_ptr<TexturePack>& obj,
		const std::string_view resourceId = {});

	void addDrawable(const std::string& key, const std::shared_ptr<UIObject>& obj,
		const std::string_view resourceId = {});

	void addPlayingSound(const sf::Sound& obj, bool unique = false);

	void clearFinishedSounds();
	void clearPlayingSounds() noexcept { playingSounds.clear(); }

	// if a sf::Keyboard::KeyCount key exists, any key will return this action.
	std::shared_ptr<Action> getKeyboardAction(const sf::Event::KeyEvent& key) const;
	std::shared_ptr<Action> getAction(const std::string& key) const;
	Font getFont(const std::string& key) const;
	std::shared_ptr<sf::Texture> getTexture(const std::string& key) const;
	AudioSource getAudioSource(const std::string& key) const;
	sf::SoundBuffer* getSoundBuffer(const std::string& key) const;
	std::shared_ptr<sf::Music2> getSong(const std::string& key) const;
	std::shared_ptr<Palette> getPalette(const std::string& key) const;
	std::shared_ptr<ImageContainer> getImageContainer(const std::string& key) const;
	std::shared_ptr<TexturePack> getTexturePack(const std::string& key) const;

	bool hasFont(const std::string& key) const;
	bool hasTexture(const std::string& key) const;
	bool hasAudioSource(const std::string& key) const;
	bool hasSong(const std::string& key, bool checkTopOnly = false) const;
	bool hasPalette(const std::string& key) const;
	bool hasImageContainer(const std::string& key) const;
	bool hasTexturePack(const std::string& key) const;

	bool hasDrawable(const std::string& key) const;

	UIObject* getDrawable(const std::string& key) const
	{
		return getDrawable<UIObject>(key);
	}

	template <class T>
	T* getDrawable(const std::string& key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(key);
			if (it != res.drawableIds.cend())
			{
				return dynamic_cast<T*>(it->second.get());
			}
		}
		return nullptr;
	}

	template <class T>
	std::shared_ptr<T> getDrawableSharedPtr(const std::string& key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(key);
			if (it != res.drawableIds.cend())
			{
				return std::dynamic_pointer_cast<T>(it->second);
			}
		}
		return nullptr;
	}

	void deleteDrawable(const std::string& id);

	void playPaused(bool restart);
	void pauseSongs();
	void stopSongs();

	void play(const std::string& key);

	void addFocused(const std::shared_ptr<Button>& obj);
	void clickFocused(Game& game, bool playSound);
	const Button* getFocused() const;
	void setFocused(const Button* obj);

	void moveFocusDown(Game& game);
	void moveFocusUp(Game& game);
	void updateFocus(Game& game);
};
