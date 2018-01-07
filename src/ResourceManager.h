#pragma once

#include "Actions/Action.h"
#include "Animation.h"
#include "AudioSource.h"
#include "BitmapFont.h"
#include "Font2.h"
#include "Game/Level.h"
#include "IgnoreResource.h"
#include "ImageContainers/ImageContainer.h"
#include <list>
#include <memory>
#include "Music2.h"
#include "MusicLoops.h"
#include "Palette.h"
#include "ReverseIterable.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "TexturePacks/TexturePack.h"
#include "UIObject.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Button;

struct ResourceBundle
{
	struct CompareKeyEvent
	{
		size_t operator()(const sf::Event::KeyEvent& obj) const
		{
			return ((obj.code << 4) |
				(obj.alt << 0) |
				(obj.control << 1) |
				(obj.shift << 2) |
				(obj.system << 3));
		}
		bool operator()(const sf::Event::KeyEvent& obj1, const sf::Event::KeyEvent& obj2) const
		{
			return (obj1.code == obj2.code &&
				obj1.alt == obj2.alt &&
				obj1.control == obj2.control &&
				obj1.shift == obj2.shift &&
				obj1.system == obj2.system);
		}
	};

	ResourceBundle() {}
	ResourceBundle(const std::string& id_) :id(id_) {}

	IgnoreResource ignore{ IgnoreResource::None };
	std::string id;
	std::unordered_map<std::string, std::shared_ptr<Action>> actions;
	std::unordered_map<sf::Event::KeyEvent, std::shared_ptr<Action>, CompareKeyEvent, CompareKeyEvent> keyboardActions;
	std::unordered_map<std::string, std::shared_ptr<Font2>> fonts;
	std::unordered_map<std::string, std::shared_ptr<BitmapFont>> bitmapFonts;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
	std::unordered_map<std::string, AudioSource> audioSources;
	std::unordered_map<std::string, std::shared_ptr<sf::Music2>> songs;
	std::unordered_map<std::string, std::shared_ptr<Palette>> palettes;
	std::unordered_map<std::string, std::shared_ptr<ImageContainer>> imageContainers;
	std::unordered_map<std::string, std::shared_ptr<TexturePack>> texturePacks;

	std::unordered_set<std::string> drawableIds;
	std::vector<std::pair<std::string, std::shared_ptr<UIObject>>> drawables;
	std::vector<std::shared_ptr<Button>> focusButtons;
	size_t focusIdx{ 0 };
};

class ResourceManager
{
private:
	std::vector<ResourceBundle> resources;
	std::vector<std::shared_ptr<Image>> cursors;
	std::list<sf::Sound> playingSounds;
	mutable std::pair<std::string, UIObject*> drawableCache{ "", nullptr };
	Level* currentLevel{ nullptr };
	size_t currentLevelResourceIdx{ 0 };

	void clearCache()
	{
		drawableCache.first.clear();
		drawableCache.second = nullptr;
	}

	void clearCurrentLevel()
	{
		if (currentLevelResourceIdx > resources.size() - 1)
		{
			currentLevel = nullptr;
			currentLevelResourceIdx = 0;
		}
	}

	bool addFont(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<Font2>& obj);
	bool addBitmapFont(ResourceBundle& res,
		const std::string& key, const std::shared_ptr<BitmapFont>& obj);
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

	iterator begin() { return resources.begin(); }
	iterator end() { return resources.end(); }
	const_iterator begin() const { return resources.begin(); }
	const_iterator end() const { return resources.end(); }
	const_iterator cbegin() const { return resources.cbegin(); }
	const_iterator cend() const { return resources.cend(); }
	reverse_iterator rbegin() { return resources.rbegin(); }
	reverse_iterator rend() { return resources.rend(); }
	const_reverse_iterator rbegin() const { return resources.rbegin(); }
	const_reverse_iterator rend() const { return resources.rend(); }
	const_reverse_iterator crbegin() const { return resources.crbegin(); }
	const_reverse_iterator crend() const { return resources.crend(); }

	ResourceManager() : resources(1, ResourceBundle()) {}

	void setCurrentLevel(Level* level)
	{
		currentLevel = level;
		currentLevelResourceIdx = resources.size() - 1;
	}
	void addResource(const std::string& id);
	void popResource();
	void popResource(const std::string& id);
	void popAllResources(const std::string& id);
	void popAllResources(bool popBaseResources);
	void ignoreResources(const std::string& id, IgnoreResource ignore);
	void ignoreTopResource(IgnoreResource ignore);
	bool resourceExists(const std::string& id) const;

