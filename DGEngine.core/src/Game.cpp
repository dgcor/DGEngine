#include "Game.h"
#include "Button.h"
#include "FileUtils.h"
#include "Formula.h"
#include "Hooks.h"
#include "Image.h"
#include "Json/JsonUtils.h"
#include <mutex>
#include "Parser/Parser.h"
#include <SFML/Audio/SoundFileFactory.hpp>
#include "SFML/SFMLUtils.h"
#include "SFML/Wave2.h"
#include "Utils/ReverseIterable.h"
#include "Utils/Utils.h"

uint32_t Game::DefaultSizeX{ 640 };
uint32_t Game::DefaultSizeY{ 480 };

uint32_t Game::MinSizeX{ 640 };
uint32_t Game::MinSizeY{ 480 };

uint32_t Game::RefSizeX{ 640 };
uint32_t Game::RefSizeY{ 480 };

Game::Game()
{
	static std::once_flag initWave2;
	std::call_once(initWave2, []() { sf::SoundFileFactory::registerReader<Wave2FileReader>(); });
}

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
	gameSprite = {};

	refSize = { RefSizeX, RefSizeY };
	minSize = { MinSizeX, MinSizeY };
	size = { DefaultSizeX, DefaultSizeY };
	drawRegionSize = { DefaultSizeX, DefaultSizeY };
	oldDrawRegionSize = {};
	maxHeight = { 0 };
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
	if (Hooks::InitializeShaderManager != nullptr)
	{
		Hooks::InitializeShaderManager(resourceManager.Shaders());
	}
	else
	{
		ShaderManager::init(resourceManager.Shaders());
	}
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

	window.create(sf::VideoMode(size.x, size.y), title);

	if (framerate > 0)
	{
		window.setFramerateLimit(framerate);
	}

	// forces update of drawing window
	updateGameWindowSize();

	if (gameSprite.getTexture() == nullptr)
	{
		recreateRenderTexture(gameTexture);
		gameSprite.setTexture(gameTexture.getTexture(), true);
	}
}

void Game::WindowSize(sf::Vector2u size_)
{
	size_.x = std::max(minSize.x, size_.x);
	size_.y = std::max(minSize.y, size_.y);
	if (window.isOpen() == true)
	{
		window.setSize(size_);
	}
	else
	{
		size = size_;
	}
}

void Game::RefSize(const sf::Vector2u& size_)
{
	if (size_.x >= RefSizeX && size_.y >= RefSizeY)
	{
		refSize = size_;
	}
}

void Game::MinSize(const sf::Vector2u& size_)
{
	bool needsUpdate = false;
	if (size_.x >= MinSizeX && size_.y >= MinSizeY)
	{
		minSize = size_;
		if (window.isOpen() == true)
		{
			needsUpdate = true;
		}
	}
	if (size.x < minSize.x && size.y < minSize.y)
	{
		if (window.isOpen() == true)
		{
			window.setSize(minSize);
			return;
		}
		else
		{
			size = minSize;
		}
	}
	if (needsUpdate == true)
	{
		updateGameWindowSize();
	}
}

