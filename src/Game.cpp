#include "Game.h"
#include "FileUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/Parser.h"
#include "Parser/ParseVariable.h"
#include "SFML/SFMLUtils.h"
#include "Utils/ReverseIterable.h"
#include "Utils/Utils.h"

Game::~Game()
{
	resourceManager = {};
	if (window.isOpen() == true)
	{
		window.close();
	}
}

void Game::reset()
{
	refSize = { 640, 480 };
	minSize = { 640, 480 };
	size = { 640, 480 };
	oldSize = {};
	drawRegionSize = {};
	framerate = { 0 };
	smoothScreen = { false };
	stretchToFit = { false };
	keepAR = { true };
	enableInput = { true };
	pauseOnFocusLoss = { false };
	paused = { false };

	mousePositioni = {};
	mousePositionf = {};

	mousePressEvt = {};
	mouseReleaseEvt = {};
	mouseMoveEvt = {};
	mouseScrollEvt = {};
	keyPressEvt = {};
	textEnteredEvt = {};
	touchBeganEvt = {};
	touchMovedEvt = {};
	touchEndedEvt = {};
	mousePressed = { false };
	mouseReleased = { false };
	mouseMoved = { false };
	mouseScrolled = { false };
	keyPressed = { false };
	textEntered = { false };
	touchBegan = { false };
	touchMoved = { false };
	touchEnded = { false };

	musicVolume = { 100 };
	soundVolume = { 100 };
	gamma = { 0 };

	elapsedTime = {};
	totalElapsedTime = {};

	path = {};
	title = {};
	version = {};

	resourceManager = {};
	resourceManager.Shaders().init();
	resourceManager.Shaders().init(shaders);

	variables = {};

	loadingScreen = {};
	fadeObj = {};
}

void Game::load(const std::string_view gamefilePath, const std::string_view mainFile)
{
	if (window.isOpen() == true)
	{
		window.close();
	}
	reset();
	FileUtils::unmountAll();
	Parser::parseGame(*this, gamefilePath, mainFile);
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
	updateGameTexture();
}

void Game::WindowSize(const sf::Vector2u& size_)
{
	if (size_.x >= minSize.x && size_.y >= minSize.y)
	{
		if (window.isOpen() == true)
		{
			window.setSize(size_);
		}
		else
		{
			size = size_;
		}
	}
}

void Game::RefSize(const sf::Vector2u& size_)
{
	if (window.isOpen() == false &&
		size_.x > minSize.x &&
		size_.y > minSize.y)
	{
		refSize = size_;
	}
}

void Game::MinSize(const sf::Vector2u& size_)
{
	if (window.isOpen() == true)
	{
		return;
	}
	minSize = size_;
	if (size.x < minSize.x && size.y < minSize.y)
	{
		size = minSize;
	}
}

void Game::Framerate(int framerate_)
{
	if (framerate_ > 0)
	{
		framerate = (unsigned)std::clamp(framerate_, 30, 60);
	}
	else
	{
		framerate = 0;
	}
	if (window.isOpen() == true)
	{
		window.setFramerateLimit(framerate);
	}
}

void Game::setTitle(const std::string& title_)
{
	title = title_;
	if (window.isOpen() == true)
	{
		window.setTitle(title);
	}
}

