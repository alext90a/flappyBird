#include "stdafx.h"

float GameEngine::mDeltaTime = 0.0f;
DWORD GameEngine::mLastUpdateTime = 0;

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{
}

HRESULT GameEngine::init(HWND hWnd)
{
	mHwnd = hWnd;
	if (FAILED(initD3D(hWnd)))
	{
		return E_FAIL;
	}
	mTextureManager->init(mDevice);

	D3DXCreateFont(mDevice,    // the D3D Device
		24,    // font height
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		false,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		"Arial",    // use Facename Arial
		&g_Font);    // the font object
	
	mLastUpdateTime = timeGetTime();
	mMainCamera = std::make_unique<Camera>();
	return S_OK;
}

void GameEngine::close()
{
	GameObject::mObjects;
	
	
	for (auto curLayer : mGameObjectLayers)
	{
		std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> curVectorPtr = curLayer.second;
		for (auto curObj : *curVectorPtr)
		{
			curObj->clean();
			curObj.reset();
		}
		
	}
	mGameObjectLayers.clear();

	

	mGeometryManager->clean();
	mTextureManager->clean();
	
	GameObject::mObjects;

	
	if (g_Font != NULL)
	{
		g_Font->Release();
	}
	

	if (mDevice != NULL)
		mDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
	int objects = GameObject::mObjects;
	Component::mAlived;
	
	ExitProcess(0);
}

HRESULT GameEngine::initD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &mDevice)))
	{
		return E_FAIL;
	}

	// Turn off culling
	mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting
	mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	mDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

void GameEngine::setCameraParams(const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookPos)
{
	mMainCamera->setCameraEyePos(eyePos);
	mMainCamera->setCameraLookPos(lookPos);
}



void GameEngine::SetupMatrices()
{
	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationX(&matWorld, timeGetTime() / 1000.0f);
	mDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.

	
	
	D3DXMatrixLookAtLH(&mMatView, mMainCamera->getCameraEyePos(), mMainCamera->getCameraLookAt(), mMainCamera->getCamerUpVec());
	mDevice->SetTransform(D3DTS_VIEW, &mMatView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	
	D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, kGameWidth/kGameHeight, 1.0f, 100.0f);
	mDevice->SetTransform(D3DTS_PROJECTION, &mMatProj);
}

void GameEngine::update()
{
	

	DWORD curTime = timeGetTime();
	mDeltaTime = (curTime - mLastUpdateTime) / 1000.0f;
	mLastUpdateTime = curTime;

	for (auto curLayer : mGameObjectLayers)
	{
		for (auto curObj : *curLayer.second)
		{
			curObj->update();
		}
		
	}



	
	
}





void GameEngine::render()
{
	// Clear the backbuffer and the zbuffer
	mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(mDevice->BeginScene()))
	{
		

		
		// Setup the world, view, and projection matrices
		SetupMatrices();
		

		

		
		for (auto curLayer : mGameObjectLayers)
		{
			for (auto curObj : *curLayer.second)
			{
				curObj->draw();
			}
			
		}



		

		
		// End the scene
		mDevice->EndScene();
	}



	// Present the backbuffer contents to the display
	mDevice->Present(NULL, NULL, NULL, NULL);
}



void GameEngine::addGameObject(const int layer, std::shared_ptr<GameObject> gameObject)
{
	mGameObjectLayers[layer]->push_back(gameObject);
}

void GameEngine::addLayer(const int layer)
{
	mGameObjectLayers.insert(std::make_pair(layer, std::make_shared<std::vector<std::shared_ptr<GameObject>>>()));
}

LPDIRECT3DDEVICE9 GameEngine::getDevice()
{
	return mDevice;
}

std::shared_ptr<Texture> GameEngine::getTexture(const std::string& name)
{
	return mTextureManager->getTexture(name);
}

std::shared_ptr<Geometry> GameEngine::getGeometry(GEOMETRY geoType)
{
	return mGeometryManager->getGeometry(geoType, mDevice);
}

LPD3DXFONT GameEngine::getFont()
{
	return g_Font;
}

void GameEngine::getMouseWorldRay(D3DXVECTOR3& pickRayDir, D3DXVECTOR3& pickRayOrig)
{
	IDirect3DSurface9 *offscreenSurface = 0;
	HRESULT hr = mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &offscreenSurface);
	D3DSURFACE_DESC surfDesc;
	offscreenSurface->GetDesc(&surfDesc);
	int width = surfDesc.Width;
	int height = surfDesc.Height;

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(mHwnd, &ptCursor);

	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x = (((2.0f * ptCursor.x) / width) - 1) / mMatProj._11;
	v.y = -(((2.0f * ptCursor.y) / height) - 1) / mMatProj._22;
	v.z = 1.0f;

	// Get the inverse view matrix

	D3DXMATRIX matWorld;
	mDevice->GetTransform(D3DTS_WORLD, &matWorld);
	D3DXMATRIX mWorldView = mMatView;
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &mMatView);

	// Transform the screen space pick ray into 3D space
	
	pickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	pickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	pickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	pickRayOrig.x = m._41;
	pickRayOrig.y = m._42;
	pickRayOrig.z = m._43;



	D3DXVec3Normalize(&pickRayDir, &pickRayDir);
}



