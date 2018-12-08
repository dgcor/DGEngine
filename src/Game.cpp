#include "Game.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseVariable.h"
#include "SFML/SFMLUtils.h"
#include "Shaders.h"
#include "Utils/ReverseIterable.h"
#include "Utils/Utils.h"

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
	Shaders::init();

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
		processEvents();

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
	mousePressed = false;
	mouseReleased = false;
	mouseMoved = false;
	mouseScrolled = false;
	keyPressed = false;
	textEntered = false;

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
		case sf::Event::TouchBegan:
			onTouchBegan(evt.touch);
			break;
		case sf::Event::TouchMoved:
			onTouchMoved(evt.touch);
			break;
		case sf::Event::TouchEnded:
			onTouchEnded(evt.touch);
			break;
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
		for (auto obj : res.drawables)
		{
			obj->updateSize(*this);
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

void Game::onLostFocus() noexcept
{
	paused = (pauseOnFocusLoss == true);
}

void Game::onGainedFocus() noexcept
{
	paused = false;
}

void Game::onTextEntered(const sf::Event::TextEvent& evt) noexcept
{
	if (enableInput == false)
	{
		return;
	}
	textEnteredEvt = evt;
	textEntered = true;
}

void Game::onKeyPressed(const sf::Event::KeyEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	keyPressEvt = evt;
	keyPressed = true;
#ifdef __ANDROID__
	keyPressEvt.system = false;
#endif
	if (loadingScreen != nullptr)
	{
		return;
	}
	auto action = resourceManager.getKeyboardAction(keyPressEvt);
	if (action != nullptr)
	{
		eventManager.addBack(action);
	}
}

void Game::onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mouseScrollEvt = evt;
	mouseScrolled = true;
	updateMouse();
	mouseScrollEvt.x = mousePositioni.x;
	mouseScrollEvt.y = mousePositioni.y;
}

void Game::onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mousePressEvt = evt;
	mousePressed = true;
	updateMouse();
	mousePressEvt.x = mousePositioni.x;
	mousePressEvt.y = mousePositioni.y;
}

void Game::onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mouseReleaseEvt = evt;
	mouseReleased = true;
	updateMouse();
	mouseReleaseEvt.x = mousePositioni.x;
	mouseReleaseEvt.y = mousePositioni.y;
}

void Game::onMouseMoved(const sf::Event::MouseMoveEvent& evt)
{
	updateMouse(sf::Vector2i(evt.x, evt.y));
	if (enableInput == false)
	{
		return;
	}
	mouseMoveEvt = evt;
	mouseMoved = true;
	mouseMoveEvt.x = mousePositioni.x;
	mouseMoveEvt.y = mousePositioni.y;
}

void Game::onTouchBegan(const sf::Event::TouchEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	touchBeganEvt = evt;
	touchBegan = true;
	updateMouse();
	touchBeganEvt.x = mousePositioni.x;
	touchBeganEvt.y = mousePositioni.y;
}

void Game::onTouchMoved(const sf::Event::TouchEvent& evt)
{
	updateMouse(sf::Vector2i(evt.x, evt.y));
	if (enableInput == false)
	{
		return;
	}
	touchMovedEvt = evt;
	touchMoved = true;
	touchMovedEvt.x = mousePositioni.x;
	touchMovedEvt.y = mousePositioni.y;
}

void Game::onTouchEnded(const sf::Event::TouchEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	touchEndedEvt = evt;
	touchEnded = true;
	updateMouse();
	touchEndedEvt.x = mousePositioni.x;
	touchEndedEvt.y = mousePositioni.y;
}

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

void Game::update()
{
	for (auto& res : reverse(resourceManager))
	{
		if (((int)res.ignore & (int)IgnoreResource::Update) == 0)
		{
			for (auto obj : reverse(res.drawables))
			{
				if (paused == false && res.ignore != IgnoreResource::Update)
				{
					obj->update(*this);
				}
			}
		}
		else if (((int)res.ignore & (int)IgnoreResource::All) != 0)
		{
			break;
		}
	}
}

void Game::drawUI()
{
	for (auto& res : resourceManager)
	{
		if (((int)res.ignore & (int)IgnoreResource::Draw) == 0)
		{
			for (auto obj : res.drawables)
			{
				windowTex.draw(*obj);
			}
		}
		else if (((int)res.ignore & (int)IgnoreResource::All) != 0)
		{
			break;
		}
	}
}

void Game::drawAndUpdate()
{
	update();
	drawUI();
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
	sf::RenderStates states = sf::RenderStates::Default;
	if (Shaders::supportsGamma() &&
		gamma > 30 && gamma <= 100)
	{
		states.shader = &Shaders::Gamma;
		Shaders::Gamma.setUniform("gamma", (float)(130 - gamma) * 0.01f);
	}
	window.draw(windowSprite, states);
	window.display();
}

