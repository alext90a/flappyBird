#pragma once

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	HRESULT init(HWND hWnd);
	void update();
	void render();
	void close();
	void addLayer(const int layer);
	void addGameObject(const int layer, std::shared_ptr<GameObject> gameObject);
	static float mDeltaTime;
	LPDIRECT3DDEVICE9 getDevice();
	LPD3DXFONT getFont();
	std::shared_ptr<Geometry> getGeometry(GEOMETRY goemetryType);
	std::shared_ptr<Texture> getTexture(const std::string& name);
	void getMouseWorldRay(D3DXVECTOR3& pickRayDir, D3DXVECTOR3& pickRayOrig);
	HWND getHwnd();
	void setCameraParams(const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAt);
protected:
	HRESULT initD3D(HWND hWnd);
	void SetupMatrices();

protected:
	LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       mDevice = NULL; // Our rendering device
	LPD3DXFONT				g_Font = NULL;

	std::unique_ptr<TextureManager> mTextureManager = std::make_unique<TextureManager>();
	std::unique_ptr<GeometryManager> mGeometryManager = std::make_unique<GeometryManager>();
	std::map<int, std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>> mGameObjectLayers;

	static DWORD mLastUpdateTime;
	
	D3DXMATRIXA16 mMatView;
	D3DXMATRIXA16 mMatProj;
	HWND mHwnd;
	std::unique_ptr<Camera> mMainCamera;

	
};

