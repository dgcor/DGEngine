#include "Game.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseVariable.h"
#include "ReverseIterable.h"
#include "SFMLUtils.h"
#include "Utils.h"

Game::~Game()
{
	resourceManager = ResourceManager();
	if (window.isOpen() == true)
	{
		window.close();
	}
}

void Game::init()
{
	if (window.isOpen() == true)
	{
		return;
	}
#ifdef __ANDROID__
	window.create(sf::VideoMode::getDesktopMode(), title);
	size = window.getSize();
	oldSize = size;
#else
	oldSize = size;
	window.create(sf::VideoMode(size.x, size.y), title);
#endif
	if (framerate > 0)
	{
		window.setFramerateLimit(framerate);
	}
	if (stretchToFit == true)
	{
		stretchToFit = false;
		StretchToFit(true);
	}
	updateWindowTex();
}

void Game::play()
{
	if (window.isOpen() == false)
	{
		return;
	}

	updateMouse();
	sf::Clock frameClock;

	while (window.isOpen() == true)
	{
		mouseClicked = false;
		mouseDoubleClicked = false;
		mouseMoved = false;
		mouseReleased = false;
		mouseScrolled = false;
		keyboardChar = 0;
		keyPressed.code = sf::Keyboard::Unknown;

		processEvents();
		checkKeyPress();

		window.clear();
		windowTex.clear();

		elapsedTime = frameClock.restart();

		updateEvents();

		resourceManager.clearFinishedSounds();

		if (drawLoadingScreen() == false)
		{
			drawAndUpdate();
			drawCursor();
			drawFadeEffect();
			drawWindow();
		}
	}
}

void Game::processEvents()
{
	sf::Event evt;
	while (window.pollEvent(evt))
	{
		switch (evt.type)
		{
		case sf::Event::Closed:
			onClosed();
			break;
		case sf::Event::Resized:
			onResized(evt.size);
			break;
		case sf::Event::LostFocus:
			onLostFocus();
			break;
		case sf::Event::GainedFocus:
			onGainedFocus();
			break;
		case sf::Event::TextEntered:
			onTextEntered(evt.text);
			break;
		case sf::Event::KeyPressed:
			onKeyPressed(evt.key);
			break;
		case sf::Event::MouseWheelScrolled:
			onMouseWheelScrolled(evt.mouseWheelScroll);
			break;
		case sf::Event::MouseButtonPressed:
			onMouseButtonPressed(evt.mouseButton);
			break;
		case sf::Event::MouseButtonReleased:
			onMouseButtonReleased(evt.mouseButton);
			break;
		case sf::Event::MouseMoved:
			onMouseMoved(evt.mouseMove);
			break;
#ifdef __ANDROID__
		case sf::Event::TouchBegan:
			onTouchBegan(evt.touch);
			break;
		case sf::Event::TouchMoved:
			onTouchMoved(evt.touch);
			break;
		case sf::Event::TouchEnded:
			onTouchEnded(evt.touch);
			break;
#endif
		default:
			break;
		}
	}
}

void Game::onClosed()
{
	window.close();
}

void Game::onResized(const sf::Event::SizeEvent& evt)
{
	bool needsResize = false;
	sf::Vector2u newSize(evt.width, evt.height);
	if (newSize.x < minSize.x)
	{
		newSize.x = minSize.x;
		needsResize = true;
	}
	if (newSize.y < minSize.y)
	{
		newSize.y = minSize.y;
		needsResize = true;
	}
	if (needsResize == true)
	{
		window.setSize(newSize);
		return;
	}
	oldSize = size;
	size = newSize;

	updateWindowTex();

	auto view = window.getView();
	if (stretchToFit == false)
	{
		view.reset(sf::FloatRect(0.f, 0.f, (float)evt.width, (float)evt.height));
	}
	else
	{
		if (keepAR == true)
		{
			SFMLUtils::viewStretchKeepAR(view, newSize);
		}
		else
		{
			view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
		}
	}
	window.setView(view);
	updateSize();
}

void Game::updateSize()
{
	if (loadingScreen != nullptr)
	{
		loadingScreen->updateSize(*this);
	}
	if (fadeInOut != nullptr)
	{
		fadeInOut->updateSize(*this);
	}
	for (auto& res : resourceManager)
	{
		for (auto& obj : res.drawables)
		{
			obj.second->updateSize(*this);
		}
	}
}

void Game::updateWindowTex()
{
	if (stretchToFit == false)
	{
		windowTex.create(size.x, size.y);
		windowTexSize = size;
	}
	else
	{
		windowTex.create(minSize.x, minSize.y);
		windowTexSize = minSize;
	}
	windowTex.setSmooth(smoothScreen);
	windowSprite.setTexture(windowTex.getTexture(), true);
}

