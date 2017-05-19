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
	mButtons.reserve(kMaxGameObjects);
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
	mHwnd = hWnd;
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

	if (mDevice != NULL)
		mDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
	ExitProcess(0);
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

HRESULT Game::initGeometry()
{
	mCollideableLayer.reserve(kCollideable);

	mTextureManager->init(mDevice);
	std::shared_ptr<Texture> mPlayerTexture = mTextureManager->createTexture("png\\bird.png");
	

	createBackground();
	//create player
	std::shared_ptr<GameObject> playerGameObj = std::make_shared<GameObject>();
	playerGameObj->init(mDevice);
	
	std::shared_ptr<Renderable> playerSprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> playerGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
	playerSprite->init(playerGeometry, mPlayerTexture);
	playerGameObj->addComponent(playerSprite);
	
	mPlayerBounds = std::make_shared<BoundingBox>();
	mPlayerBounds->init(playerGeometry->getTopLeft(), playerGeometry->getBottomRight(), nullptr);
	playerGameObj->setLocalScale(2.0f, 2.0f, 1.0f);
	playerGameObj->addComponent(mPlayerBounds);

	mPlayer = std::make_shared<Bird>();
	playerGameObj->addComponent(mPlayer);
	mPlayer->start();
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
			barrier->setEnabled(false);
			mObjectsReserve.push_back(barrier);
		}
		
	}
	

	createMainMenu();
	

	SetRect(&textbox, 0, 0, kGameWidth, kGameHeight);
	SetRect(&mScoreRect, kGameWidth/2, 0, kGameWidth, 30);
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

	return S_OK;
}

std::shared_ptr<GameObject> Game::createBarrierBottom()
{
	std::shared_ptr<Texture> barrierTexture = mTextureManager->createTexture("png\\Objects\\Crate.png");
	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(mDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X6, mDevice);
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
	bonusObject->init(mDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
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
	barrierParent->init(mDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X3, mDevice);
	enemySprite->init(geom, barrierTexture);
	barrierBottom->addComponent(enemySprite);

	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->init(geom->getTopLeft(), geom->getBottomRight(), &mCollideableLayer);
	barrierBottom->addComponent(box);
	barrierBottom->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierBottom->setLocalPosY(0.0f);

	std::shared_ptr<GameObject> barrierTop = std::make_shared<GameObject>();
	barrierTop->init(mDevice);
	std::shared_ptr<Renderable> renderableTop = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geomTop = mGeometryManager->getGeometry(GEOMETRY::POLY_1X2, mDevice);
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
	bonusObject->init(mDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
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
	barrierParent->init(mDevice);

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mDevice);
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGeometryManager->getGeometry(GEOMETRY::POLY_1X6, mDevice);
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
	bonusObject->init(mDevice);
	std::shared_ptr<Geometry> bonusGeometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
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
		background->init(mDevice);
		std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
		renderable->init(mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice), texture);
		background->addComponent(renderable);
		background->setLocalScale(kBackgroundWidth, kBackgroundWidth*1.2f, 1.0f);
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
	mDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(mPlayer->getGameObject()->getLocalPosX(), 0.0f, -25.0f);
	D3DXVECTOR3 vLookatPt(mPlayer->getGameObject()->getLocalPosX(), 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	
	D3DXMatrixLookAtLH(&mMatView, &vEyePt, &vLookatPt, &vUpVec);
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

void Game::update()
{
	

	DWORD curTime = timeGetTime();
	mDeltaTime = (curTime - mLastUpdateTime) / 1000.0f;
	mLastUpdateTime = curTime;
	if (mIsOnMenu)
	{
		return;
	}
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
			curObj->setEnabled(true);
			curObj->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kBarrierXStartOffset);
			
			mActiveObject.insert(curObj);
			mTimeSinceLastBarrierSpawn = 0.0f;
		}
		
		
	}
	
	checkCollideables();

	for (unsigned int i = 0; i < mBackgroundObjects.size(); ++i)
	{

		float displacement = mPlayer->getSpeed() * mDeltaTime;
		mBackgroundObjects[i]->addLocalPosX(displacement);


	}
}

void Game::checkCollideables()
{
	const BoundingBox* const playerBoundingBox = mPlayerBounds.get();
	
	for (auto curBoundingBox : mCollideableLayer)
	{
		if (!curBoundingBox->getGameObject()->isEnabled())
		{
			continue;
		}

		if (curBoundingBox->isIntersect(playerBoundingBox))
			
		{
			mTestText = "object collided!";
			mPlayer->setFallDawnState();
			return;
		}
	}
	

	for (auto curBonusBox : mBonusLayer)
	{
		if (!curBonusBox->getGameObject()->isEnabled())
		{
			continue;
		}
		if (curBonusBox->isIntersect(playerBoundingBox)/* || playerBoundingBox->isIntersect(curBonusBox)*/)
		{
			curBonusBox->getGameObject()->setEnabled(false);
			mPlayer->addScore(1);
			updateScoreText();
			mTestText = "bonus acheived!";
			return;
		}
	}
	
	
}

