#pragma once

#include "ActionQueryable.h"
#include "Drawables/LoadingScreen.h"
#include "EventManager.h"
#include "FadeInOut.h"
#include "GameInputEventManager.h"
#include "InputEvent.h"
#include <optional>
#include "Queryable.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "VariableManager.h"
#include "VarOrQueryObject.h"
#include <vector>

class Game : public ActionQueryable, public Queryable
{
public:
	static uint32_t DefaultSizeX;
	static uint32_t DefaultSizeY;

	static uint32_t MinSizeX;
	static uint32_t MinSizeY;

	static uint32_t RefSizeX;
	static uint32_t RefSizeY;

protected:
	friend class GameQueryable;

	sf::RenderWindow window;
	sf::RenderTexture gameTexture;
	sf::Sprite gameSprite;

	// window position when not in fullscreen
	std::optional<sf::Vector2i> position;

	// reference size
	// used to position drawables in bigger draw regions
	sf::Vector2u refSize;

	// minimum window size
	// all other sizes are equal or bigger
	sf::Vector2u minSize;

	// actual size of the window the player sees
	sf::Vector2u size;

	// size of the window when not in fullscreen
	// if not in fullscreen, this will be equal to size
	sf::Vector2u windowedSize;

	// draw region size
	// if stretched, this will be equal to minSize
	// otherwise, this will be equal to size
	sf::Vector2u drawRegionSize;

	// previous draw region size
	sf::Vector2u oldDrawRegionSize;

	uint32_t maxHeight{ 0 };

	unsigned frameRate{};
	bool fullScreen{};
	bool smoothScreen{};
	bool stretchToFit{};
	bool keepAR{};
	bool enableInput{};
	bool paused{};
	bool mouseCursorVisible{};

	sf::Vector2i mousePositioni;
	sf::Vector2f mousePositionf;

	sf::Event::MouseButtonEvent mousePressEvt{};
	sf::Event::MouseButtonEvent mouseReleaseEvt{};
	sf::Event::MouseMoveEvent mouseMoveEvt{};
	sf::Event::MouseWheelScrollEvent mouseScrollEvt{};
	sf::Event::KeyEvent keyPressEvt{};
	sf::Event::TextEvent textEnteredEvt{};
	sf::Event::TouchEvent touchBeganEvt{};
	sf::Event::TouchEvent touchMovedEvt{};
	sf::Event::TouchEvent touchEndedEvt{};
	bool mousePressed{};
	bool mouseReleased{};
	bool mouseMoved{};
	bool mouseScrolled{};
	bool keyPressed{};
	bool textEntered{};
	bool touchBegan{};
	bool touchMoved{};
	bool touchEnded{};

	unsigned musicVolume{};
	unsigned soundVolume{};
	unsigned gamma{};

	sf::Time elapsedTime;
	sf::Time totalElapsedTime;

	std::string path;
	std::string title;
	std::string version;

	ResourceManager resourceManager;
	EventManager eventManager;
	VariableManager variableManager;
	GameInputEventManager gameInputEventManager;

	std::unique_ptr<LoadingScreen> loadingScreen;
	FadeInOut fadeObj;

	GameShaders shaders;

	std::shared_ptr<Action> closeAction;
	std::shared_ptr<Action> resizeAction;
	std::shared_ptr<Action> focusLossAction;
	std::shared_ptr<Action> focusGainAction;

	void processEvents();
	void onClosed();
	void onResized(const sf::Event::SizeEvent& evt);
	void onLostFocus() noexcept;
	void onGainedFocus() noexcept;

	void onTextEntered(const sf::Event::TextEvent& evt) noexcept;
	void onKeyPressed(sf::Event evt);
	void onKeyReleased(sf::Event evt);
	void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt);
	void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
	void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);
	void onMouseMoved(const sf::Event::MouseMoveEvent& evt);
	void onTouchBegan(const sf::Event::TouchEvent& evt);
	void onTouchMoved(const sf::Event::TouchEvent& evt);
	void onTouchEnded(const sf::Event::TouchEvent& evt);

	void updateMousePosition(const sf::Vector2i mousePos);
	void updateEvents();
	void drawCursor();
	void drawUI();
	void update();
	void drawWindow();

	void updateWindowSize();

	// recreates the given renderTexture using the current draw region size
	// and sets the smooth property.
	void recreateRenderTexture(sf::RenderTexture& renderTexture) const;

	void recreateWindow();

	void reset();

