#pragma once

#include "Actions/Action.h"
#include "AudioSource.h"
#include "FileBytes.h"
#include "Font.h"
#include "IgnoreResource.h"
#include "ImageContainers/ImageContainer.h"
#include <initializer_list>
#include "InputEvent.h"
#include <list>
#include <memory>
#include "Palette.h"
#include <SFML/Audio/Sound.hpp>
#include "SFML/Music2.h"
#include "SFML/MusicLoops.h"
#include "ShaderManager.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "UIObject.h"
#include "Utils/ReverseIterable.h"
#include "Utils/UnorderedStringMap.h"
#include <variant>
#include <vector>

class Button;
class Image;

struct ResourceBundle
{
	typedef std::variant<
		std::shared_ptr<FileBytes>,
		Font,
		std::shared_ptr<sf::Texture>,
		AudioSource,
		std::shared_ptr<Palette>,
		std::shared_ptr<ImageContainer>,
		std::shared_ptr<TexturePack>>
		Resource;

	IgnoreResource ignore{ IgnoreResource::None };
	std::string id;
	std::shared_ptr<Action> anyKeyAction;
	UnorderedStringMap<std::shared_ptr<Action>> actions;
	std::unordered_map<sf::Event, std::shared_ptr<Action>,
		CompareEvent, CompareEvent> inputActions;
	std::vector<std::pair<InputEvent, uint16_t>> inputEvents;
	UnorderedStringMultiMap<Resource> resources;
	UnorderedStringMap<std::shared_ptr<sf::Music2>> songs;
	UnorderedStringMap<std::shared_ptr<UIObject>> drawableIds;
	std::vector<UIObject*> drawables;
	mutable std::vector<std::weak_ptr<Button>> focusButtons;
	mutable size_t focusIdx{ 0 };

	ResourceBundle() noexcept {}
	ResourceBundle(const std::string& id_) :id(id_) {}

	template <class T>
	bool hasResource(const std::string_view key) const
	{
		auto range = resources.equal_range(sv2str(key));
		for (; range.first != range.second; ++range.first)
		{
			if (std::holds_alternative<T>(range.first->second) == true)
			{
				return true;
			}
		}
		return false;
	}

	template <class T>
	T getResource(const std::string_view key) const
	{
		auto range = resources.equal_range(sv2str(key));
		for (; range.first != range.second; ++range.first)
		{
			if (std::holds_alternative<T>(range.first->second) == true)
			{
				return std::get<T>(range.first->second);
			}
		}
		return nullptr;
	}
};

class ResourceManager
{
private:
	std::vector<ResourceBundle> resources;
	ShaderManager shaders;
	std::vector<std::shared_ptr<Image>> cursors;
	std::vector<uint16_t> activeInputEvents;
	std::list<sf::Sound> playingSounds;
	UIObject* currentLevel{ nullptr };
	size_t currentLevelResourceIdx{ 0 };

	void clearCurrentLevel() noexcept
	{
		if (currentLevelResourceIdx + 1 > resources.size())
		{
			currentLevel = nullptr;
			currentLevelResourceIdx = 0;
		}
	}

	bool addSong(ResourceBundle& res,
		const std::string_view key, const std::shared_ptr<sf::Music2>& obj);

	void addDrawable(ResourceBundle& res, const std::string_view key,
		const std::shared_ptr<UIObject>& obj, bool manageObjDrawing);

	void addFocused(ResourceBundle& res, const std::shared_ptr<Button>& obj);

	void removeUnusedFocused(const ResourceBundle& res) const;

	template <class T>
	bool addResource(ResourceBundle& res, const std::string_view key, const T& obj)
	{
		if (res.hasResource<T>(key) == false)
		{
			res.resources.insert(std::make_pair(key, obj));
			return true;
		}
		return false;
	}

	template <class T>
	bool addResource(const std::string_view key, const T& obj, const std::string_view resourceId)
	{
		if (resourceId.empty() == true)
		{
			return addResource(resources.back(), key, obj);
		}
		for (auto& res : reverse(resources))
		{
			if (res.id == resourceId)
			{
				return addResource(res, key, obj);
			}
		}
		return false;
	}

	template <class T>
	bool hasResource(const std::string_view key, bool checkTopOnly) const
	{
		for (const auto& resource : resources)
		{
			if (resource.hasResource<T>(key) == true)
			{
				return true;
			}
			if (checkTopOnly == true)
			{
				return false;
			}
		}
		return false;
	}