void Game::updateScoreText()
{
	
	mScoreText = "Score: " + std::to_string(mPlayer->getScore());
}

void Game::render()
{
	// Clear the backbuffer and the zbuffer
	mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(mDevice->BeginScene()))
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

		if (mIsOnMenu)
		{
			mMainMenu->setLocalPosX(mPlayer->getGameObject()->getLocalPosX());
			mMainMenu->draw();
		}
		g_Font->DrawTextA(NULL,
			mTestText.c_str(),
			strlen(mTestText.c_str()),
			&textbox,
			DT_LEFT | DT_TOP,
			D3DCOLOR_RGBA(255, 1, 1, 255));

		g_Font->DrawTextA(NULL,
			mScoreText.c_str(),
			strlen(mTestText.c_str()),
			&mScoreRect,
			DT_LEFT | DT_TOP,
			D3DCOLOR_RGBA(255, 1, 1, 255));

		// End the scene
		mDevice->EndScene();
	}



	// Present the backbuffer contents to the display
	mDevice->Present(NULL, NULL, NULL, NULL);
}

void Game::processInput(WPARAM wParam, LPARAM lParam)
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
	else if (wParam == VK_SPACE)
	{
		if (mIsOnMenu)
		{
			startPlay();
		}
	}
	else if (wParam == MK_LBUTTON)
	{
		if (mIsOnMenu)
		{
			IDirect3DSurface9 *offscreenSurface = 0;
			HRESULT hr = mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &offscreenSurface);
			D3DSURFACE_DESC surfDesc;
			offscreenSurface->GetDesc(&surfDesc);
			float width = surfDesc.Width;
			float height = surfDesc.Height;

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
			D3DXVECTOR3 vPickRayDir, vPickRayOrig;
			vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
			vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
			vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
			vPickRayOrig.x = m._41;
			vPickRayOrig.y = m._42;
			vPickRayOrig.z = m._43;

			
			//D3DXVECTOR3 rayDir = rayend - rayorigin;

			D3DXVec3Normalize(&vPickRayDir, &vPickRayDir);

			for (auto curButton : mButtons)
			{
				if (curButton->checkClick(&vPickRayOrig, &vPickRayDir))
				{
					break;
				}
			}
		}
		
		int i = 0;
	}
	else
	{

	}
	
}

void Game::onPlayerFallDawn()
{

}

void Game::onPlayerCrashed()
{
	mIsOnMenu = true;
}

void Game::startPlay()
{
	mIsOnMenu = false;
	mPlayer->start();
	for (auto curObj : mActiveObject)
	{
		curObj->setEnabled(false);
		mObjectsReserve.push_back(curObj);
	}
	mActiveObject.clear();

	float startX = -kBackgroundWidth;
	float halfWidth = kBackgroundWidth / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		mBackgroundObjects[i]->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		mBackgroundObjects[i]->addLocalPos(startX + 2.0f* halfWidth*(float)i, 0.0f, 1.0f);
	}

	updateScoreText();
}

void Game::createMainMenu()
{

	mMainMenu = std::make_shared<GameObject>();
	std::shared_ptr<Geometry> geometry = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
	std::shared_ptr<Texture> texture = mTextureManager->createTexture("png\\Menu.png");
	std::shared_ptr<Renderable> render = std::make_shared<Renderable>();

	mMainMenu->init(mDevice);
	render->init(geometry, texture);
	mMainMenu->addComponent(render);
	mMainMenu->setLocalScale(13.0f, 18.0f, 1.0f);

	float startY = 4.0f;
	float buttonHeight = 3.0f;
	float buttonSpace = 1.0f;
	std::vector<std::string> buttonNames = { "png\\button_play.png", "png\\button_scores.png", "png\\button_exit.png" };
	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<GameObject> buttonObj = std::make_shared<GameObject>();
		std::shared_ptr<Geometry> buttonGeo = mGeometryManager->getGeometry(GEOMETRY::POLY_1X1, mDevice);
		std::shared_ptr<Texture> buttonTex = mTextureManager->createTexture(buttonNames[i]);
		std::shared_ptr<Renderable> buttonRender = std::make_shared<Renderable>();
		std::shared_ptr<BoundingBox> boundingBox = std::make_shared<BoundingBox>();
		std::shared_ptr<Button> button = std::make_shared<Button>();
		

		buttonObj->init(mDevice);
		buttonRender->init(buttonGeo, buttonTex);
		boundingBox->init(buttonGeo->getTopLeft(), buttonGeo->getBottomRight(), nullptr);
		button->init(boundingBox.get());
		
		buttonObj->addComponent(buttonRender);
		buttonObj->addComponent(boundingBox);
		buttonObj->addComponent(button);
		
		buttonObj->setWorldScale(8.0f, buttonHeight, 1.0f);
		buttonObj->setLocalPos(0.0f, startY - (buttonHeight + buttonSpace)*i, -0.1f);
		mMainMenu->addChild(buttonObj);
		mButtons.push_back(button);
	}
	mButtons[0]->setFunc([this]() {this->startPlay(); });
	mButtons[2]->setFunc([this]() {this->close(); });



	mMainMenu->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
}