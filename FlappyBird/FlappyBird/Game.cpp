#include "stdafx.h"
#include "Game.h"

float Game::mDeltaTime = 0.0f;
DWORD Game::mLastUpdateTime = 0;

Game::Game()
{
	mGameObjects.reserve(kMaxGameObjects);
	mActiveObject.reserve(kMaxBarriers);
	mCollideableLayer.reserve(kMaxGameObjects);
	mBonusLayer.reserve(kMaxGameObjects);
	
}

int Game::getRandomInt(int minValue, int maxValue)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(minValue, maxValue); // guaranteed unbiased
	return uni(rng);
}

Game::~Game()
{
}

HRESULT Game::init(HWND hWnd)
{
	
	if (FAILED(initD3D(hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(initGeometry()))
	{
		return E_FAIL;
	}
	mLastUpdateTime = timeGetTime();
	return S_OK;
}

void Game::close()
{
	mGeometryManager->clean();
	mTextureManager->clean();
	
	for (auto curObj : mGameObjects)
	{
		curObj->clean();
	}

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

HRESULT Game::initD3D(HWND hWnd)
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
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// Turn off culling
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

HRESULT Game::initGeometry()
{
	mCollideableLayer.reserve(kCollideable);

	mTextureManager->init(g_pd3dDevice);
	std::shared_ptr<Texture> mPlayerTexture = mTextureManager->createTexture("png\\bird.png");
	

	createBackground();
	//create player
	std::shared_ptr<GameObject> playerGameObj = std::make_shared<GameObject>();
	playerGameObj->init(g_pd3dDevice);
	
	std::shared_ptr<Renderable> playerSprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> playerGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, g_pd3dDevice);
	playerSprite->init(playerGeometry, mPlayerTexture);
	playerGameObj->addComponent(playerSprite);
	
	mPlayerBounds = std::make_shared<BoundingBox>();
	mPlayerBounds->init(playerGeometry->getTopLeft(), playerGeometry->getBottomRight(), nullptr);
	playerGameObj->setLocalScale(2.0f, 2.0f, 1.0f);
	playerGameObj->addComponent(mPlayerBounds);

	mPlayer = std::make_shared<Bird>();
	playerGameObj->addComponent(mPlayer);
	mPlayer->setOnPlayerCrashed([this]() {this->onPlayerCrashed(); });
	
	mGameObjects.push_back(playerGameObj);
	//==============================================
	
	for (int i = 0; i < kMaxBarriers; ++i)
	{
		int random = getRandomInt(1, 3);
		std::shared_ptr<GameObject> barrier = nullptr;
		if (random == 1)
		{
			barrier = createBarrierBottom();
		}
		else if (random == 2)
		{
			barrier = createBarrierMiddle();
		}
		else if (random == 3)
		{
			barrier = createBarrierTop();
		}
		if (barrier != nullptr)
		{
			//barrier->setEnabled(false);
			mObjectsReserve.push_back(barrier);
		}
		
	}
	
	

	SetRect(&textbox, 0, 0, 640, 480);
	D3DXCreateFont(g_pd3dDevice,    // the D3D Device
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

	return S_OK;
}

std::shared_ptr<GameObject> Game::createBarrierBottom()
{
	std::shared_ptr<Texture> barrierTexture = mTextureManager->createTexture("png\\Objects\\Crate.png");
	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(g_pd3dDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(g_pd3dDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X6, g_pd3dDevice);
	enemySprite->init(geom, barrierTexture);
	barrierBottom->addComponent(enemySprite);

	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->init(geom->getTopLeft(), geom->getBottomRight(), &mCollideableLayer);
	barrierBottom->addComponent(box);
	barrierBottom->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierBottom->setLocalPosY(0.0f);



	barrierParent->addChild(barrierBottom);
	barrierParent->setLocalPosY(-9.0f);
	barrierBottom->setLocalPosY(12.0f);



	//create bonus

	std::shared_ptr<GameObject> bonusObject = std::make_shared<GameObject>();
	bonusObject->init(g_pd3dDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, g_pd3dDevice);
	std::shared_ptr<Texture> bonusTexture = mTextureManager->createTexture("png\\Objects\\StoneBlock.png");
	std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
	renderable->init(bonusGeometry, bonusTexture);
	bonusObject->addComponent(renderable);
	std::shared_ptr<BoundingBox> bonusBounding = std::make_shared<BoundingBox>();
	bonusBounding->init(bonusGeometry->getTopLeft(), bonusGeometry->getBottomRight(), &mBonusLayer);
	bonusObject->addComponent(bonusBounding);
	barrierParent->addChild(bonusObject);
	float bottomHeight = geom->getTopLeft().y - geom->getBottomRight().y;
	bonusObject->setLocalPosY(3.0f);
	return barrierParent;
}

std::shared_ptr<GameObject> Game::createBarrierMiddle()
{
	std::shared_ptr<Texture> barrierTexture = mTextureManager->createTexture("png\\Objects\\Crate.png");
	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(g_pd3dDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(g_pd3dDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X3, g_pd3dDevice);
	enemySprite->init(geom, barrierTexture);
	barrierBottom->addComponent(enemySprite);

	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->init(geom->getTopLeft(), geom->getBottomRight(), &mCollideableLayer);
	barrierBottom->addComponent(box);
	barrierBottom->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierBottom->setLocalPosY(0.0f);

	std::shared_ptr<GameObject> barrierTop = std::make_shared<GameObject>();
	barrierTop->init(g_pd3dDevice);
	std::shared_ptr<Renderable> renderableTop = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geomTop = mGeometryManager->getGeometry(GEOMETRY::POLY_1X2, g_pd3dDevice);
	renderableTop->init(geomTop, barrierTexture);
	barrierTop->addComponent(renderableTop);
	std::shared_ptr<BoundingBox> topBox = std::make_shared<BoundingBox>();
	topBox->init(geomTop->getTopLeft(), geomTop->getBottomRight(), &mCollideableLayer);
	barrierTop->addComponent(topBox);
	barrierTop->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierTop->setLocalPosY(0.0f);

	barrierParent->addChild(barrierBottom);
	barrierParent->addChild(barrierTop);
	barrierParent->setLocalPosY(-9.0f);
	barrierBottom->setLocalPosY(3.0f);
	barrierTop->setLocalPosY(15.0f);



	//create bonus

	std::shared_ptr<GameObject> bonusObject = std::make_shared<GameObject>();
	bonusObject->init(g_pd3dDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, g_pd3dDevice);
	std::shared_ptr<Texture> bonusTexture = mTextureManager->createTexture("png\\Objects\\StoneBlock.png");
	std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
	renderable->init(bonusGeometry, bonusTexture);
	bonusObject->addComponent(renderable);
	std::shared_ptr<BoundingBox> bonusBounding = std::make_shared<BoundingBox>();
	bonusBounding->init(bonusGeometry->getTopLeft(), bonusGeometry->getBottomRight(), &mBonusLayer);
	bonusObject->addComponent(bonusBounding);
	barrierParent->addChild(bonusObject);
	float bottomHeight = geom->getTopLeft().y - geom->getBottomRight().y;
	bonusObject->setLocalPosY(bottomHeight + barrierBottom->getLocalPosY() + 3.0f);

	return barrierParent;
}

std::shared_ptr<GameObject> Game::createBarrierTop()
{
	std::shared_ptr<Texture> barrierTexture = mTextureManager->createTexture("png\\Objects\\Crate.png");

	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(g_pd3dDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(g_pd3dDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X6, g_pd3dDevice);
	enemySprite->init(geom, barrierTexture);
	barrierBottom->addComponent(enemySprite);

	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->init(geom->getTopLeft(), geom->getBottomRight(), &mCollideableLayer);
	barrierBottom->addComponent(box);
	barrierBottom->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierBottom->setLocalPosY(0.0f);



	barrierParent->addChild(barrierBottom);
	barrierParent->setLocalPosY(-9.0f);
	barrierBottom->setLocalPosY(6.0f);



	//create bonus

	std::shared_ptr<GameObject> bonusObject = std::make_shared<GameObject>();
	bonusObject->init(g_pd3dDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, g_pd3dDevice);
	std::shared_ptr<Texture> bonusTexture = mTextureManager->createTexture("png\\Objects\\StoneBlock.png");
	std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
	renderable->init(bonusGeometry, bonusTexture);
	bonusObject->addComponent(renderable);
	std::shared_ptr<BoundingBox> bonusBounding = std::make_shared<BoundingBox>();
	bonusBounding->init(bonusGeometry->getTopLeft(), bonusGeometry->getBottomRight(), &mBonusLayer);
	bonusObject->addComponent(bonusBounding);
	barrierParent->addChild(bonusObject);
	float bottomHeight = geom->getTopLeft().y - geom->getBottomRight().y;
	bonusObject->setLocalPosY(bottomHeight + barrierBottom->getLocalPosY() + 3.0f);

	return barrierParent;
}

void Game::createBackground()
{
	float startX = -kBackgroundWidth;
	float halfWidth = kBackgroundWidth/2.0f;
	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<Texture> texture = mTextureManager->createTexture("png\\BG.png");

		std::shared_ptr<GameObject> background = std::make_shared<GameObject>();
		background->init(g_pd3dDevice);
		std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
		renderable->init(mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, g_pd3dDevice), texture);
		background->addComponent(renderable);
		background->setLocalScale(kBackgroundWidth, kBackgroundWidth, 1.0f);
		background->addLocalPos(startX + 2.0f* halfWidth*(float)i, 0.0f, 1.0f);
		
		mBackgroundObjects.push_back(background);
	}
	

}

void Game::SetupMatrices()
{
	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationX(&matWorld, timeGetTime() / 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(mPlayer->getGameObject()->getLocalPosX(), 0.0f, -25.0f);
	D3DXVECTOR3 vLookatPt(mPlayer->getGameObject()->getLocalPosX(), 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 640.0f/480.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Game::update()
{
	if (mIsOnMenu)
	{
		return;
	}

	DWORD curTime = timeGetTime();
	mDeltaTime = (curTime - mLastUpdateTime) / 1000.0f;
	mLastUpdateTime = curTime;

	for (auto curObj : mGameObjects)
	{
		curObj->update();
	}

	mTimeSinceLastBarrierSpawn += mDeltaTime;
	if (mTimeSinceLastBarrierSpawn >= kBarrierTimeSpawn)
	{
		if (!mObjectsReserve.empty())
		{
			int randomIndex = getRandomInt(0, mObjectsReserve.size() - 1);
			
			auto curObj = mObjectsReserve[randomIndex];
			mObjectsReserve[randomIndex] = mObjectsReserve[mObjectsReserve.size() - 1];
			mObjectsReserve.pop_back();
			curObj->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kBarrierXStartOffset);
			curObj->setEnabled(true);
			mActiveObject.insert(curObj);
			mTimeSinceLastBarrierSpawn = 0.0f;
		}
		
		
	}
	
	checkCollideables();

	for (unsigned int i = 0; i < mBackgroundObjects.size(); ++i)
	{

		float displacement = kPlayerSpeed * mDeltaTime;
		mBackgroundObjects[i]->addLocalPosX(displacement);


	}
}

void Game::checkCollideables()
{
	const BoundingBox* const playerBoundingBox = mPlayerBounds.get();
	
	for (auto curBoundingBox : mCollideableLayer)
	{
		if (curBoundingBox->isIntersect(playerBoundingBox))
			
		{
			mTestText = "object collided!";
			return;
		}
	}
	

	for (auto curBonusBox : mBonusLayer)
	{
		if (curBonusBox->isIntersect(playerBoundingBox)/* || playerBoundingBox->isIntersect(curBonusBox)*/)
		{
			curBonusBox->getGameObject()->setEnabled(false);
			mTestText = "bonus acheived!";
			return;
		}
	}
	
	
}

void Game::render()
{
	// Clear the backbuffer and the zbuffer
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		

		
		// Setup the world, view, and projection matrices
		SetupMatrices();
		
		
		for (unsigned int i=0; i < mBackgroundObjects.size(); ++i)
		{
			mBackgroundObjects[i]->draw();
		}
		
		
		std::list<std::shared_ptr<GameObject>> removeList;
		for (auto curObj : mActiveObject)
		{
			
			curObj->draw();
			if ((mPlayer->getGameObject()->getLocalPosX() - curObj->getLocalPosX()) >= kObjLiveMaxDistance)
			{
				removeList.push_back(curObj);
			}

			
		}
		
		for (auto curRemove : removeList)
		{
			mActiveObject.erase(curRemove);
			mObjectsReserve.push_back(curRemove);
		}
		
		for (auto curObj : mGameObjects)
		{
			curObj->draw();
		}
		g_Font->DrawTextA(NULL,
			mTestText.c_str(),
			strlen(mTestText.c_str()),
			&textbox,
			DT_LEFT | DT_TOP,
			D3DCOLOR_RGBA(255, 1, 1, 255));

		// End the scene
		g_pd3dDevice->EndScene();
	}



	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Game::processInput(WPARAM wParam)
{
	
	if (wParam == VK_UP)
	{
		mPlayer->addUpImpulse();
	}

	else if (wParam == VK_DOWN)
	{
		mPlayer->getGameObject()->addLocalPos(0.0f, -1.0f, 0.0f);
	}

	else if (wParam == VK_LEFT)
	{
		mPlayer->getGameObject()->addLocalPos(-1.0f, 0.0f, 0.0f);
	}

	else if (wParam == VK_RIGHT)
	{
		mPlayer->getGameObject()->addLocalPos(1.0f, 0.0f, 0.0f);
	}
	else
	{

	}
	
}

void Game::onPlayerCrashed()
{
	mIsOnMenu = true;
}