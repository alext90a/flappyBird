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

protected:
	HRESULT initD3D(HWND hWnd);
	HRESULT initGeometry();
	void SetupMatrices();

	void checkCollideables();

protected:
	LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
	LPD3DXFONT				g_Font = NULL;
	//LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture
	std::unique_ptr<TextureManager> mTextureManager = std::make_unique<TextureManager>();
	std::shared_ptr<Texture> mBananaTexture = nullptr;
	std::shared_ptr<GameObject> mPlayer = nullptr;
	std::shared_ptr<BoundingBox> mPlayerBounds = nullptr;

	std::string mTestText = "Hello word!";
	RECT textbox;

	std::vector<const BoundingBox*> mCollideableStore;

	std::vector<std::shared_ptr<GameObject>> mGameObjects;
};