	Image* getCursor() const;
	Level* getCurrentLevel() const { return currentLevel; }
	Level* getLevel(const std::string& id) const
	{
		if (id.empty() == true)
		{
			return currentLevel;
		}
		return getResource<Level>(id);
	}
	void addCursor(const std::shared_ptr<Image>& cursor_) { cursors.push_back(cursor_); }
	void popCursor(bool popAll = false);
	void popAllCursors() { cursors.clear(); }
	size_t cursorCount() const { return cursors.size(); }

	void setKeyboardAction(const sf::Event::KeyEvent& key, const std::shared_ptr<Action>& obj);
	void setAction(const std::string& key, const std::shared_ptr<Action>& obj);

	bool addFont(const std::string& key, const std::shared_ptr<Font2>& obj)
	{
		return addFont(resources.back(), key, obj);
	}
	bool addBitmapFont(const std::string& key, const std::shared_ptr<BitmapFont>& obj)
	{
		return addBitmapFont(resources.back(), key, obj);
	}
	bool addTexture(const std::string& key, const std::shared_ptr<sf::Texture>& obj)
	{
		return addTexture(resources.back(), key, obj);
	}
	bool addAudioSource(const std::string& key, const AudioSource& obj)
	{
		return addAudioSource(resources.back(), key, obj);
	}
	bool addSong(const std::string& key, const std::shared_ptr<sf::Music2>& obj)
	{
		return addSong(resources.back(), key, obj);
	}
	bool addPalette(const std::string& key, const std::shared_ptr<Palette>& obj)
	{
		return addPalette(resources.back(), key, obj);
	}
	bool addImageContainer(const std::string& key, const std::shared_ptr<ImageContainer>& obj)
	{
		return addImageContainer(resources.back(), key, obj);
	}
	bool addTexturePack(const std::string& key, const std::shared_ptr<TexturePack>& obj)
	{
		return addTexturePack(resources.back(), key, obj);
	}

	void addDrawable(const std::string& key, const std::shared_ptr<UIObject>& obj)
	{
		return addDrawable(resources.back(), key, obj);
	}
	void addDrawable(const std::string& resourceId,
		const std::string& key, const std::shared_ptr<UIObject>& obj);

	void addPlayingSound(const sf::Sound& obj, bool unique = false);

	void clearFinishedSounds();
	void clearPlayingSounds() { playingSounds.clear(); }

	std::shared_ptr<Action> getKeyboardAction(const sf::Event::KeyEvent& key) const;
	std::shared_ptr<Action> getAction(const std::string& key) const;
	std::shared_ptr<Font2> getFont(const std::string& key) const;
	std::shared_ptr<BitmapFont> getBitmapFont(const std::string& key) const;
	std::shared_ptr<sf::Texture> getTexture(const std::string& key) const;
	AudioSource getAudioSource(const std::string& key) const;
	sf::SoundBuffer* getSoundBuffer(const std::string& key) const;
	std::shared_ptr<sf::Music2> getSong(const std::string& key) const;
	std::shared_ptr<Palette> getPalette(const std::string& key) const;
	std::shared_ptr<ImageContainer> getImageContainer(const std::string& key) const;
	std::shared_ptr<TexturePack> getTexturePack(const std::string& key) const;

	bool hasSong(const std::string& key, bool checkTopOnly = false) const;
	bool hasPalette(const std::string& key) const;
	bool hasImageContainer(const std::string& key) const;
	bool hasTexturePack(const std::string& key) const;

	bool hasDrawable(const std::string& key) const;

	UIObject* getDrawable(const std::string& key) const;

	template <class T>
	T* getResource(const std::string& key) const
	{
		for (const auto& res : reverse(resources))
		{
			for (const auto& elem : res.drawables)
			{
				if (elem.first == key)
				{
					return dynamic_cast<T*>(elem.second.get());
				}
			}
		}
		return nullptr;
	}

	template <class T>
	std::shared_ptr<T> getResourceSharedPtr(const std::string& key) const
	{
		for (const auto& res : reverse(resources))
		{
			for (const auto& elem : res.drawables)
			{
				if (elem.first == key)
				{
					return std::dynamic_pointer_cast<T>(elem.second);
				}
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
};