void Game::onLostFocus()
{
	paused = (pauseOnFocusLoss == true);
}

void Game::onGainedFocus()
{
	paused = false;
}

void Game::onTextEntered(const sf::Event::TextEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	if (evt.unicode < 256)
	{
		keyboardChar = static_cast<char>(evt.unicode);
	}
}

void Game::onKeyPressed(const sf::Event::KeyEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	keyPressed = evt;
#ifdef __ANDROID__
	keyPressed.system = false;
#endif
}

void Game::onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	updateMouse();
	mouseWheel = evt;
	mouseWheel.x = mousePositioni.x;
	mouseWheel.y = mousePositioni.y;
	mouseScrolled = true;
}

void Game::onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	mouseButton = evt.button;
	mouseClicked = true;
	mousePressed = true;

	auto mouseClickElapsedTime = mouseClickClock.restart();
	if (mouseClickElapsedTime.asMilliseconds() <= GameUtils::DoubleClickDelay)
	{
		mouseDoubleClicked = true;
	}
}

void Game::onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	mouseButton = evt.button;
	mousePressed = false;
	mouseReleased = true;
}

void Game::onMouseMoved(const sf::Event::MouseMoveEvent& evt)
{
	updateMouse(sf::Vector2i(evt.x, evt.y));
	if (disableInput == true)
	{
		return;
	}
	mouseMoved = true;
}
#ifdef __ANDROID__
void Game::onTouchBegan(const sf::Event::TouchEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	updateMouse(sf::Vector2i(evt.x, evt.y));
	sf::Event::MouseButtonEvent mouseEvt;
	mouseEvt.button = sf::Mouse::Left;
	mouseEvt.x = evt.x;
	mouseEvt.y = evt.y;
	onMouseButtonPressed(mouseEvt);
}

void Game::onTouchMoved(const sf::Event::TouchEvent& evt)
{
	updateMouse(sf::Vector2i(evt.x, evt.y));
	if (disableInput == true)
	{
		return;
	}
	mouseMoved = true;
}

void Game::onTouchEnded(const sf::Event::TouchEvent& evt)
{
	if (disableInput == true)
	{
		return;
	}
	updateMouse(sf::Vector2i(evt.x, evt.y));
	sf::Event::MouseButtonEvent mouseEvt;
	mouseEvt.button = sf::Mouse::Left;
	mouseEvt.x = evt.x;
	mouseEvt.y = evt.y;
	onMouseButtonReleased(mouseEvt);
}
#endif
void Game::updateMouse()
{
	updateMouse(sf::Mouse::getPosition(window));
}

void Game::updateMouse(const sf::Vector2i mousePos)
{
	mousePositionf = window.mapPixelToCoords(mousePos);
	mousePositionf.x = std::round(mousePositionf.x);
	mousePositionf.y = std::round(mousePositionf.y);
	mousePositioni.x = (int)mousePositionf.x;
	mousePositioni.y = (int)mousePositionf.y;
	updateCursorPosition();
}

void Game::updateCursorPosition()
{
	auto cursor = resourceManager.getCursor();
	if (cursor != nullptr)
	{
		cursor->Position(mousePositionf);
	}
}

void Game::checkKeyPress()
{
	if (loadingScreen == nullptr && keyPressed.code != sf::Keyboard::Unknown)
	{
		auto action = resourceManager.getKeyboardAction(keyPressed);
		if (action != nullptr)
		{
			eventManager.addBack(action);
		}
	}
}

void Game::updateEvents()
{
	if (paused == false)
	{
		eventManager.update(*this);
	}
}

bool Game::drawLoadingScreen()
{
	if (loadingScreen == nullptr)
	{
		return false;
	}
	windowTex.draw(*loadingScreen);
	drawFadeEffect();
	drawWindow();
	return true;
}

void Game::drawAndUpdate()
{
	for (auto& res : reverse(resourceManager))
	{
		if (res.ignore != IgnoreResource::DrawAndUpdate)
		{
			for (auto it2 = res.drawables.rbegin(); it2 != res.drawables.rend(); ++it2)
			{
				auto& obj = *(it2);
				if (paused == false && res.ignore != IgnoreResource::Update)
				{
					obj.second->update(*this);
				}
			}
		}
	}
	for (auto& res : resourceManager)
	{
		if (res.ignore != IgnoreResource::DrawAndUpdate)
		{
			for (auto& obj : res.drawables)
			{
				windowTex.draw(*obj.second);
			}
		}
	}
}

void Game::drawCursor()
{
	auto cursor = resourceManager.getCursor();
	if (cursor != nullptr)
	{
		cursor->update(*this);
		windowTex.draw(*cursor);
	}
}

void Game::drawFadeEffect()
{
	if (fadeInOut != nullptr)
	{
		windowTex.draw(static_cast<sf::RectangleShape>(*fadeInOut));
		fadeInOut->update(*this);
	}
}

