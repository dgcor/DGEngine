#include "ResourceManager.h"
#include "Button.h"
#include <cctype>
#include "Game/Level.h"

void ResourceManager::addResource(const std::string& id)
{
	resources.push_back(ResourceBundle(id));
}

void ResourceManager::popResource()
{
	if (resources.size() > 0)
	{
		resources.pop_back();
		clearCurrentLevel();
	}
}

void ResourceManager::popResource(const std::string& id)
{
	for (auto it = resources.rbegin(); it != resources.rend(); ++it)
	{
		if (it->id == id)
		{
			auto idx = std::distance(std::begin(resources), it.base()) - 1;
			if (idx == (int)currentLevelResourceIdx)
			{
				currentLevel = nullptr;
				currentLevelResourceIdx = 0;
			}
			else if (idx < (int)currentLevelResourceIdx)
			{
				currentLevelResourceIdx--;
			}
			resources.erase(--it.base());
			return;
		}
	}
}

void ResourceManager::popAllResources(const std::string& id)
{
	for (auto it = resources.rbegin(); it != resources.rend(); ++it)
	{
		if (it->id == id && it.base() != resources.begin())
		{
			resources.erase(--it.base(), resources.end());
			clearCurrentLevel();
			return;
		}
	}
}

void ResourceManager::popAllResources(bool popBaseResources)
{
	resources.resize(1);
	if (popBaseResources)
	{
		resources.front() = {};
		currentLevel = nullptr;
		currentLevelResourceIdx = 0;
	}
	else
	{
		clearCurrentLevel();
	}
}

void ResourceManager::ignoreResources(const std::string& id, IgnoreResource ignore) noexcept
{
	for (auto& res : reverse(resources))
	{
		if (res.id == id)
		{
			res.ignore = ignore;
			return;
		}
	}
}

void ResourceManager::ignoreTopResource(IgnoreResource ignore)
{
	if (resources.size() > 0)
	{
		resources.back().ignore = ignore;
	}
}

