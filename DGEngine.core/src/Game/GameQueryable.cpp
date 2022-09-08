#include "GameQueryable.h"
#include "ActionQueryable.h"
#include "Formula.h"
#include "Game.h"
#include "Game/Drawables/Button.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/GameUtils.h"
#include "Game/Utils/UIObjectUtils.h"
#include "Game/Utils/VarUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

std::shared_ptr<Action> GameQueryable::getAction(const Game& game, uint16_t nameHash16) noexcept
{
	switch (nameHash16)
	{
	case str2int16("close"):
		return game.closeAction;
	case str2int16("focusGain"):
		return game.focusGainAction;
	case str2int16("focusLoss"):
		return game.focusLossAction;
	case str2int16("resize"):
		return game.resizeAction;
	default:
		return nullptr;
	}
}

bool GameQueryable::setAction(Game& game, uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("close"):
		game.closeAction = action;
		break;
	case str2int16("focusGain"):
		game.focusGainAction = action;
		break;
	case str2int16("focusLoss"):
		game.focusLossAction = action;
		break;
	case str2int16("resize"):
		game.resizeAction = action;
		break;
	default:
		return false;
	}
	return true;
}

std::shared_ptr<Action> GameQueryable::getQueryAction(const Game& game, const std::string_view prop)
{
	auto idx = prop.rfind('.');
	if (idx > 0 && idx != std::string::npos)
	{
		auto query = prop.substr(0, idx);
		auto actionName = prop.substr(idx + 1);
		auto obj = game.getQueryable(query);
		auto queryable = getQueryablePtr(obj);
		if (queryable != nullptr)
		{
			auto uiObj = dynamic_cast<const ActionQueryable*>(queryable);
			if (uiObj != nullptr)
			{
				return uiObj->getAction(str2int16(actionName));
			}
		}
	}
	return nullptr;
}

bool GameQueryable::getProperty(const Game& game, const std::string_view prop, Variable& var)
{
	if (prop.size() <= 2)
	{
		return false;
	}
	if (prop[0] == '#')
	{
		return game.Variables().getVariableNoToken(prop.substr(1), var);
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return game.getProperty(props.first, props.second, var);
}

bool GameQueryable::getProperty(const Game& game, const std::string_view prop1, const std::string_view prop2, Variable& var)
{
	switch (str2int16(prop1))
	{
	case str2int16("$"):
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(prop2, game));
		return true;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(prop2, game));
		return true;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(prop2, game));
		return true;
	case str2int16("$f"):
	case str2int16("evalf"):
		var = Variable(Formula::evalString(prop2, game));
		return true;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(prop2, game));
		return true;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(prop2, game));
		return true;
	case str2int16("game"):
	{
		if (prop2.size() > 1)
		{
			return game.getGameProperty(prop2, var);
		}
		break;
	}
	default:
	{
		std::shared_ptr<UIObject> uiObject;
		if (prop1 == "currentLevel")
		{
			uiObject = game.Resources().getCurrentLevel<UIObject>();
		}
		else if (prop1 == "focus")
		{
			uiObject = game.Resources().getFocused();
		}
		else
		{
			uiObject = game.Resources().getDrawableSharedPtr<UIObject>(prop1);
		}
		if (uiObject != nullptr)
		{
			return uiObject->getProperty(prop2, var);
		}
		break;
	}
	}
	return false;
}

bool GameQueryable::getGameProperty(const Game& game, const std::string_view prop, Variable& var)
{
	auto props = Utils::splitStringIn2(prop, '.');
	return game.getGameProperty(props.first, props.second, var);
}