void Game::drawWindow()
{
	windowTex.display();
	window.draw(windowSprite);
	window.display();
}

void Game::SmoothScreen(bool smooth_)
{
	smoothScreen = smooth_;
	windowTex.setSmooth(smooth_);
}

void Game::StretchToFit(bool stretchToFit_)
{
	if (stretchToFit != stretchToFit_)
	{
		stretchToFit = stretchToFit_;
		if (window.isOpen() == true)
		{
			if (stretchToFit_ == true)
			{
				auto view = window.getView();
				view.reset(sf::FloatRect(0, 0, (float)minSize.x, (float)minSize.y));
				window.setView(view);
				oldSize = size;
				size = minSize;
				stretchToFit = false;
				updateSize();
				size = oldSize;
				stretchToFit = true;
				if (keepAR == true)
				{
					SFMLUtils::viewStretchKeepAR(view, size);
				}
				else
				{
					view.setViewport(sf::FloatRect(0, 0, 1, 1));
				}
				window.setView(view);
			}
			else
			{
				auto view = window.getView();
				view.setViewport(sf::FloatRect(0, 0, 1, 1));
				window.setView(view);
				oldSize = minSize;
				updateSize();
				view.reset(sf::FloatRect(0, 0, (float)size.x, (float)size.y));
				window.setView(view);
				updateWindowTex();
			}
			updateMouse();
		}
	}
}

void Game::KeepAR(bool keepAR_)
{
	if (keepAR != keepAR_)
	{
		if (window.isOpen() == true && stretchToFit == true)
		{
			auto view = window.getView();
			if (keepAR_ == true)
			{
				SFMLUtils::viewStretchKeepAR(view, size);
			}
			else
			{
				view.setViewport(sf::FloatRect(0, 0, 1, 1));
			}
			window.setView(view);
			updateSize();
			updateMouse();
		}
		keepAR = keepAR_;
	}
}

void Game::addPlayingSound(const sf::SoundBuffer& obj)
{
	sf::Sound sound(obj);
	sound.setVolume((float)soundVolume);
	resourceManager.addPlayingSound(sound);
}

void Game::addPlayingSound(const sf::SoundBuffer* obj)
{
	if (obj != nullptr)
	{
		addPlayingSound(*obj);
	}
}

bool Game::getVariableNoPercentage(const std::string& key, Variable& var) const
{
	auto it = variables.find(key);
	if (it != variables.cend())
	{
		var = it->second;
		return true;
	}
	return false;
}

bool Game::getVariable(const std::string& key, Variable& var) const
{
	auto it = findVariable(key);
	if (it != variables.cend())
	{
		var = it->second;
		return true;
	}
	return false;
}

std::map<std::string, Variable>::const_iterator Game::findVariable(const std::string& key) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		return variables.find(key.substr(1, key.size() - 2));
	}
	return variables.end();
}

bool Game::getVarOrProp(const std::string& key, Variable& var) const
{
	if (getVariable(key, var) == true)
	{
		return true;
	}
	return GameUtils::getObjectProperty(*this, key, var);
}

Variable Game::getVarOrProp(const Variable& var) const
{
	if (var.is<std::string>())
	{
		Variable var2;
		if (getVarOrProp(var.get<std::string>(), var2) == true)
		{
			return var2;
		}
	}
	return var;
}

bool Game::getVarOrPropBool(const std::string& key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toBool(var);
	}
	return false;
}

bool Game::getVarOrPropBool(const Variable& var) const
{
	if (var.is<std::string>())
	{
		return getVarOrPropBool(var.get<std::string>());
	}
	return VarUtils::toBool(var);
}

double Game::getVarOrPropDouble(const std::string& key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toDouble(var);
	}
	return 0.0;
}

double Game::getVarOrPropDouble(const Variable& var) const
{
	if (var.is<std::string>())
	{
		return getVarOrPropDouble(var.get<std::string>());
	}
	return VarUtils::toDouble(var);
}

int64_t Game::getVarOrPropLong(const std::string& key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toLong(var);
	}
	return 0;
}

int64_t Game::getVarOrPropLong(const Variable& var) const
{
	if (var.is<std::string>())
	{
		return getVarOrPropLong(var.get<std::string>());
	}
	return VarUtils::toLong(var);
}

std::string Game::getVarOrPropString(const std::string& key) const
{
	Variable var;
	if (getVariable(key, var) == true)
	{
		if (var.is<std::string>())
		{
			GameUtils::getObjectProperty(*this, var.get<std::string>(), var);
		}
	}
	else if (GameUtils::getObjectProperty(*this, key, var) == false)
	{
		return key;
	}
	return VarUtils::toString(var);
}