void Game::Framerate(int framerate_)
{
	if (framerate_ > 0)
	{
		framerate = (unsigned)std::clamp(framerate_, 30, 240);
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
			onKeyPressed(evt);
			break;
		case sf::Event::KeyReleased:
			onKeyReleased(evt);
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
	resourceManager.updateActiveInputEvents();
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
	size = newSize;
	updateGameWindowSize();
}

void Game::updateGameWindowSize()
{
	// update draw region sizes
	oldDrawRegionSize = drawRegionSize;
	drawRegionSize = (stretchToFit == false ? size : minSize);

	bool usingMaxHeight = false;
	if (stretchToFit == false &&
		maxHeight >= minSize.y &&
		drawRegionSize.y > maxHeight)
	{
		usingMaxHeight = true;

		auto factor = (float)(maxHeight) / (float)(drawRegionSize.y);
		drawRegionSize.x = (uint32_t)((float)(drawRegionSize.x) * factor);
		drawRegionSize.y = maxHeight;

		if (drawRegionSize.x < minSize.x)
		{
			drawRegionSize.x = minSize.x;
		}
	}

	// force even size
	auto evenRegionSize = drawRegionSize;
	evenRegionSize.x = ((uint32_t)((int)((float)evenRegionSize.x * 0.5f)) * 2);
	evenRegionSize.y = ((uint32_t)((int)((float)evenRegionSize.y * 0.5f)) * 2);

	// update main view
	auto view = window.getView();
	view.reset({ 0.f, 0.f, (float)evenRegionSize.x, (float)evenRegionSize.y });
	if (stretchToFit == true && keepAR == true)
	{
		SFMLUtils::viewStretchKeepAR(view, size);
	}
	else if (stretchToFit == false && usingMaxHeight == true)
	{
		SFMLUtils::viewStretchKeepAR(view, size);
	}
	else
	{
		float normalizedWidth = (float)evenRegionSize.x / (float)drawRegionSize.x;
		float normalizedHeight = (float)evenRegionSize.y / (float)drawRegionSize.y;
		view.setViewport({ 0.f, 0.f, normalizedWidth, normalizedHeight });
	}
	window.setView(view);

	drawRegionSize = evenRegionSize;

	if (oldDrawRegionSize == drawRegionSize)
	{
		return;
	}

	// clears artefacts
	window.clear();
	window.display();

	// update game texture
	recreateRenderTexture(gameTexture);
	gameSprite.setTexture(gameTexture.getTexture(), true);

	// update drawables
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

void Game::onKeyPressed(sf::Event evt)
{
	if (enableInput == false)
	{
		return;
	}
	keyPressEvt = evt.key;
	keyPressed = true;
	if (loadingScreen != nullptr)
	{
		return;
	}
	auto action = resourceManager.getInputAction(evt);
	if (action != nullptr)
	{
		eventManager.addBack(action);
	}
}

void Game::onKeyReleased(sf::Event evt)
{
	if (enableInput == false)
	{
		return;
	}
	if (loadingScreen != nullptr)
	{
		return;
	}
	auto action = resourceManager.getInputAction(evt);
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
	loadingScreen->draw(*this, gameTexture);
	drawWindow();
	return true;
}

void Game::update()
{
	for (auto& res : reverse(resourceManager))
	{
		if ((int)(res.ignore & IgnoreResource::Update) == 0)
		{
			for (auto obj : reverse(res.drawables))
			{
				if (paused == false && res.ignore != IgnoreResource::Update)
				{
					obj->update(*this);
				}
			}
		}
		else if ((int)(res.ignore & IgnoreResource::All) != 0)
		{
			break;
		}
	}
}

void Game::drawUI()
{
	for (auto& res : resourceManager)
	{
		if ((int)(res.ignore & IgnoreResource::Draw) == 0)
		{
			for (auto obj : res.drawables)
			{
				obj->draw(*this, gameTexture);
			}
		}
		else if ((int)(res.ignore & IgnoreResource::All) != 0)
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
	auto states(sf::RenderStates::Default);
	if (shaders.hasGameShader() == true)
	{
		auto shader = shaders.Game->shader.get();
		states.shader = shader;
		for (auto uniformHash : shaders.Game->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("elapsedTime"):
			{
				shader->setUniform("elapsedTime", totalElapsedTime.asSeconds());
				break;
			}
			case str2int16("gamma"):
			{
				shader->setUniform("gamma", (float)gamma);
				break;
			}
			case str2int16("fade"):
			{
				shader->setUniform("fade", sf::Glsl::Vec4(fadeObj.getColor()));
				break;
			}
			case str2int16("loading"):
			{
				if (loadingScreen != nullptr)
				{
					shader->setUniform("loading", (float)loadingScreen->getProgress() / 100.f);
				}
				else
				{
					shader->setUniform("loading", 1.f);
				}
				break;
			}
			case str2int16("mousePosition"):
			{
				shader->setUniform("mousePosition", sf::Glsl::Vec2(
					(float)std::clamp(mousePositioni.x, 0, (int)drawRegionSize.x) / (float)drawRegionSize.x,
					(float)std::clamp(mousePositioni.y, 0, (int)drawRegionSize.y) / (float)drawRegionSize.y
				));
				break;
			}
			case str2int16("textureSize"):
			{
				shader->setUniform("textureSize", sf::Glsl::Vec2(
					(float)drawRegionSize.x,
					(float)drawRegionSize.y
				));
				break;
			}
			default:
				break;
			}
		}
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

void Game::clearInputEvents(InputEventType e) noexcept
{
	if (e == InputEventType::None)
	{
		return;
	}
	else if (e == InputEventType::All)
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
		(int)(e & InputEventType::MousePress) != 0)
	{
		if ((int)(e & InputEventType::LeftClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Left)
		{
			mousePressed = false;
		}
		else if ((int)(e & InputEventType::MiddleClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Middle)
		{
			mousePressed = false;
		}
		else if ((int)(e & InputEventType::RightClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Right)
		{
			mousePressed = false;
		}
	}
	if (mouseReleased == true &&
		(int)(e & InputEventType::MouseRelease) != 0)
	{
		if ((int)(e & InputEventType::LeftClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Left)
		{
			mouseReleased = false;
		}
		else if ((int)(e & InputEventType::MiddleClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Middle)
		{
			mouseReleased = false;
		}
		else if ((int)(e & InputEventType::RightClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Right)
		{
			mouseReleased = false;
		}
	}
	if (mouseMoved == true &&
		(int)(e & InputEventType::MouseMove) != 0)
	{
		mouseMoved = false;
	}
	if (mouseScrolled == true &&
		(int)(e & InputEventType::MouseScroll) != 0)
	{
		mouseScrolled = false;
	}
	if (keyPressed == true &&
		(int)(e & InputEventType::KeyPress) != 0)
	{
		keyPressed = false;
	}
	if (textEntered == true &&
		(int)(e & InputEventType::TextEnter) != 0)
	{
		textEntered = false;
	}
	if (touchBegan == true &&
		(int)(e & InputEventType::TouchBegin) != 0)
	{
		touchBegan = false;
	}
	if (touchMoved == true &&
		(int)(e & InputEventType::TouchMove) != 0)
	{
		touchMoved = false;
	}
	if (touchEnded == true &&
		(int)(e & InputEventType::TouchEnd) != 0)
	{
		touchEnded = false;
	}
}

void Game::SmoothScreen(bool smooth_)
{
	smoothScreen = smooth_;
	gameTexture.setSmooth(smooth_);
}

void Game::MaxHeight(uint32_t maxHeight_)
{
	if (maxHeight == maxHeight_)
	{
		return;
	}
	maxHeight = maxHeight_;
	if (window.isOpen() == false)
	{
		return;
	}
	updateGameWindowSize();
	updateMousePosition();
}

void Game::StretchToFit(bool stretchToFit_)
{
	if (stretchToFit == stretchToFit_)
	{
		return;
	}
	stretchToFit = stretchToFit_;
	if (window.isOpen() == false)
	{
		return;
	}
	updateGameWindowSize();
	updateMousePosition();
}

void Game::KeepAR(bool keepAR_)
{
	if (keepAR == keepAR_)
	{
		return;
	}
	keepAR = keepAR_;
	if (window.isOpen() == false || stretchToFit == false)
	{
		return;
	}
	updateGameWindowSize();
	updateMousePosition();
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

bool Game::getVariableNoToken(const std::string_view key, Variable& var) const
{
	auto it = variables.find(sv2str(key));
	if (it != variables.cend())
	{
		var = it->second;
		return true;
	}
	return false;
}

bool Game::getVarOrPropNoToken(const std::string_view key, Variable& var) const
{
	if (getVariableNoToken(key, var) == true)
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

int64_t Game::getVarOrPropInt64S(const std::string_view key) const
{
	Variable var;
	if (getVarOrProp(key, var) == true)
	{
		return VarUtils::toInt64(var);
	}
	return 0;
}

int64_t Game::getVarOrPropInt64V(const Variable& var) const
{
	if (std::holds_alternative<std::string>(var))
	{
		return getVarOrPropInt64S(std::get<std::string>(var));
	}
	return VarUtils::toInt64(var);
}

bool Game::getVarOrPropStringS(const std::string_view key, std::string& outStr) const
{
	if ((key.size() > 2) &&
		(key.front() == '%') &&
		(key.back() == '%'))
	{
		auto key2 = key.substr(1, key.size() - 2);
		Variable var;
		if (getVariableNoToken(key2, var) == true)
		{
			if (std::holds_alternative<std::string>(var) == true)
			{
				outStr = std::get<std::string>(var);
				if (getProperty(outStr, var) == true)
				{
					outStr = VarUtils::toString(var);
				}
			}
			else
			{
				outStr = VarUtils::toString(var);
			}
			return true;
		}
		else if (getProperty(key2, var) == true)
		{
			outStr = VarUtils::toString(var);
			return true;
		}
	}
	outStr = key;
	return false;
}

std::string Game::getVarOrPropStringS(const std::string_view key) const
{
	std::string str;
	getVarOrPropStringS(key, str);
	return str;
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
	if (prop[0] == '#')
	{
		return getVariableNoToken(prop.substr(1), var);
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return getProperty(props.first, props.second, var);
}

bool Game::getProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const
{
	switch (str2int16(prop1))
	{
	case str2int16("$"):
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(prop2, *this));
		return true;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(prop2, *this));
		return true;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(prop2, *this));
		return true;
	case str2int16("$f"):
	case str2int16("evalf"):
		var = Variable(Formula::evalString(prop2, *this));
		return true;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(prop2, *this));
		return true;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(prop2, *this));
		return true;
	case str2int16("game"):
	{
		if (prop2.size() > 1)
		{
			return getGameProperty(prop2, var);
		}
		break;
	}
	default:
	{
		const UIObject* uiObject = nullptr;
		if (prop1 == "currentLevel")
		{
			uiObject = resourceManager.getCurrentLevel<UIObject>();
		}
		else if (prop1 == "focus")
		{
			uiObject = resourceManager.getFocused();
		}
		else
		{
			uiObject = resourceManager.getDrawable(prop1);
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

bool Game::getGameProperty(const std::string_view prop, Variable& var) const
{
	auto props = Utils::splitStringIn2(prop, '.');
	return getGameProperty(props.first, props.second, var);
}

bool Game::getGameProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const
{
	switch (str2int16(prop1))
	{
	case str2int16("cursor"):
	{
		if (prop2 == "x")
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
	case str2int16("hasAudio"):
		var = Variable(resourceManager.hasAudioSource(prop2));
		break;
	case str2int16("hasDrawable"):
		var = Variable(resourceManager.hasDrawable(prop2));
		break;
	case str2int16("hasEvent"):
		var = Variable(eventManager.exists(prop2));
		break;
	case str2int16("hasFont"):
		var = Variable(resourceManager.hasFont(prop2));
		break;
	case str2int16("hasGameShader"):
		var = Variable(shaders.hasGameShader());
		break;
	case str2int16("hasImageContainer"):
		var = Variable(resourceManager.hasImageContainer(prop2));
		break;
	case str2int16("hasPalette"):
		var = Variable(resourceManager.hasPalette(prop2));
		break;
	case str2int16("hasResource"):
		var = Variable(resourceManager.resourceExists(prop2));
		break;
	case str2int16("hasShader"):
		var = Variable(resourceManager.Shaders().has(prop2));
		break;
	case str2int16("hasSong"):
		var = Variable(resourceManager.hasSong(prop2));
		break;
	case str2int16("hasSpriteShader"):
		var = Variable(shaders.hasSpriteShader());
		break;
	case str2int16("hasTexture"):
		var = Variable(resourceManager.hasTexture(prop2));
		break;
	case str2int16("hasTexturePack"):
		var = Variable(resourceManager.hasTexturePack(prop2));
		break;
	case str2int16("keepAR"):
		var = Variable(keepAR);
		break;
	case str2int16("maxWindowHeight"):
		var = Variable((int64_t)maxHeight);
		break;
	case str2int16("minSize"):
	{
		if (prop2 == "x")
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
		switch (str2int16(prop2))
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
		case str2int16("maxTextureSize"):
			var = Variable((int64_t)sf::Texture::getMaximumSize());
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
		if (prop2 == "x")
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
		if (prop2 == "x")
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
	case str2int16("maxWindowHeight"):
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			MaxHeight((uint32_t)std::get<int64_t>(val));
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
	return getQueryable(props.first, props.second);
}

const Queryable* Game::getQueryable(const std::string_view prop1, const std::string_view prop2) const
{
	if (prop1.empty() == true || prop1 == "game")
	{
		return this;
	}
	const Queryable* queryable = nullptr;
	if (prop1 == "currentLevel")
	{
		queryable = resourceManager.getCurrentLevel<Queryable>();
	}
	else if (prop1 == "focus")
	{
		queryable = resourceManager.getFocused();
	}
	else
	{
		queryable = resourceManager.getDrawable(prop1);
	}
	if (queryable != nullptr && prop2.empty() == false)
	{
		return queryable->getQueryable(prop2);
	}
	return queryable;
}

std::vector<std::variant<const Queryable*, Variable>> Game::getQueryableList(
	const std::string_view prop) const
{
	std::vector<std::variant<const Queryable*, Variable>> queryableList;
	if (prop.empty() == false)
	{
		auto props = Utils::splitStringIn2(prop, '.');
		getQueryableList(props.first, props.second, queryableList);
	}
	return queryableList;
}

bool Game::getQueryableList(const std::string_view prop1, const std::string_view prop2,
	std::vector<std::variant<const Queryable*, Variable>>& queryableList) const
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

void Game::setShader(const std::string_view id, GameShader* shader) noexcept
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
	case str2int16("level"):
		shaders.Level = shader;
		break;
	case str2int16("sprite"):
		shaders.Sprite = shader;
		break;
	default:
		break;
	}
}
