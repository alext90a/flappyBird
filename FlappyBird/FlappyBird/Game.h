#pragma once

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

	static float mDeltaTime;
protected:
	HRESULT initD3D(HWND hWnd);
	HRESULT initGeometry();
	void SetupMatrices();

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
	LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       mDevice = NULL; // Our rendering device
	LPD3DXFONT				g_Font = NULL;

	std::unique_ptr<TextureManager> mTextureManager = std::make_unique<TextureManager>();
	std::unique_ptr<GeometryManager> mGeometryManager = std::make_unique<GeometryManager>();
	std::map<int, std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>> mGameObjectLayers;

	static DWORD mLastUpdateTime;
	float mTimeSinceLastBarrierSpawn;
	D3DXMATRIXA16 mMatView;
	D3DXMATRIXA16 mMatProj;
	HWND mHwnd;
	

	std::shared_ptr<Bird> mPlayer = nullptr;
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