void Game::play()
{
	if (window.isOpen() == false)
	{
		return;
	}

	updateMousePosition();
	sf::Clock frameClock;

	while (window.isOpen() == true)
	{
		processEvents();

		window.clear();
		gameTexture.clear();

		elapsedTime = frameClock.restart();
		totalElapsedTime += elapsedTime;

		updateEvents();

		resourceManager.clearFinishedSounds();

		if (drawLoadingScreen() == false)
		{
			drawAndUpdate();
			drawCursor();
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

	updateGameTexture();

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
	for (auto& res : resourceManager)
	{
		for (auto obj : res.drawables)
		{
			obj->updateSize(*this);
		}
	}
}

void Game::recreateRenderTexture(sf::RenderTexture& renderTexture) const
{
	renderTexture.create(drawRegionSize.x, drawRegionSize.y);
	renderTexture.setSmooth(smoothScreen);
}

void Game::updateGameTexture()
{
	if (stretchToFit == false)
	{
		drawRegionSize = size;
	}
	else
	{
		drawRegionSize = minSize;
	}
	recreateRenderTexture(gameTexture);
	gameSprite.setTexture(gameTexture.getTexture(), true);
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
	updateMousePosition();
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
	updateMousePosition();
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
	updateMousePosition();
	mouseReleaseEvt.x = mousePositioni.x;
	mouseReleaseEvt.y = mousePositioni.y;
}

void Game::onMouseMoved(const sf::Event::MouseMoveEvent& evt)
{
	updateMousePosition(sf::Vector2i(evt.x, evt.y));
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
	updateMousePosition();
	touchBeganEvt.x = mousePositioni.x;
	touchBeganEvt.y = mousePositioni.y;
}

void Game::onTouchMoved(const sf::Event::TouchEvent& evt)
{
	updateMousePosition(sf::Vector2i(evt.x, evt.y));
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
	updateMousePosition();
	touchEndedEvt.x = mousePositioni.x;
	touchEndedEvt.y = mousePositioni.y;
}

void Game::setMousePosition(sf::Vector2i mousePos)
{
	mousePos = window.mapCoordsToPixel({ (float)mousePos.x , (float)mousePos.y });
	sf::Mouse::setPosition(mousePos, window);
	updateMousePosition();
}

void Game::updateMousePosition()
{
	updateMousePosition(sf::Mouse::getPosition(window));
}

void Game::updateMousePosition(const sf::Vector2i mousePos)
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
	loadingScreen->draw(gameTexture);
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
				obj->draw(*this, gameTexture);
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
		cursor->draw(*this, gameTexture);
	}
}

void Game::drawWindow()
{
	gameTexture.display();
	sf::RenderStates states = sf::RenderStates::Default;
	if (shaders.hasGameShader() == true)
	{
		states.shader = shaders.Game;
		shaders.Game->setUniform("elapsedTime", totalElapsedTime.asSeconds());
		shaders.Game->setUniform("gamma", (float)gamma);
		shaders.Game->setUniform("fade", sf::Glsl::Vec4(fadeObj.getColor()));
		if (loadingScreen != nullptr)
		{
			shaders.Game->setUniform("loading", (float)loadingScreen->getProgress() / 100.f);
		}
		else
		{
			shaders.Game->setUniform("loading", 1.f);
		}
		shaders.Game->setUniform("mousePosition", sf::Glsl::Vec2(
			(float)std::clamp(mousePositioni.x, 0, (int)drawRegionSize.x) / (float)drawRegionSize.x,
			(float)std::clamp(mousePositioni.y, 0, (int)drawRegionSize.y) / (float)drawRegionSize.y
		));
		shaders.Game->setUniform("textureSize", sf::Glsl::Vec2(
			(float)drawRegionSize.x,
			(float)drawRegionSize.y
		));
	}
	fadeObj.update(*this);
	window.draw(gameSprite, states);
	window.display();
}

void Game::draw()
{
	drawUI();
	drawCursor();
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
	gameTexture.setSmooth(smooth_);
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
				updateGameTexture();
			}
			updateMousePosition();
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
			updateMousePosition();
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

bool Game::getVarOrPropNoToken(const std::string_view key, Variable& var) const
{
	if (getVariableNoToken(std::string(key), var) == true)
	{
		return true;
	}
	return getProperty(key, var);
}

bool Game::getVarOrProp(const std::string_view key, Variable& var) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		return getVarOrPropNoToken(key2, var);
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
				getProperty(key3, var);
			}
			return VarUtils::toString(var);
		}
		else if (getProperty(key2, var) == true)
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

std::shared_ptr<Action> Game::getQueryAction(const std::string_view prop) const
{
	auto idx = prop.rfind('.');
	if (idx > 0 && idx != std::string::npos)
	{
		auto query = prop.substr(0, idx);
		auto actionName = prop.substr(idx + 1);
		auto obj = getQueryable(query);
		if (obj != nullptr)
		{
			auto uiObj = dynamic_cast<const UIObject*>(obj);
			if (uiObj != nullptr)
			{
				return uiObj->getAction(str2int16(actionName));
			}
		}
	}
	return nullptr;
}

