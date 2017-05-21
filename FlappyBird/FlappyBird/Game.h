#pragma once
class GameEngine;
class Game
{
public:
	Game();
	~Game();

	HRESULT init(HWND hWnd);
	void update();
	void render();
	void processInput(WPARAM wParam, LPARAM lParam);
	void close();

	void startPlay();
	void onPlayerFallDawn();
	void onPlayerCrashed();
	void showMainMenu();
	void showHighScore();

protected:
	HRESULT initGeometry();

	void checkCollideables();
	void createBackground();

	int getRandomInt(int minVale, int maxValue);

	std::shared_ptr<GameObject> createBarrierBottom();
	std::shared_ptr<GameObject> createBarrierMiddle();
	std::shared_ptr<GameObject> createBarrierTop();

	void updateScoreText();
	void createMainMenu();
	void createHighscoreMenu();
	void createPlayerHud();
protected:
	std::unique_ptr<GameEngine> mGameEngine = nullptr;

	float mTimeSinceLastBarrierSpawn;

	

	std::shared_ptr<PlayerBird> mPlayer = nullptr;
	std::shared_ptr<BoundingBox> mPlayerBounds = nullptr;
	std::shared_ptr<PlayerHud> mPlayerHud = nullptr;

	std::vector<const BoundingBox*> mCollideableLayer;
	std::vector<const BoundingBox*> mBonusLayer;

	std::vector<std::shared_ptr<GameObject>> mBarriersReserve;
	std::unordered_set<std::shared_ptr<GameObject>> mBarriersActive;

	std::shared_ptr<GroundObjectController> mGroundController = nullptr;
	std::shared_ptr<GameObject> mMainMenu = nullptr;
	std::vector<Button*> mButtons;
	std::shared_ptr<HighScoreDialog> mHighScoreDialog = nullptr;
	std::shared_ptr<BackgroundController> mBackgroundController = nullptr;
	bool mIsOnMenu = true;
};