bool ResourceManager::resourceExists(const std::string_view id) const noexcept
{
	for (auto& res : resources)
	{
		if (res.id == id)
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::moveResourceToTop(const std::string& id)
{
	auto it = std::find_if(resources.begin(), resources.end(),
		[&id](const auto& res) -> bool { return res.id == id; }
	);
	if (it != resources.end())
	{
		std::rotate(it, it + 1, resources.end());
	}
}

Level* ResourceManager::getLevel(const std::string& id) const noexcept
{
	if (id.empty() == true)
	{
		return currentLevel;
	}
	return getDrawable<Level>(id);
}

Image* ResourceManager::getCursor() const
{
	if (cursors.empty() == true)
	{
		return nullptr;
	}
	return cursors.back().get();
}

void ResourceManager::popCursor(bool popAll)
{
	if (cursors.empty() == true)
	{
		return;
	}
	if (popAll)
	{
		cursors.clear();
	}
	else
	{
		cursors.pop_back();
	}
}

void ResourceManager::setKeyboardAction(const sf::Event::KeyEvent& key, const std::shared_ptr<Action>& obj)
{
	if (key.code == sf::Keyboard::KeyCount)
	{
		resources.back().anyKeyAction = obj;
		return;
	}
	resources.back().keyboardActions[key] = obj;
}

void ResourceManager::setAction(const std::string& key, const std::shared_ptr<Action>& obj)
{
	resources.back().actions[key] = obj;
}

bool ResourceManager::addFont(ResourceBundle& res,
	const std::string& key, const Font& obj)
{
	if (res.fonts.find(key) == res.fonts.cend())
	{
		res.fonts[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addFont(const std::string& key,
	const Font& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addFont(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addFont(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addTexture(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<sf::Texture>& obj)
{
	if (res.textures.find(key) == res.textures.cend())
	{
		res.textures[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addTexture(const std::string& key,
	const std::shared_ptr<sf::Texture>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addTexture(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addTexture(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addAudioSource(ResourceBundle& res,
	const std::string& key, const AudioSource& obj)
{
	if (res.audioSources.find(key) == res.audioSources.cend())
	{
		res.audioSources[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addAudioSource(const std::string& key,
	const AudioSource& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addAudioSource(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addAudioSource(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addSong(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<sf::Music2>& obj)
{
	if (hasSong(key, true) == false)
	{
		res.songs[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addSong(const std::string& key,
	const std::shared_ptr<sf::Music2>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addSong(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addSong(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addPalette(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<Palette>& obj)
{
	if (hasPalette(key) == false)
	{
		res.palettes[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addPalette(const std::string& key,
	const std::shared_ptr<Palette>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addPalette(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addPalette(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addImageContainer(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<ImageContainer>& obj)
{
	if (hasImageContainer(key) == false)
	{
		res.imageContainers[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addImageContainer(const std::string& key,
	const std::shared_ptr<ImageContainer>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addImageContainer(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addImageContainer(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addTexturePack(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<TexturePack>& obj)
{
	if (hasTexturePack(key) == false)
	{
		res.texturePacks[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addTexturePack(const std::string& key,
	const std::shared_ptr<TexturePack>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addTexturePack(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addTexturePack(res, key, obj);
		}
	}
	return false;
}

void ResourceManager::addDrawable(ResourceBundle& res, const std::string& key,
	const std::shared_ptr<UIObject>& obj, bool manageObjDrawing)
{
	auto it = res.drawableIds.find(key);
	if (it == res.drawableIds.end())
	{
		res.drawableIds[key] = obj;
		if (manageObjDrawing == true)
		{
			res.drawables.push_back(obj.get());
		}
	}
}

void ResourceManager::addDrawable(const std::string& key,
	const std::shared_ptr<UIObject>& obj, bool manageObjDrawing,
	const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		addDrawable(resources.back(), key, obj, manageObjDrawing);
		return;
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			addDrawable(res, key, obj, manageObjDrawing);
			return;
		}
	}
}

void ResourceManager::addPlayingSound(const sf::Sound& obj, bool unique)
{
	if (unique == true)
	{
		for (const auto& snd : playingSounds)
		{
			if (snd.getBuffer() == obj.getBuffer())
			{
				return;
			}
		}
	}
	playingSounds.push_back(obj);
	playingSounds.back().play();
}

void ResourceManager::clearFinishedSounds()
{
	for (auto it = playingSounds.begin(); it != playingSounds.end();)
	{
		if ((*it).getStatus() != sf::Sound::Playing)
		{
			it = playingSounds.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::shared_ptr<Action> ResourceManager::getKeyboardAction(const sf::Event::KeyEvent& key) const
{
	for (const auto& res : reverse(resources))
	{
		if (res.anyKeyAction != nullptr)
		{
			return res.anyKeyAction;
		}
		const auto elem = res.keyboardActions.find(key);
		if (elem != res.keyboardActions.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<Action> ResourceManager::getAction(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.actions.find(key);
		if (elem != res.actions.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

Font ResourceManager::getFont(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.fonts.find(key);
		if (elem != res.fonts.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.textures.find(key);
		if (elem != res.textures.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

AudioSource ResourceManager::getAudioSource(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.audioSources.find(key);
		if (elem != res.audioSources.cend())
		{
			return elem->second;
		}
	}
	return {};
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key) const
{
	auto elem = getAudioSource(key);
	if (std::holds_alternative<std::shared_ptr<sf::SoundBuffer>>(elem) == true)
	{
		return std::get<std::shared_ptr<sf::SoundBuffer>>(elem).get();
	}
	else if (std::holds_alternative<std::shared_ptr<SoundBufferLoops>>(elem) == true)
	{
		return &std::get<std::shared_ptr<SoundBufferLoops>>(elem)->soundBuffer;
	}
	return nullptr;
}

std::shared_ptr<sf::Music2> ResourceManager::getSong(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.songs.find(key);
		if (elem != res.songs.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<Palette> ResourceManager::getPalette(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.palettes.find(key);
		if (elem != res.palettes.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<ImageContainer> ResourceManager::getImageContainer(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.imageContainers.find(key);
		if (elem != res.imageContainers.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<TexturePack> ResourceManager::getTexturePack(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.texturePacks.find(key);
		if (elem != res.texturePacks.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

bool ResourceManager::hasFont(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.fonts.find(key) != resource.fonts.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasTexture(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.textures.find(key) != resource.textures.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasAudioSource(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.audioSources.find(key) != resource.audioSources.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasSong(const std::string& key, bool checkTopOnly) const
{
	for (const auto& resource : resources)
	{
		if (resource.songs.find(key) != resource.songs.cend())
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

bool ResourceManager::hasPalette(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.palettes.find(key) != resource.palettes.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasImageContainer(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.imageContainers.find(key) != resource.imageContainers.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasTexturePack(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.texturePacks.find(key) != resource.texturePacks.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasDrawable(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.drawableIds.find(key) != resource.drawableIds.cend())
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::deleteDrawable(const std::string& id)
{
	for (auto& res : reverse(resources))
	{
		auto it1 = res.drawableIds.find(id);
		if (it1 != res.drawableIds.end())
		{
			auto objPtr = it1->second.get();
			res.drawableIds.erase(it1);
			auto& drawables = res.drawables;
			for (auto it2 = drawables.begin(); it2 != drawables.end(); ++it2)
			{
				if (*it2 == objPtr)
				{
					drawables.erase(it2);
					return;
				}
			}
		}
	}
}

void ResourceManager::play(const std::string& key)
{
	auto& song = resources.back().songs[key];
	if (song != nullptr)
	{
		song->play();
	}
}

void ResourceManager::playPaused(bool restart)
{
	for (auto& res : resources) {
		for (auto& song : res.songs) {
			if (song.second->getStatus() == sf::SoundSource::Paused) {
				if (restart) {
					song.second->setPlayingOffset(sf::Time::Zero);
				}
				song.second->play();
			}
		}
	}
}

void ResourceManager::pauseSongs()
{
	for (auto& res : resources) {
		for (auto& song : res.songs) {
			if (song.second->getStatus() == sf::SoundSource::Playing) {
				song.second->pause();
			}
		}
	}
}

void ResourceManager::stopSongs()
{
	for (auto& res : resources) {
		for (auto& song : res.songs) {
			song.second->stop();
		}
	}
}

void ResourceManager::addFocused(ResourceBundle& res, const std::shared_ptr<Button>& obj)
{
	for (const auto& focusBtn : res.focusButtons)
	{
		auto btnPtr = focusBtn.lock();
		if (btnPtr != nullptr && btnPtr == obj)
		{
			return;
		}
	}
	res.focusButtons.push_back(obj);
}

void ResourceManager::addFocused(const std::shared_ptr<Button>& obj,
	const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		addFocused(resources.back(), obj);
		return;
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			addFocused(res, obj);
			return;
		}
	}
}

void ResourceManager::removeUnusedFocused(const ResourceBundle& res) const
{
	for (auto it = res.focusButtons.begin(); it != res.focusButtons.end();)
	{
		if (it->expired() == true)
		{
			it = res.focusButtons.erase(it);
			if (res.focusIdx != 0 &&
				res.focusIdx >= res.focusButtons.size())
			{
				res.focusIdx--;
			}
		}
		else
		{
			++it;
		}
	}
}

void ResourceManager::clickFocused(Game& game, bool playSound)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false &&
			res.focusIdx < res.focusButtons.size())
		{
			res.focusButtons[res.focusIdx].lock()->click(game, playSound);
		}
		break;
	}
}

const Button* ResourceManager::getFocused() const
{
	for (const auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false &&
			res.focusIdx < res.focusButtons.size())
		{
			return res.focusButtons[res.focusIdx].lock().get();
		}
		break;
	}
	return nullptr;
}

void ResourceManager::setFocused(const Button* obj)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		if (res.focusButtons.empty() == false)
		{
			for (size_t i = 0; i < res.focusButtons.size(); i++)
			{
				if (res.focusButtons[i].lock().get() == obj)
				{
					res.focusIdx = i;
					return;
				}
			}
		}
		break;
	}
}

void ResourceManager::moveFocusDown(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx + 1 < res.focusButtons.size())
				{
					idx++;
				}
				else
				{
					idx = 0;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
				focus->focus(game);
			}
		}
		break;
	}
}

void ResourceManager::moveFocusUp(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx > 0)
				{
					idx--;
				}
				else
				{
					idx = res.focusButtons.size() - 1;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
				focus->focus(game);
			}
		}
		break;
	}
}

void ResourceManager::updateFocus(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			if (res.focusButtons[res.focusIdx].lock()->isEnabled() == true)
			{
				break;
			}
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx + 1 < res.focusButtons.size())
				{
					idx++;
				}
				else
				{
					idx = 0;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
			}
		}
		break;
	}
}