bool GameQueryable::getGameProperty(const Game& game, const std::string_view prop1, const std::string_view prop2, Variable& var)
{
	switch (str2int16(prop1))
	{
	case str2int16("cursor"):
		var = UIObjectUtils::getTuple2iProp(game.MousePositioni(), prop2);
		break;
	case str2int16("frameRate"):
		var = Variable((int64_t)game.FrameRate());
		break;
	case str2int16("fullScreen"):
		var = Variable(game.FullScreen());
		break;
	case str2int16("gamma"):
		var = Variable((int64_t)game.Gamma());
		break;
	case str2int16("hasAudio"):
		var = Variable(game.Resources().hasAudioSource(prop2));
		break;
	case str2int16("hasDrawable"):
		var = Variable(game.Resources().hasDrawable(prop2));
		break;
	case str2int16("hasEvent"):
		var = Variable(game.Events().exists(prop2));
		break;
	case str2int16("hasFont"):
		var = Variable(game.Resources().hasFont(prop2));
		break;
	case str2int16("hasGameShader"):
		var = Variable(game.Shaders().hasGameShader());
		break;
	case str2int16("hasImageContainer"):
		var = Variable(game.Resources().hasImageContainer(prop2));
		break;
	case str2int16("hasPalette"):
		var = Variable(game.Resources().hasPalette(prop2));
		break;
	case str2int16("hasResource"):
		var = Variable(game.Resources().resourceExists(prop2));
		break;
	case str2int16("hasShader"):
		var = Variable(game.Resources().Shaders().has(prop2));
		break;
	case str2int16("hasSong"):
		var = Variable(game.Resources().hasSong(prop2));
		break;
	case str2int16("hasSpriteShader"):
		var = Variable(game.Shaders().hasSpriteShader());
		break;
	case str2int16("hasTexture"):
		var = Variable(game.Resources().hasTexture(prop2));
		break;
	case str2int16("hasTexturePack"):
		var = Variable(game.Resources().hasTexturePack(prop2));
		break;
	case str2int16("keepAR"):
		var = Variable(game.KeepAR());
		break;
	case str2int16("maxHeight"):
		var = Variable((int64_t)game.MaxHeight());
		break;
	case str2int16("minSize"):
		var = UIObjectUtils::getTuple2iProp(game.MinSize(), prop2);
		break;
	case str2int16("musicVolume"):
		var = Variable((int64_t)game.MusicVolume());
		break;
	case str2int16("openGL"):
	{
		switch (str2int16(prop2))
		{
		case str2int16("antialiasingLevel"):
			var = Variable((int64_t)game.getOpenGLAntialiasingLevel());
			break;
		case str2int16("depthBits"):
			var = Variable((int64_t)game.getOpenGLDepthBits());
			break;
		case str2int16("hasGeometryShaders"):
			var = Variable(sf::Shader::isGeometryAvailable());
			break;
		case str2int16("hasShaders"):
			var = Variable(sf::Shader::isAvailable());
			break;
		case str2int16("majorVersion"):
			var = Variable((int64_t)game.getOpenGLMajorVersion());
			break;
		case str2int16("maxTextureSize"):
			var = Variable((int64_t)sf::Texture::getMaximumSize());
			break;
		case str2int16("minorVersion"):
			var = Variable((int64_t)game.getOpenGLMinorVersion());
			break;
		case str2int16("sRgbCapable"):
			var = Variable(game.getOpenGLSRgbCapable());
			break;
		case str2int16("stencilBits"):
			var = Variable((int64_t)game.getOpenGLStencilBits());
			break;
		default:
			return false;
		}
		break;
	}
	case str2int16("path"):
		var = Variable(game.Path());
		break;
	case str2int16("position"):
		var = UIObjectUtils::getTuple2iProp(game.Position(), prop2);
		break;
	case str2int16("refSize"):
		var = UIObjectUtils::getTuple2iProp(game.RefSize(), prop2);
		break;
	case str2int16("saveDir"):
		var = Variable(FileUtils::getSaveDir());
		break;
	case str2int16("scancodeKey"):
		var = Variable(GameUtils::getScanCodeDescription(prop2));
		break;
	case str2int16("size"):
		var = UIObjectUtils::getTuple2iProp(game.Size(), prop2);
		break;
	case str2int16("smoothScreen"):
		var = Variable(game.SmoothScreen());
		break;
	case str2int16("soundVolume"):
		var = Variable((int64_t)game.SoundVolume());
		break;
	case str2int16("stretchToFit"):
		var = Variable(game.StretchToFit());
		break;
	case str2int16("title"):
		var = Variable(game.Title());
		break;
	case str2int16("version"):
		var = Variable(game.Version());
		break;
	case str2int16("windowedSize"):
		var = UIObjectUtils::getTuple2iProp(game.WindowedSize(), prop2);
		break;
	default:
		return false;
	}
	return true;
}