bool Game::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 2)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first))
	{
	case str2int16("$"):
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, *this));
		return true;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, *this));
		return true;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, *this));
		return true;
	case str2int16("$f"):
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, *this));
		return true;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, *this));
		return true;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, *this));
		return true;
	case str2int16("game"):
		break;
	default:
	{
		const UIObject* uiObject = nullptr;
		if (props.first == "currentLevel")
		{
			uiObject = resourceManager.getCurrentLevel();
		}
		else if (props.first == "focus")
		{
			uiObject = resourceManager.getFocused();
		}
		else
		{
			uiObject = resourceManager.getDrawable(std::string(props.first));
		}
		if (uiObject != nullptr)
		{
			return uiObject->getProperty(props.second, var);
		}
		return false;
	}
	}
	if (props.second.size() <= 1)
	{
		return false;
	}
	return getGameProperty(props.second, var);
}

bool Game::getGameProperty(const std::string_view prop, Variable& var) const
{
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first))
	{
	case str2int16("cursor"):
	{
		if (props.second == "x")
		{
			var = Variable((int64_t)mousePositioni.x);
		}
		else
		{
			var = Variable((int64_t)mousePositioni.y);
		}
		break;
	}
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
	case str2int16("hasShader"):
		var = Variable(resourceManager.Shaders().has(std::string(props.second)));
		break;
	case str2int16("hasGameShader"):
		var = Variable(shaders.hasGameShader());
		break;
	case str2int16("hasSpriteShader"):
		var = Variable(shaders.hasSpriteShader());
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
		var = Variable(FileUtils::getSaveDir());
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
	case str2int16("title"):
		var = Variable(title);
		break;
	case str2int16("var"):
		return getVariableNoToken(std::string(props.second), var);
	case str2int16("version"):
		var = Variable(version);
		break;
	default:
		return false;
	}
	return true;
}

void Game::setGameProperty(const std::string_view prop, const Variable& val)
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
			Framerate((int)std::get<int64_t>(val));
		}
	}
	break;
	case str2int16("gamma"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			Gamma((int)std::get<int64_t>(val));
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
			MusicVolume((int)std::get<int64_t>(val));
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
			SoundVolume((int)std::get<int64_t>(val));
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
	const Queryable* queryable = nullptr;
	if (props.first == "currentLevel")
	{
		queryable = resourceManager.getCurrentLevel();
	}
	else if (props.first == "focus")
	{
		queryable = resourceManager.getFocused();
	}
	else
	{
		queryable = resourceManager.getDrawable(std::string(props.first));
	}
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryable;
}

std::vector<std::variant<const Queryable*, Variable>> Game::getQueryableList(
	const std::string_view prop) const
{
	std::vector<std::variant<const Queryable*, Variable>> queriableList;

	if (prop.empty() == true)
	{
		return queriableList;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		if (props.first == "game")
		{
			auto props2 = Utils::splitStringIn2(props.second, '.');
			if (props2.first == "saveDirs")
			{
				for (const auto& dir : FileUtils::getSaveDirList())
				{
					queriableList.push_back({ dir });
				}
			}
			else if (props2.first == "dirs")
			{
				for (const auto& dir : FileUtils::geDirList(props2.second, ""))
				{
					queriableList.push_back({ dir });
				}
			}
			else if (props2.first == "files")
			{
				for (const auto& dir : FileUtils::getFileList(props2.second, "", false))
				{
					queriableList.push_back({ dir });
				}
			}
		}
		else
		{
			Level* level = nullptr;
			if (props.first == "currentLevel")
			{
				level = resourceManager.getCurrentLevel();
			}
			else
			{
				level = resourceManager.getDrawable<Level>(std::string(props.first));
			}
			if (level != nullptr &&
				props.second.empty() == false)
			{
				return level->getQueryableList(props.second);
			}
		}
	}
	return queriableList;
}

void Game::setShader(const std::string_view id, sf::Shader* shader) noexcept
{
	if (id.size() <= 1)
	{
		return;
	}
	switch (str2int16(id))
	{
	case str2int16("game"):
		shaders.Game = shader;
		break;
	case str2int16("sprite"):
		shaders.Sprite = shader;
		break;
	default:
		break;
	}
}