public:
	Game();
	virtual ~Game();

	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;

	void load(const std::string_view gamefilePath, const std::string_view mainFile);
	void init();

	auto& Shaders() noexcept { return shaders; };
	auto& Shaders() const noexcept { return shaders; };

	auto getOpenGLDepthBits() const { return window.getSettings().depthBits; }
	auto getOpenGLStencilBits() const { return window.getSettings().stencilBits; }
	auto getOpenGLAntialiasingLevel() const { return window.getSettings().antialiasingLevel; }
	auto getOpenGLMajorVersion() const { return window.getSettings().majorVersion; }
	auto getOpenGLMinorVersion() const { return window.getSettings().minorVersion; }
	bool getOpenGLSRgbCapable() const { return window.getSettings().sRgbCapable; }

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	auto hasFocus() const noexcept { return window.hasFocus(); }

	auto Position() const noexcept { return position.value_or(sf::Vector2i()); }
	auto& Size() const noexcept { return size; }
	auto& WindowedSize() const noexcept { return windowedSize; }
	auto& DrawRegionSize() const noexcept { return drawRegionSize; }
	auto& OldDrawRegionSize() const noexcept { return oldDrawRegionSize; }
	auto DrawRegionSizef() const
	{
		return sf::Vector2f((float)drawRegionSize.x, (float)drawRegionSize.y);
	}
	void Position(const sf::Vector2i& position_);
	void Size(sf::Vector2u size_);
	auto& RefSize() const noexcept { return refSize; }
	void RefSize(const sf::Vector2u& size_);
	auto& MinSize() const noexcept { return minSize; }
	void MinSize(const sf::Vector2u& size_);
	auto MaxHeight() const noexcept { return maxHeight; }
	void MaxHeight(uint32_t maxHeight_);
	auto FrameRate() const noexcept { return frameRate; }
	bool FullScreen() const noexcept { return fullScreen; }
	bool SmoothScreen() const noexcept { return smoothScreen; }
	bool StretchToFit() const noexcept { return stretchToFit; }
	bool KeepAR() const noexcept { return keepAR; }

	auto& MousePositioni() const noexcept { return mousePositioni; }
	auto& MousePositionf() const noexcept { return mousePositionf; }

	auto& MousePress() const noexcept { return mousePressEvt; }
	auto& MouseRelease() const noexcept { return mouseReleaseEvt; }
	auto& MouseMove() const noexcept { return mouseMoveEvt; }
	auto& MouseScroll() const noexcept { return mouseScrollEvt; }
	auto& KeyPress() const noexcept { return keyPressEvt; }
	auto& TextEntered() const noexcept { return textEnteredEvt; }
	auto& TouchBegan() const noexcept { return touchBeganEvt; }
	auto& TouchMoved() const noexcept { return touchMovedEvt; }
	auto& TouchEnded() const noexcept { return touchEndedEvt; }

	bool wasMousePressed() const noexcept { return mousePressed; }
	bool wasMouseReleased() const noexcept { return mouseReleased; }
	bool wasMouseMoved() const noexcept { return mouseMoved; }
	bool wasMouseScrolled() const noexcept { return mouseScrolled; }
	bool wasKeyPressed() const noexcept { return keyPressed; }
	bool wasTextEntered() const noexcept { return textEntered; }
	bool hasTouchBegan() const noexcept { return touchBegan; }
	bool hasTouchMoved() const noexcept { return touchMoved; }
	bool hasTouchEnded() const noexcept { return touchEnded; }

	void clearMousePressed() noexcept { mousePressed = false; }
	void clearMouseReleased() noexcept { mouseReleased = false; }
	void clearMouseMoved() noexcept { mouseMoved = false; }
	void clearMouseScrolled() noexcept { mouseScrolled = false; }
	void clearKeyPressed() noexcept { keyPressed = false; }
	void clearTextEntered() noexcept { textEntered = false; }
	void clearTouchBegan() noexcept { touchBegan = false; }
	void clearTouchMoved() noexcept { touchMoved = false; }
	void clearTouchEnded() noexcept { touchEnded = false; }

	void clearInputEvents(InputEventType e) noexcept;

	auto& GameInputEvents() noexcept { return gameInputEventManager; }
	auto& GameInputEvents() const noexcept { return gameInputEventManager; }

	void FrameRate(int frameRate_);
	void FullScreen(bool fullScreen_);
	void SmoothScreen(bool smooth_);
	void StretchToFit(bool stretchToFit_);
	void KeepAR(bool keepAR_);

	auto MusicVolume() const noexcept { return musicVolume; }
	void MusicVolume(int volume) noexcept
	{
		musicVolume = (unsigned)std::clamp(volume, 0, 100);
	}
	auto SoundVolume() const noexcept { return soundVolume; }
	void SoundVolume(int volume) noexcept
	{
		soundVolume = (unsigned)std::clamp(volume, 0, 100);
	}
	auto Gamma() const noexcept { return gamma; }
	void Gamma(int gamma_) noexcept
	{
		gamma = (unsigned)std::clamp(gamma_, 0, 100);
	}

	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	auto& getElapsedTime() const noexcept { return elapsedTime; }
	auto& getTotalElapsedTime() const noexcept { return totalElapsedTime; }

	auto& Path() const noexcept { return path; }
	auto& Title() const noexcept { return title; }
	auto& Version() const noexcept { return version; }

	bool isInputEnabled() const noexcept { return enableInput; }
	void EnableInput(bool enable) noexcept { enableInput = enable; }

	auto& Resources() noexcept { return resourceManager; }
	auto& Resources() const noexcept { return resourceManager; }
	auto& Events() noexcept { return eventManager; }
	auto& Events() const noexcept { return eventManager; }

	void close() { window.close(); }
	void setIcon(unsigned int width, unsigned int height, const sf::Uint8* pixels)
	{
		window.setIcon(width, height, pixels);
	}
	void setMouseCursorVisible(bool show);
	void Path(const std::string& path_) { path = path_; }
	void Title(const std::string& title_);
	void Version(const std::string& version_) { version = version_; }

	auto getLoadingScreen() noexcept { return loadingScreen.get(); }
	void setLoadingScreen(std::unique_ptr<LoadingScreen> loadingScreen_) noexcept
	{
		loadingScreen = std::move(loadingScreen_);
	}

	void draw();
	bool drawLoadingScreen();

	auto& FadeObj() noexcept { return fadeObj; }
	auto& FadeObj() const noexcept { return fadeObj; }

	void setMousePosition(sf::Vector2i mousePos);
	void updateMousePosition();
	void updateCursorPosition();

	void play();

	auto& Variables() noexcept { return variableManager; }
	auto& Variables() const noexcept { return variableManager; }

	template <class T, class U>
	U getVarOrProp(const Variable& var, U defVal = U())
	{
		return variableManager.getVarOrProp<T, U>(*this, var, defVal);
	}

	bool getVarOrPropNoToken(const std::string_view key, Variable& var) const;
	bool getVarOrProp(const std::string_view key, Variable& var) const;
	Variable getVarOrProp(const Variable& var) const;
	bool getVarOrPropBoolS(const std::string_view key) const;
	bool getVarOrPropBoolV(const Variable& var) const;
	double getVarOrPropDoubleS(const std::string_view key) const;
	double getVarOrPropDoubleV(const Variable& var) const;
	int64_t getVarOrPropInt64S(const std::string_view key) const;
	int64_t getVarOrPropInt64V(const Variable& var) const;
	bool getVarOrPropStringS(const std::string_view key, std::string& outStr) const;
	std::string getVarOrPropStringS(const std::string_view key) const;
	std::string getVarOrPropStringV(const Variable& var) const;

	// get an action from a UIObject using a query.
	// ex: getQueryAction("btn1.click")
	std::shared_ptr<Action> getQueryAction(const std::string_view prop) const;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	virtual bool getProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const;

	bool getGameProperty(const std::string_view prop, Variable& var) const;

	virtual bool getGameProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const;

	virtual void setGameProperty(const std::string_view prop, const Variable& val);

	QueryObject getQueryable(const std::string_view prop) const override;

	virtual QueryObject getQueryable(const std::string_view prop1, const std::string_view prop2) const;

	std::vector<VarOrQueryObject> getQueryableList(const std::string_view prop) const;

	virtual bool getQueryableList(const std::string_view prop1,
		const std::string_view prop2, std::vector<VarOrQueryObject>& queryableList) const;
};