void Game::draw()
{
	drawUI();
	drawCursor();
	drawFadeEffect();
	drawWindow();
}

void Game::clearInputEvents(InputEvent e) noexcept
{
	if (e == InputEvent::None)
	{
		return;
	}
	else if (e == InputEvent::All)
	{
		mousePressed = false;
		mouseReleased = false;
		mouseMoved = false;
		mouseScrolled = false;
		keyPressed = false;
		textEntered = false;
		touchBegan = false;
		touchMoved = false;
		touchEnded = false;
		return;
	}
	if (mousePressed == true &&
		((int)e & (int)InputEvent::MousePress) != 0)
	{
		if (((int)e & (int)InputEvent::LeftClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Left)
		{
			mousePressed = false;
		}
		else if (((int)e & (int)InputEvent::MiddleClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Middle)
		{
			mousePressed = false;
		}
		else if (((int)e & (int)InputEvent::RightClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Right)
		{
			mousePressed = false;
		}
	}
	if (mouseReleased == true &&
		((int)e & (int)InputEvent::MouseRelease) != 0)
	{
		if (((int)e & (int)InputEvent::LeftClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Left)
		{
			mouseReleased = false;
		}
		else if (((int)e & (int)InputEvent::MiddleClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Middle)
		{
			mouseReleased = false;
		}
		else if (((int)e & (int)InputEvent::RightClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Right)
		{
			mouseReleased = false;
		}
	}
	if (mouseMoved == true &&
		((int)e & (int)InputEvent::MouseMove) != 0)
	{
		mouseMoved = false;
	}
	if (mouseScrolled == true &&
		((int)e & (int)InputEvent::MouseScroll) != 0)
	{
		mouseScrolled = false;
	}
	if (keyPressed == true &&
		((int)e & (int)InputEvent::KeyPress) != 0)
	{
		keyPressed = false;
	}
	if (textEntered == true &&
		((int)e & (int)InputEvent::TextEnter) != 0)
	{
		textEntered = false;
	}
	if (touchBegan == true &&
		((int)e & (int)InputEvent::TouchBegin) != 0)
	{
		touchBegan = false;
	}
	if (touchMoved == true &&
		((int)e & (int)InputEvent::TouchMove) != 0)
	{
		touchMoved = false;
	}
	if (touchEnded == true &&
		((int)e & (int)InputEvent::TouchEnd) != 0)
	{
		touchEnded = false;
	}
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

bool Game::getVariableNoToken(const std::string& key, Variable& var) const
{
	auto it = variables.find(key);
	if (it != variables.cend())
	{
		var = it->second;
		return true;
	}
	return false;
}

bool Game::getVarOrPropNoToken(const std::string& key, Variable& var) const
{
	if (getVariableNoToken(key, var) == true)
	{
		return true;
	}
	return GameUtils::getObjectProperty(*this, key, var);
}

bool Game::getVarOrProp(const std::string_view key, Variable& var) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		if (getVariableNoToken(std::string(key2), var) == true)
		{
			return true;
		}
		return GameUtils::getObjectProperty(*this, key2, var);
	}
	return false;
}

Variable Game::getVarOrProp(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		Variable var2;
		if (getVarOrProp(std::get<std::string>(var), var2) == true)
		{
			return var2;
		}
	}
	return var;
}

bool Game::getVarOrPropBoolS(const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toBool(var);
	}
	return false;
}

bool Game::getVarOrPropBoolV(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropBoolS(std::get<std::string>(var));
	}
	return VarUtils::toBool(var);
}

double Game::getVarOrPropDoubleS(const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toDouble(var);
	}
	return 0.0;
}

double Game::getVarOrPropDoubleV(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropDoubleS(std::get<std::string>(var));
	}
	return VarUtils::toDouble(var);
}

int64_t Game::getVarOrPropLongS(const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toLong(var);
	}
	return 0;
}

int64_t Game::getVarOrPropLongV(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropLongS(std::get<std::string>(var));
	}
	return VarUtils::toLong(var);
}

std::string Game::getVarOrPropStringS(const std::string_view key) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		Variable var;
		if (getVariableNoToken(std::string(key2), var) == true)
		{
			if (std::holds_alternative<std::string>(var))
			{
				auto key3 = std::get<std::string>(var).substr(1, std::get<std::string>(var).size() - 2);
				GameUtils::getObjectProperty(*this, key3, var);
			}
			return VarUtils::toString(var);
		}
		else if (GameUtils::getObjectProperty(*this, key2, var) == true)
		{
			return VarUtils::toString(var);
		}
	}
	return std::string(key);
}

std::string Game::getVarOrPropStringV(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropStringS(std::get<std::string>(var));
	}
	return VarUtils::toString(var);
}