void GameQueryable::setGameProperty(Game& game, const std::string_view prop, const Variable& val)
{
	if (prop.size() <= 1)
	{
		return;
	}
	switch (str2int16(prop))
	{
	case str2int16("frameRate"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			game.FrameRate((int)std::get<int64_t>(val));
		}
		break;
	}
	case str2int16("fullScreen"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			game.FullScreen(std::get<bool>(val));
		}
		break;
	}
	case str2int16("gamma"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			game.Gamma((int)std::get<int64_t>(val));
		}
		break;
	}
	case str2int16("keepAR"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			game.KeepAR(std::get<bool>(val));
		}
		break;
	}
	case str2int16("maxHeight"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			game.MaxHeight((uint32_t)std::get<int64_t>(val));
		}
		break;
	}
	case str2int16("minSize"):
	{
		if (std::holds_alternative<Int64Tuple2>(val) == true)
		{
			game.MinSize(VarUtils::getVector2uVal<sf::Vector2u>(std::get<Int64Tuple2>(val)));
		}
		break;
	}
	case str2int16("musicVolume"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			game.MusicVolume((int)std::get<int64_t>(val));
		}
		break;
	}
	case str2int16("position"):
	{
		if (std::holds_alternative<Int64Tuple2>(val) == true)
		{
			game.Position(VarUtils::getVector2iVal<sf::Vector2i>(std::get<Int64Tuple2>(val)));
		}
		break;
	}
	case str2int16("refSize"):
	{
		if (std::holds_alternative<Int64Tuple2>(val) == true)
		{
			game.RefSize(VarUtils::getVector2uVal<sf::Vector2u>(std::get<Int64Tuple2>(val)));
		}
		break;
	}
	case str2int16("size"):
	{
		if (std::holds_alternative<Int64Tuple2>(val) == true)
		{
			game.Size(VarUtils::getVector2uVal<sf::Vector2u>(std::get<Int64Tuple2>(val)));
		}
		break;
	}
	case str2int16("smoothScreen"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			game.SmoothScreen(std::get<bool>(val));
		}
		break;
	}
	case str2int16("soundVolume"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			game.SoundVolume((int)std::get<int64_t>(val));
		}
		break;
	}
	case str2int16("stretchToFit"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			game.StretchToFit(std::get<bool>(val));
		}
		break;
	}
	case str2int16("title"):
	{
		if (std::holds_alternative<std::string>(val) == true)
		{
			game.Title(std::get<std::string>(val));
		}
		break;
	}
	case str2int16("version"):
	{
		if (std::holds_alternative<std::string>(val) == true)
		{
			game.Version(std::get<std::string>(val));
		}
		break;
	}
	default:
		break;
	}
}

QueryObject GameQueryable::getQueryable(const Game& game, const std::string_view prop)
{
	if (prop.empty() == true)
	{
		return &game;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return game.getQueryable(props.first, props.second);
}

QueryObject GameQueryable::getQueryable(const Game& game, const std::string_view prop1, const std::string_view prop2)
{
	if (prop1.empty() == true || prop1 == "game")
	{
		return &game;
	}
	std::shared_ptr<Queryable> queryable;
	if (prop1 == "currentLevel")
	{
		queryable = game.Resources().getCurrentLevel<Queryable>();
	}
	else if (prop1 == "focus")
	{
		queryable = game.Resources().getFocused();
	}
	else
	{
		queryable = game.Resources().getDrawableSharedPtr<Queryable>(prop1);
	}
	if (queryable != nullptr && prop2.empty() == false)
	{
		return queryable->getQueryable(prop2);
	}
	return queryable;
}

std::vector<VarOrQueryObject> GameQueryable::getQueryableList(const Game& game, const std::string_view prop)
{
	std::vector<VarOrQueryObject> queryableList;
	if (prop.empty() == false)
	{
		auto props = Utils::splitStringIn2(prop, '.');
		game.getQueryableList(props.first, props.second, queryableList);
	}
	return queryableList;
}

bool GameQueryable::getQueryableList(const std::string_view prop1,
	const std::string_view prop2, std::vector<VarOrQueryObject>& queryableList)
{
	if (prop1.empty() == false)
	{
		if (prop1 == "game")
		{
			auto props = Utils::splitStringIn2(prop2, '.');
			if (props.first == "saveDirs")
			{
				for (const auto& dir : FileUtils::getSaveDirList())
				{
					queryableList.push_back({ dir });
				}
				return true;
			}
			else if (props.first == "dirs")
			{
				for (const auto& dir : FileUtils::geDirList(props.second, ""))
				{
					queryableList.push_back({ dir });
				}
				return true;
			}
			else if (props.first == "files")
			{
				for (const auto& dir : FileUtils::getFileList(props.second, "", false))
				{
					queryableList.push_back({ dir });
				}
				return true;
			}
		}
	}
	return false;
}