std::string Game::getVarOrPropString(const Variable& var) const
{
	if (var.is<std::string>())
	{
		return getVarOrPropString(var.get<std::string>());
	}
	return VarUtils::toString(var);
}

void Game::clearVariable(const std::string& key)
{
	if (key.size() > 1)
	{
		if ((key.front() == '%') && (key.back() == '%'))
		{
			auto it = variables.find(key.substr(1, key.size() - 2));
			if (it != variables.end())
			{
				variables.erase(it);
			}
		}
	}
}

void Game::setVariable(const std::string& key, const Variable& value)
{
	variables[key] = value;
}

void Game::saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const
{
	if (varNames.empty() == true)
	{
		return;
	}
	std::vector<std::pair<std::string, Variable>> variablesToSave;
	for (const auto& name : varNames)
	{
		auto it = variables.find(name);
		if (it != variables.end())
		{
			variablesToSave.push_back(*it);
		}
	}
	if (variablesToSave.empty() == false)
	{
		JsonUtils::saveToFile<decltype(variablesToSave)>(filePath, "variable", variablesToSave);
	}
}

bool Game::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first.c_str()))
	{
	case str2int16("framerate"):
		var = Variable((int64_t)framerate);
		break;
	case str2int16("hasEvent"):
	{
		var = Variable((bool)eventManager.exists(props.second));
		break;
	}
	case str2int16("hasResource"):
	{
		var = Variable((bool)resourceManager.resourceExists(props.second));
		break;
	}
	case str2int16("keepAR"):
		var = Variable((bool)keepAR);
		break;
	case str2int16("minSize"):
	{
		if (props.second == "x")
		{
			var = Variable((int64_t)minSize.x);
		}
		else
		{
			var = Variable((int64_t)minSize.y);
		}
		break;
	}
	case str2int16("musicVolume"):
		var = Variable((int64_t)musicVolume);
		break;
	case str2int16("path"):
		var = Variable(path);
		break;
	case str2int16("refSize"):
	{
		if (props.second == "x")
		{
			var = Variable((int64_t)refSize.x);
		}
		else
		{
			var = Variable((int64_t)refSize.y);
		}
		break;
	}
	case str2int16("saveDir"):
		var = Variable(std::string(FileUtils::getSaveDir()));
		break;
	case str2int16("size"):
	{
		if (props.second == "x")
		{
			var = Variable((int64_t)size.x);
		}
		else
		{
			var = Variable((int64_t)size.y);
		}
		break;
	}
	case str2int16("smoothScreen"):
		var = Variable((bool)smoothScreen);
		break;
	case str2int16("soundVolume"):
		var = Variable((int64_t)soundVolume);
		break;
	case str2int16("stretchToFit"):
		var = Variable((bool)stretchToFit);
		break;
	case str2int16("title"):
		var = Variable(title);
		break;
	case str2int16("version"):
		var = Variable(version);
		break;
	default:
		return false;
	}
	return true;
}

void Game::setProperty(const std::string& prop, const Variable& val)
{
	if (prop.size() <= 1)
	{
		return;
	}
	switch (str2int16(prop.c_str()))
	{
	case str2int16("framerate"):
	{
		if (val.is<int64_t>() == true)
		{
			Framerate((unsigned)val.get<int64_t>());
		}
	}
	break;
	case str2int16("keepAR"):
	{
		if (val.is<bool>() == true)
		{
			KeepAR(val.get<bool>());
		}
	}
	break;
	case str2int16("musicVolume"):
	{
		if (val.is<int64_t>() == true)
		{
			MusicVolume((unsigned)val.get<int64_t>());
		}
	}
	break;
	case str2int16("smoothScreen"):
	{
		if (val.is<bool>() == true)
		{
			SmoothScreen(val.get<bool>());
		}
	}
	break;
	case str2int16("soundVolume"):
	{
		if (val.is<int64_t>() == true)
		{
			SoundVolume((unsigned)val.get<int64_t>());
		}
	}
	break;
	case str2int16("stretchToFit"):
	{
		if (val.is<bool>() == true)
		{
			StretchToFit(val.get<bool>());
		}
	}
	break;
	case str2int16("title"):
	{
		if (val.is<std::string>() == true)
		{
			setTitle(val.get<std::string>());
		}
	}
	break;
	case str2int16("version"):
		if (val.is<std::string>() == true)
		{
			setVersion(val.get<std::string>());
		}
	}
}

const Queryable* Game::getQueryable(const std::string& prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == true ||
		props.first == "game")
	{
		return this;
	}
	const Queryable* queryable = resourceManager.getDrawable(props.first);
	if (queryable == nullptr)
	{
		if (props.first == "focus")
		{
			queryable = resourceManager.getFocused();
		}
		else if (props.first == "currentLevel")
		{
			queryable = resourceManager.getCurrentLevel();
		}
	}
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryable;
}
