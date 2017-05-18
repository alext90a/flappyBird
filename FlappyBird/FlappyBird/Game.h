#pragma once

class Game
{
public:
	Game();
	~Game();

	HRESULT init(HWND hWnd);
	void update();
	void render();
	void processInput(WPARAM wParam);
	void close();

	void startPlay();
	void onPlayerFallDawn();
	void onPlayerCrashed();
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

protected:
	LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
	LPD3DXFONT				g_Font = NULL;
	//LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture
	std::unique_ptr<TextureManager> mTextureManager = std::make_unique<TextureManager>();
	std::unique_ptr<GeometryManager> mGeometryManager = std::make_unique<GeometryManager>();
	std::shared_ptr<Bird> mPlayer = nullptr;
	std::shared_ptr<BoundingBox> mPlayerBounds = nullptr;
	
	

	std::string mTestText = "Hello word!";
	RECT textbox;

	std::string mScoreText = "Score: 0";
	RECT mScoreRect;

	std::vector<const BoundingBox*> mCollideableLayer;
	std::vector<const BoundingBox*> mBonusLayer;

	std::vector<std::shared_ptr<GameObject>> mGameObjects;
	std::vector<std::shared_ptr<GameObject>> mBackgroundObjects;
	std::vector<std::shared_ptr<GameObject>> mObjectsReserve;
	std::unordered_set<std::shared_ptr<GameObject>> mActiveObject;

	static DWORD mLastUpdateTime;
	float mTimeSinceLastBarrierSpawn;

	bool mIsOnMenu = false;
};

