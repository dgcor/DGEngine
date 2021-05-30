#pragma once

#include "EventManager.h"
#include "FadeInOut.h"
#include "InputEvent.h"
#include "LoadingScreen.h"
#include "Queryable.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Utils/UnorderedStringMap.h"
#include "Variable.h"
#include <vector>

class Game : public sf::NonCopyable, public Queryable
{
public:
	static uint32_t DefaultSizeX;
	static uint32_t DefaultSizeY;

	static uint32_t MinSizeX;
	static uint32_t MinSizeY;

	static uint32_t RefSizeX;
	static uint32_t RefSizeY;

protected:
	sf::RenderWindow window;
	sf::RenderTexture gameTexture;
	sf::Sprite gameSprite;

	// reference size
	// used to position drawables in bigger draw regions
	sf::Vector2u refSize;

	// minimum window size
	// all other sizes are equal or bigger
	sf::Vector2u minSize;

	// actual size of the window the player sees
	sf::Vector2u size;

	// draw region size
	// if stretchd, this will be equal to minSize
	// otherwise, this will be equal to size
	sf::Vector2u drawRegionSize;

	// previous draw region size
	sf::Vector2u oldDrawRegionSize;

	uint32_t maxHeight{ 0 };

	unsigned framerate{};
	bool smoothScreen{};
	bool stretchToFit{};
	bool keepAR{};
	bool enableInput{};
	bool pauseOnFocusLoss{};
	bool paused{};

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

	UnorderedStringMap<Variable> variables;

	std::unique_ptr<LoadingScreen> loadingScreen;
	FadeInOut fadeObj;

	GameShaders shaders;

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
	void drawAndUpdate();
	void drawWindow();

	void updateGameWindowSize();

	// recreates the given renderTexture using the current draw region size
	// and sets the smooth property.
	void recreateRenderTexture(sf::RenderTexture& renderTexture) const;

	void reset();

public:
	Game();
	virtual ~Game();

	void load(const std::string_view gamefilePath, const std::string_view mainFile);
	void init();

	const GameShaders& Shaders() const noexcept { return shaders; };

	void setShader(const std::string_view id, GameShader* shader) noexcept;

	unsigned getOpenGLDepthBits() const { return window.getSettings().depthBits; }
	unsigned getOpenGLStencilBits() const { return window.getSettings().stencilBits; }
	unsigned getOpenGLAntialiasingLevel() const { return window.getSettings().antialiasingLevel; }
	unsigned getOpenGLMajorVersion() const { return window.getSettings().majorVersion; }
	unsigned getOpenGLMinorVersion() const { return window.getSettings().minorVersion; }
	bool getOpenGLSRgbCapable() const { return window.getSettings().sRgbCapable; }

	const sf::Vector2u& WindowSize() const noexcept { return size; }
	const sf::Vector2u& DrawRegionSize() const noexcept { return drawRegionSize; }
	const sf::Vector2u& OldDrawRegionSize() const noexcept { return oldDrawRegionSize; }
	sf::Vector2f DrawRegionSizef() const
	{
		return sf::Vector2f((float)drawRegionSize.x, (float)drawRegionSize.y);
	}
	void WindowSize(sf::Vector2u size_);
	const sf::Vector2u& RefSize() const noexcept { return refSize; }
	void RefSize(const sf::Vector2u& size_);
	const sf::Vector2u& MinSize() const noexcept { return minSize; }
	void MinSize(const sf::Vector2u& size_);
	uint32_t MaxHeight() const noexcept { return maxHeight; }
	void MaxHeight(uint32_t maxHeight_);
	unsigned Framerate() const noexcept { return framerate; }
	bool SmoothScreen() const noexcept { return smoothScreen; }
	bool StretchToFit() const noexcept { return stretchToFit; }
	bool KeepAR() const noexcept { return keepAR; }

	const sf::Vector2i& MousePositioni() const noexcept { return mousePositioni; }
	const sf::Vector2f& MousePositionf() const noexcept { return mousePositionf; }

	const sf::Event::MouseButtonEvent& MousePress() const noexcept { return mousePressEvt; }
	const sf::Event::MouseButtonEvent& MouseRelease() const noexcept { return mouseReleaseEvt; }
	const sf::Event::MouseMoveEvent& MouseMove() const noexcept { return mouseMoveEvt; }
	const sf::Event::MouseWheelScrollEvent& MouseScroll() const noexcept { return mouseScrollEvt; }
	const sf::Event::KeyEvent& KeyPress() const noexcept { return keyPressEvt; }
	const sf::Event::TextEvent& TextEntered() const noexcept { return textEnteredEvt; }
	const sf::Event::TouchEvent& TouchBegan() const noexcept { return touchBeganEvt; }
	const sf::Event::TouchEvent& TouchMoved() const noexcept { return touchMovedEvt; }
	const sf::Event::TouchEvent& TouchEnded() const noexcept { return touchEndedEvt; }

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

