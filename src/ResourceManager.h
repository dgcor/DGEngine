#pragma once

#include "Actions/Action.h"
#include "Animation.h"
#include "BitmapFont.h"
#include "CelCache.h"
#include "Font2.h"
#include "Game/Level.h"
#include <list>
#include <memory>
#include "Music2.h"
#include "Palette.h"
#include <string>
#include "UIObject.h"
#include <unordered_map>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Button;

enum class IgnoreResource
{
	None,
	DrawAndUpdate,
	Update
};

struct ResourceBundle
{
	struct CompareKeyEvent
	{
		size_t operator()(const sf::Event::KeyEvent& obj) const
		{
			return ((obj.code << 4) | (obj.alt << 0) | (obj.control << 1) | (obj.shift << 2) | (obj.system << 3));
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
	~ResourceBundle()
	{
		drawables.clear();
		fonts.clear();
		bitmapFonts.clear();
		textures.clear();
		songs.clear();
		sounds.clear();
		palettes.clear();
	}

	IgnoreResource ignore{ IgnoreResource::None };
	std::string id;
	std::unordered_map<std::string, std::shared_ptr<Action>> actions;
	std::unordered_map<sf::Event::KeyEvent, std::shared_ptr<Action>, CompareKeyEvent, CompareKeyEvent> keyboardActions;
	std::unordered_map<std::string, std::shared_ptr<Font2>> fonts;
	std::unordered_map<std::string, std::shared_ptr<BitmapFont>> bitmapFonts;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
	std::unordered_map<std::string, std::shared_ptr<Music2>> songs;
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sounds;
	std::unordered_map<std::string, std::shared_ptr<Palette>> palettes;
	std::unordered_map<std::string, std::shared_ptr<CelFile>> celFiles;
	std::unordered_map<std::string, std::shared_ptr<CelTextureCacheVector>> celCaches;

	std::vector<std::pair<std::string, std::shared_ptr<UIObject>>> drawables;
	std::vector<std::shared_ptr<Button>> focusButtons;
	size_t focusIdx{ 0 };
};

class ResourceManager
{
private:
	std::vector<ResourceBundle> resources;
	std::vector<std::shared_ptr<Animation>> cursors;
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

	Animation* getCursor() const;
	Level* getCurrentLevel() const { return currentLevel; }
	Level* getLevel(const std::string& id) const
	{
		if (id.empty() == true)
		{
			return currentLevel;
		}
		return getResource<Level>(id);
	}
	void addCursor(const std::shared_ptr<Animation>& cursor_) { cursors.push_back(cursor_); }
	void popCursor(bool popAll = false);
	void popAllCursors() { cursors.clear(); }

	void setKeyboardAction(const sf::Event::KeyEvent& key, const std::shared_ptr<Action>& obj);
	void setAction(const std::string& key, const std::shared_ptr<Action>& obj);
	void addFont(const std::string& key, const std::shared_ptr<Font2>& obj);
	void addBitmapFont(const std::string& key, const std::shared_ptr<BitmapFont>& obj);
	void addTexture(const std::string& key, const std::shared_ptr<sf::Texture>& obj);
	void addSong(const std::string& key, const std::shared_ptr<Music2>& obj);
	void addSound(const std::string& key, const std::shared_ptr<sf::SoundBuffer>& obj);
	void addPalette(const std::string& key, const std::shared_ptr<Palette>& obj);
	void addCelFile(const std::string& key, const std::shared_ptr<CelFile>& obj);
	void addCelTextureCache(const std::string& key, const std::shared_ptr<CelTextureCacheVector>& obj);

	void addDrawable(const std::string& key, const std::shared_ptr<UIObject>& obj);

	void addPlayingSound(sf::Sound obj);
	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	void clearFinishedSounds();
	void clearPlayingSounds() { playingSounds.clear(); }

	std::shared_ptr<Action> getKeyboardAction(const sf::Event::KeyEvent& key) const;
	std::shared_ptr<Action> getAction(const std::string& key) const;
	std::shared_ptr<Font2> getFont(const std::string& key) const;
	std::shared_ptr<BitmapFont> getBitmapFont(const std::string& key) const;
	std::shared_ptr<sf::Texture> getTexture(const std::string& key) const;
	std::shared_ptr<Music2> getSong(const std::string& key) const;
	std::shared_ptr<sf::SoundBuffer> getSound(const std::string& key) const;
	std::shared_ptr<Palette> getPalette(const std::string& key) const;
	std::shared_ptr<CelFile> getCelFile(const std::string& key) const;
	std::shared_ptr<CelTextureCacheVector> getCelTextureCache(const std::string& key) const;

	UIObject* getDrawable(const std::string& key) const;

	template <class T>
	T* getResource(const std::string& key) const
	{
		for (auto it = resources.rbegin(); it != resources.rend(); ++it)
		{
			for (const auto& elem : it->drawables)
			{
				if (elem.first == key)
				{
					return dynamic_cast<T*>(elem.second.get());
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

	void setFocusIdx(const Button* obj);
};