void Game::clearVariable(const std::string& key)
{
	auto it = variables.find(key);
	if (it != variables.end())
	{
		variables.erase(it);
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

bool Game::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first))
	{
	case str2int16("framerate"):
		var = Variable((int64_t)framerate);
		break;
	case str2int16("gamma"):
		var = Variable((int64_t)gamma);
		break;
	case str2int16("hasDrawable"):
		var = Variable(resourceManager.hasDrawable(std::string(props.second)));
		break;
	case str2int16("hasEvent"):
		var = Variable(eventManager.exists(props.second));
		break;
	case str2int16("hasResource"):
		var = Variable(resourceManager.resourceExists(props.second));
		break;
	case str2int16("hasFont"):
		var = Variable(resourceManager.hasFont(std::string(props.second)));
		break;
	case str2int16("hasTexture"):
		var = Variable(resourceManager.hasTexture(std::string(props.second)));
		break;
	case str2int16("hasAudio"):
		var = Variable(resourceManager.hasAudioSource(std::string(props.second)));
		break;
	case str2int16("hasSong"):
		var = Variable(resourceManager.hasSong(std::string(props.second)));
		break;
	case str2int16("hasPalette"):
		var = Variable(resourceManager.hasPalette(std::string(props.second)));
		break;
	case str2int16("hasImageContainer"):
		var = Variable(resourceManager.hasImageContainer(std::string(props.second)));
		break;
	case str2int16("hasTexturePack"):
		var = Variable(resourceManager.hasTexturePack(std::string(props.second)));
		break;
	case str2int16("keepAR"):
		var = Variable(keepAR);
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
	case str2int16("openGL"):
	{
		switch (str2int16(props.second))
		{
		case str2int16("antialiasingLevel"):
			var = Variable((int64_t)getOpenGLAntialiasingLevel());
			break;
		case str2int16("depthBits"):
			var = Variable((int64_t)getOpenGLDepthBits());
			break;
		case str2int16("hasGeometryShaders"):
			var = Variable(sf::Shader::isGeometryAvailable());
			break;
		case str2int16("hasShaders"):
			var = Variable(sf::Shader::isAvailable());
			break;
		case str2int16("majorVersion"):
			var = Variable((int64_t)getOpenGLMajorVersion());
			break;
		case str2int16("minorVersion"):
			var = Variable((int64_t)getOpenGLMinorVersion());
			break;
		case str2int16("sRgbCapable"):
			var = Variable(getOpenGLSRgbCapable());
			break;
		case str2int16("stencilBits"):
			var = Variable((int64_t)getOpenGLStencilBits());
			break;
		default:
			return false;
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
		var = Variable(smoothScreen);
		break;
	case str2int16("soundVolume"):
		var = Variable((int64_t)soundVolume);
		break;
	case str2int16("stretchToFit"):
		var = Variable(stretchToFit);
		break;
	case str2int16("supportsGamma"):
		var = Variable(Shaders::supportsGamma());
		break;
	case str2int16("supportsLighting"):
		var = Variable((bool)(Shaders::supportsPalettes() | Shaders::supportsLighting()));
		break;
	case str2int16("supportsOutlines"):
		var = Variable(Shaders::supportsOutlines());
		break;
	case str2int16("supportsPalettes"):
		var = Variable(Shaders::supportsPalettes());
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

void Game::setProperty(const std::string_view prop, const Variable& val)
{
	if (prop.size() <= 1)
	{
		return;
	}
	switch (str2int16(prop))
	{
	case str2int16("framerate"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			Framerate((unsigned)std::get<int64_t>(val));
		}
	}
	break;
	case str2int16("gamma"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			Gamma((unsigned)std::get<int64_t>(val));
		}
	}
	break;
	case str2int16("keepAR"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			KeepAR(std::get<bool>(val));
		}
	}
	break;
	case str2int16("musicVolume"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			MusicVolume((unsigned)std::get<int64_t>(val));
		}
	}
	break;
	case str2int16("smoothScreen"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			SmoothScreen(std::get<bool>(val));
		}
	}
	break;
	case str2int16("soundVolume"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			SoundVolume((unsigned)std::get<int64_t>(val));
		}
	}
	break;
	case str2int16("stretchToFit"):
	{
		if (std::holds_alternative<bool>(val) == true)
		{
			StretchToFit(std::get<bool>(val));
		}
	}
	break;
	case str2int16("title"):
	{
		if (std::holds_alternative<std::string>(val) == true)
		{
			setTitle(std::get<std::string>(val));
		}
	}
	break;
	case str2int16("version"):
		if (std::holds_alternative<std::string>(val) == true)
		{
			setVersion(std::get<std::string>(val));
		}
	}
}

const Queryable* Game::getQueryable(const std::string_view prop) const
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
	const Queryable* queryable = resourceManager.getDrawable(std::string(props.first));
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