	void MinWidth(unsigned width_) noexcept { size.x = width_; }
	void MinHeight(unsigned height_) noexcept { size.y = height_; }
	void Framerate(int framerate_);
	void SmoothScreen(bool smooth_);
	void StretchToFit(bool stretchToFit_);
	void KeepAR(bool keepAR_);
	void PauseOnFocusLoss(bool pause_) noexcept { pauseOnFocusLoss = pause_; }

	unsigned MusicVolume() const noexcept { return musicVolume; }
	void MusicVolume(int volume) noexcept
	{
		musicVolume = (unsigned)std::clamp(volume, 0, 100);
	}
	unsigned SoundVolume() const noexcept { return soundVolume; }
	void SoundVolume(int volume) noexcept
	{
		soundVolume = (unsigned)std::clamp(volume, 0, 100);
	}
	unsigned Gamma() const noexcept { return gamma; }
	void Gamma(int gamma_) noexcept
	{
		gamma = (unsigned)std::clamp(gamma_, 0, 100);
	}

	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	const sf::Time& getElapsedTime() const noexcept { return elapsedTime; }
	const sf::Time& getTotalElapsedTime() const noexcept { return totalElapsedTime; }

	const std::string& getPath() const noexcept { return path; }
	const std::string& getTitle() const noexcept { return title; }
	const std::string& getVersion() const noexcept { return version; }

	bool isInputEnabled() const noexcept { return enableInput; }
	void EnableInput(bool enable) noexcept { enableInput = enable; }

	ResourceManager& Resources() noexcept { return resourceManager; }
	const ResourceManager& Resources() const noexcept { return resourceManager; }
	EventManager& Events() noexcept { return eventManager; }

	void close() { window.close(); }
	void setIcon(unsigned int width, unsigned int height, const sf::Uint8* pixels)
	{
		window.setIcon(width, height, pixels);
	}
	void setMouseCursorVisible(bool show) { window.setMouseCursorVisible(show); }
	void setPath(const std::string& path_) { path = path_; }
	void setTitle(const std::string& title_);
	void setVersion(const std::string& version_) { version = version_; }

	LoadingScreen* getLoadingScreen() noexcept { return loadingScreen.get(); }
	void setLoadingScreen(std::unique_ptr<LoadingScreen> loadingScreen_) noexcept
	{
		loadingScreen = std::move(loadingScreen_);
	}

	void draw();
	bool drawLoadingScreen();

	FadeInOut& FadeObj() noexcept { return fadeObj; }
	const FadeInOut& FadeObj() const noexcept { return fadeObj; }

	void setMousePosition(sf::Vector2i mousePos);
	void updateMousePosition();
	void updateCursorPosition();

	void play();

	const UnorderedStringMap<Variable>& getVariables() const noexcept { return variables; }

	// gets variable without tokens. ex: "var"
	bool getVariableNoToken(const std::string_view key, Variable& var) const;

	template <class T, class U>
	U getVarOrProp(const Variable& var, U defVal = U())
	{
		if (std::holds_alternative<T>(var) == true)
		{
			return (U)std::get<T>(var);
		}
		else if (std::holds_alternative<std::string>(var) == true)
		{
			Variable var2;
			if (getVarOrProp(std::get<std::string>(var), var2) == true)
			{
				if (std::holds_alternative<T>(var2) == true)
				{
					return (U)std::get<T>(var2);
				}
			}
		}
		return defVal;
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

	// no tokens in key.
	void clearVariable(const std::string& key);

	// no tokens in key.
	void setVariable(const std::string& key, const Variable& value);

	void saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const;

	// get an action from a UIObject using a query.
	// ex: getQueryAction("btn1.click")
	std::shared_ptr<Action> getQueryAction(const std::string_view prop) const;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	virtual bool getProperty(const std::string_view prop1,
		const std::string_view prop2, Variable& var) const;

	bool getGameProperty(const std::string_view prop, Variable& var) const;

	virtual bool getGameProperty(const std::string_view prop1,
		const std::string_view prop2, Variable& var) const;

	virtual void setGameProperty(const std::string_view prop, const Variable& val);

	const Queryable* getQueryable(const std::string_view prop) const override;

	virtual const Queryable* getQueryable(const std::string_view prop1,
		const std::string_view prop2) const;

	std::vector<std::variant<const Queryable*, Variable>> getQueryableList(
		const std::string_view prop) const;

	virtual bool getQueryableList(const std::string_view prop1, const std::string_view prop2,
		std::vector<std::variant<const Queryable*, Variable>>& queryableList) const;
};