	template <class T>
	T getResource(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto range = res.resources.equal_range(sv2str(key));
			for (; range.first != range.second; ++range.first)
			{
				if (std::holds_alternative<T>(range.first->second) == true)
				{
					return std::get<T>(range.first->second);
				}
			}
		}
		return {};
	}

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

	void setCurrentLevel(UIObject* level) noexcept
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
	void bringResourceToFront(const std::string& id);

	Image* getCursor() const;

	template <class T>
	T* getCurrentLevel() const noexcept { return (T*)currentLevel; }

	template <class T>
	T* getLevel(const std::string_view id) const noexcept
	{
		if (id.empty() == true)
		{
			return (T*)currentLevel;
		}
		return getDrawable<T>(id);
	}

	void addCursor(const std::shared_ptr<Image>& cursor_) { cursors.push_back(cursor_); }
	void popCursor(bool popAll = false);
	void popAllCursors() noexcept { cursors.clear(); }
	size_t cursorCount() const noexcept { return cursors.size(); }

	void setInputAction(const sf::Event& evt, const std::shared_ptr<Action>& obj);
	void setInputEvent(const InputEvent& evt, uint16_t actionHash16);
	void setAction(const std::string_view key, const std::shared_ptr<Action>& obj);

	bool addFileBytes(const std::string_view key, const std::shared_ptr<FileBytes>& obj,
		const std::string_view resourceId = {});
	bool addFont(const std::string_view key, const Font& obj,
		const std::string_view resourceId = {});
	bool addTexture(const std::string_view key, const std::shared_ptr<sf::Texture>& obj,
		const std::string_view resourceId = {});
	bool addAudioSource(const std::string_view key, const AudioSource& obj,
		const std::string_view resourceId = {});
	bool addSong(const std::string_view key, const std::shared_ptr<sf::Music2>& obj,
		const std::string_view resourceId = {});
	bool addPalette(const std::string_view key, const std::shared_ptr<Palette>& obj,
		const std::string_view resourceId = {});
	bool addImageContainer(const std::string_view key, const std::shared_ptr<ImageContainer>& obj,
		const std::string_view resourceId = {});
	bool addTexturePack(const std::string_view key, const std::shared_ptr<TexturePack>& obj,
		const std::string_view resourceId = {});

	void addDrawable(const std::string_view key, const std::shared_ptr<UIObject>& obj,
		bool manageObjDrawing, const std::string_view resourceId = {});

	void addPlayingSound(const sf::Sound& obj, bool unique = false,
		sf::Time seek = sf::Time::Zero);

	void clearFinishedSounds();
	void clearPlayingSounds() noexcept { playingSounds.clear(); }

	std::shared_ptr<Action> getInputAction(const sf::Event& key) const;
	std::shared_ptr<Action> getAction(const std::string_view key) const;
	std::shared_ptr<FileBytes> getFileBytes(const std::string_view key) const;
	Font getFont(const std::string_view key) const;
	std::shared_ptr<sf::Texture> getTexture(const std::string_view key) const;
	AudioSource getAudioSource(const std::string_view key) const;
	sf::SoundBuffer* getSoundBuffer(const std::string_view key) const;
	std::shared_ptr<sf::Music2> getSong(const std::string_view key) const;
	std::shared_ptr<Palette> getPalette(const std::string_view key) const;
	std::shared_ptr<ImageContainer> getImageContainer(const std::string_view key) const;
	std::shared_ptr<TexturePack> getTexturePack(const std::string_view key) const;

	bool hasFileBytes(const std::string_view key) const;
	bool hasFont(const std::string_view key) const;
	bool hasTexture(const std::string_view key) const;
	bool hasAudioSource(const std::string_view key) const;
	bool hasSong(const std::string_view key, bool checkTopOnly = false) const;
	bool hasPalette(const std::string_view key) const;
	bool hasImageContainer(const std::string_view key) const;
	bool hasTexturePack(const std::string_view key) const;

	bool hasDrawable(const std::string_view key) const;

	void bringDrawableToFront(const std::string& id);
	void sendDrawableToBack(const std::string& id);

	UIObject* getDrawable(const std::string_view key) const
	{
		return getDrawable<UIObject>(key);
	}

	template <class T>
	T* getDrawable(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(sv2str(key));
			if (it != res.drawableIds.cend())
			{
				return dynamic_cast<T*>(it->second.get());
			}
		}
		return nullptr;
	}

	template <class T>
	std::shared_ptr<T> getDrawableSharedPtr(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(sv2str(key));
			if (it != res.drawableIds.cend())
			{
				return std::dynamic_pointer_cast<T>(it->second);
			}
		}
		return nullptr;
	}

	// deletes a drawable from drawableIds and drawables vector
	void deleteDrawable(const std::string& id);

	// deletes a drawable from the drawables vector (and not from drawableIds)
	void deleteDrawable(UIObject* obj);

	void play(const std::string_view key);
	void playPaused(bool restart);
	void pauseSongs();
	void stopSongs();
	void deleteSong(const std::string_view key);

	void addFocused(const std::shared_ptr<Button>& obj,
		const std::string_view resourceId = {});
	void clickFocused(Game& game, bool playSound);
	void rightClickFocused(Game& game, bool playSound);
	const Button* getFocused() const;
	void setFocused(const Button* obj);

	void moveFocusDown(Game& game);
	void moveFocusUp(Game& game);
	void updateFocus(Game& game);

	bool hasActiveInputEvents() const noexcept { return activeInputEvents.empty() == false; }
	bool hasActiveInputEvents(const std::initializer_list<uint16_t> nameHashes) const;
	void updateActiveInputEvents();
};
