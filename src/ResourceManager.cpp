#include "ResourceManager.h"
#include "Button.h"
#include <cctype>
#include "ReverseIterable.h"

void ResourceManager::addResource(const std::string& id)
{
	resources.push_back(ResourceBundle(id));
	clearCache();
}

void ResourceManager::popResource()
{
	if (resources.size() > 0)
	{
		resources.pop_back();
		clearCache();
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
			clearCache();
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
			clearCache();
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
		resources.front() = ResourceBundle();
		currentLevel = nullptr;
		currentLevelResourceIdx = 0;
	}
	else
	{
		clearCurrentLevel();
	}
	clearCache();
}

void ResourceManager::ignoreResources(const std::string& id, IgnoreResource ignore)
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

bool ResourceManager::resourceExists(const std::string& id) const
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

UIObject* ResourceManager::getCursor() const
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
	resources.back().keyboardActions[key] = obj;
}

void ResourceManager::setAction(const std::string& key, const std::shared_ptr<Action>& obj)
{
	resources.back().actions[key] = obj;
}

void ResourceManager::addFont(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<Font2>& obj)
{
	if (res.fonts.find(key) == res.fonts.cend())
	{
		res.fonts[key] = obj;
	}
}

void ResourceManager::addBitmapFont(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<BitmapFont>& obj)
{
	if (res.bitmapFonts.find(key) == res.bitmapFonts.cend())
	{
		res.bitmapFonts[key] = obj;
	}
}

void ResourceManager::addTexture(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<sf::Texture>& obj)
{
	if (res.textures.find(key) == res.textures.cend())
	{
		res.textures[key] = obj;
	}
}

void ResourceManager::addSong(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<Music2>& obj)
{
	if (res.songs.find(key) == res.songs.cend())
	{
		res.songs[key] = obj;
	}
}

void ResourceManager::addSound(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<sf::SoundBuffer>& obj)
{
	if (res.sounds.find(key) == res.sounds.cend())
	{
		res.sounds[key] = obj;
	}
}

void ResourceManager::addPalette(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<Palette>& obj)
{
	if (hasPalette(key) == false)
	{
		res.palettes[key] = obj;
	}
}

void ResourceManager::addCelFile(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<CelFile>& obj)
{
	if (hasCelFile(key) == false)
	{
		res.celFiles[key] = obj;
	}
}

void ResourceManager::addCelTextureCache(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<CelTextureCache>& obj)
{
	if (hasCelTextureCache(key) == false)
	{
		res.celCaches[key] = obj;
	}
}

void ResourceManager::addCelTextureCacheVec(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<CelTextureCacheVector>& obj)
{
	if (hasCelTextureCacheVec(key) == false)
	{
		res.celCachesVec[key] = obj;
	}
}

void ResourceManager::addDrawable(const std::string& key, const std::shared_ptr<UIObject>& obj)
{
	resources.back().drawables.push_back(std::make_pair(key, obj));
	clearCache();
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

std::shared_ptr<Font2> ResourceManager::getFont(const std::string& key) const
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

std::shared_ptr<BitmapFont> ResourceManager::getBitmapFont(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.bitmapFonts.find(key);

		if (elem != res.bitmapFonts.cend())
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

std::shared_ptr<Music2> ResourceManager::getSong(const std::string& key) const
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

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSound(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.sounds.find(key);

		if (elem != res.sounds.cend())
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

std::shared_ptr<CelFile> ResourceManager::getCelFile(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.celFiles.find(key);

		if (elem != res.celFiles.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<CelTextureCache> ResourceManager::getCelTextureCache(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.celCaches.find(key);

		if (elem != res.celCaches.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<CelTextureCacheVector> ResourceManager::getCelTextureCacheVec(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.celCachesVec.find(key);

		if (elem != res.celCachesVec.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
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

bool ResourceManager::hasCelFile(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.celFiles.find(key) != resource.celFiles.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasCelTextureCache(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.celCaches.find(key) != resource.celCaches.cend())
		{
			return true;
		}
	}
	return false;
}

bool ResourceManager::hasCelTextureCacheVec(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.celCachesVec.find(key) != resource.celCachesVec.cend())
		{
			return true;
		}
	}
	return false;
}

UIObject* ResourceManager::getDrawable(const std::string& key) const
{
	if (drawableCache.first == key)
	{
		return drawableCache.second;
	}
	for (const auto& res : reverse(resources))
	{
		for (const auto& elem : res.drawables)
		{
			if (elem.first == key)
			{
				drawableCache.first = key;
				drawableCache.second = elem.second.get();
				return drawableCache.second;
			}
		}
	}
	return nullptr;
}

void ResourceManager::deleteDrawable(const std::string& id)
{
	for (auto& res : reverse(resources))
	{
		auto& drawables = res.drawables;

		for (auto it = drawables.begin(); it != drawables.end(); ++it)
		{
			if (it->first == id)
			{
				if (drawableCache.first == id)
				{
					clearCache();
				}
				drawables.erase(it);
				return;
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

void ResourceManager::addFocused(const std::shared_ptr<Button>& obj)
{
	if (std::find(resources.back().focusButtons.begin(),
		resources.back().focusButtons.end(), obj)
		== resources.back().focusButtons.end())
	{
		resources.back().focusButtons.push_back(obj);
	}
}

void ResourceManager::clickFocused(Game& game, bool playSound)
{
	for (const auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		const auto& vec = res.focusButtons;
		auto size = vec.size();
		if (size == 0)
		{
			continue;
		}
		auto focusIdx = res.focusIdx;
		if (focusIdx < size)
		{
			vec[focusIdx]->click(game, playSound);
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
		const auto& vec = res.focusButtons;
		auto size = vec.size();
		if (size == 0)
		{
			continue;
		}
		auto focusIdx = res.focusIdx;
		if (focusIdx < size)
		{
			return vec[focusIdx].get();
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
		const auto& vec = res.focusButtons;
		auto size = vec.size();
		if (size == 0)
		{
			continue;
		}
		for (size_t i = 0; i < size; i++)
		{
			if (vec[i].get() == obj)
			{
				res.focusIdx = i;
				return;
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
		const auto& vec = res.focusButtons;
		auto size = vec.size();
		if (size == 0)
		{
			continue;
		}
		auto& focusIdx = res.focusIdx;
		auto idx = focusIdx;

		while (true)
		{
			if (idx + 1 < size)
			{
				idx++;
			}
			else
			{
				idx = 0;
			}
			if (vec[idx]->isEnabled() == true || idx == focusIdx)
			{
				break;
			}
		}
		if (vec[idx]->isEnabled() == true && idx != focusIdx)
		{
			focusIdx = idx;
			vec[idx]->focus(game);
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
		const auto& vec = res.focusButtons;
		auto size = vec.size();
		if (size == 0)
		{
			continue;
		}
		auto& focusIdx = res.focusIdx;
		auto idx = focusIdx;

		while (true)
		{
			if (idx > 0)
			{
				idx--;
			}
			else
			{
				idx = size - 1;
			}
			if (vec[idx]->isEnabled() == true || idx == focusIdx)
			{
				break;
			}
		}
		if (vec[idx]->isEnabled() == true && idx != focusIdx)
		{
			focusIdx = idx;
			vec[idx]->focus(game);
		}
		break;
	}
}
