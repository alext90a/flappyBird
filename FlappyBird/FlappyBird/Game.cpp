#include "stdafx.h"
#include "Game.h"



Game::Game()
{
	
	


	mBarriersActive.reserve(kMaxBarriers);
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
	mGameEngine = std::make_unique<GameEngine>();
	if (FAILED(mGameEngine->init(hWnd)))
	{
		return E_FAIL;
	}
	for (auto curLayer : kGameLayers)
	{
		mGameEngine->addLayer(curLayer);
	}

	if (FAILED(initGeometry()))
	{
		return E_FAIL;
	}
	showMainMenu();
	
	return S_OK;
}

void Game::close()
{
	


	mBarriersActive.clear();
	mBonusLayer.clear();
	mCollideableLayer.clear();
	mBarriersReserve.clear();
	mButtons.clear();





	
	
	
	mPlayer = nullptr;
	mPlayerBounds = nullptr;
	mPlayerHud = nullptr;
	mBackgroundController = nullptr;
	mGroundController = nullptr;
	mMainMenu = nullptr;
	mHighScoreDialog = nullptr;
	mBackgroundController = nullptr;
	
	GameObject::mObjects;

	
	mGameEngine->close();
	
	Component::mAlived;
	GameObject::mObjects;
	
	int i = 0;

}


HRESULT Game::initGeometry()
{


	mCollideableLayer.reserve(kCollideable);

	std::shared_ptr<Texture> mPlayerTexture = mGameEngine->getTexture("png\\bird.png");
	
	

	
	
	//create player
	std::shared_ptr<GameObject> playerGameObj = std::make_shared<GameObject>();
	playerGameObj->init(mGameEngine->getDevice());
	
	std::shared_ptr<Renderable> playerSprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> playerGeometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	playerSprite->init(playerGeometry, mPlayerTexture);
	playerGameObj->addComponent(playerSprite);
	
	
	mPlayerBounds = std::make_shared<BoundingBox>();
	mPlayerBounds->init(playerGeometry->getTopLeft(), playerGeometry->getBottomRight(), nullptr);
	playerGameObj->setLocalScale(2.0f, 2.0f, 1.0f);
	playerGameObj->addComponent(mPlayerBounds);
	
	
	mPlayer = std::make_shared<Bird>();
	playerGameObj->addComponent(mPlayer);
	mPlayer->setOnPlayerCrashed([this]() {this->onPlayerCrashed(); });
	
	mGameEngine->addGameObject(kPlayerLayer, playerGameObj);
	
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
			mBarriersReserve.push_back(barrier);
			mGameEngine->addGameObject(kBarrierLayer, barrier);
		}
		
	}

	//create ground
	std::shared_ptr<GameObject> groundParentObject(std::make_shared<GameObject>());
	groundParentObject->init(mGameEngine->getDevice());
	mGroundController = std::make_shared<GroundObjectController>();
	groundParentObject->addComponent(mGroundController);
	mGroundController->setPlayer(mPlayer);
	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<GameObject> groundObject(std::make_shared<GameObject>());
		std::shared_ptr<Geometry> groundGeo(mGameEngine->getGeometry(GEOMETRY::POLY_10X1));
		std::shared_ptr<Texture> groundTexture(mGameEngine->getTexture("png\\Tile\\2.png"));
		std::shared_ptr<Renderable> groundRender(std::make_shared<Renderable>());
		groundObject->init(mGameEngine->getDevice());
		groundRender->init(groundGeo, groundTexture);
		groundObject->addComponent(groundRender);
		groundObject->setLocalScale(2.0f, 2.0f, 1.0f);
		groundObject->setLocalPosY(-10.0f);
		groundObject->setLocalPosX(kGroundWidth*i);
		
		
		groundParentObject->addChild(groundObject);
		mGroundController->addGroundObject(groundObject.get());
	}
	mGameEngine->addGameObject(kGroundLayer, groundParentObject);

	createBackground();
	createMainMenu();
	createHighscoreMenu();
	createPlayerHud();


	
	return S_OK;
}

void Game::createPlayerHud()
{
	std::shared_ptr<GameObject> hudObject(std::make_shared<GameObject>());
	hudObject->init(mGameEngine->getDevice());
	std::shared_ptr<TextComponent> scoreText(std::make_shared<TextComponent>());
	std::shared_ptr<TextComponent> messageText(std::make_shared<TextComponent>());
	scoreText->setPos(kGameWidth / 2, 0, kGameWidth, 30);
	scoreText->init(mGameEngine->getFont());
	scoreText->setColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	hudObject->addComponent(scoreText);
	messageText->setPos(0, 0, kGameWidth, kGameHeight);
	messageText->init(mGameEngine->getFont());
	messageText->setColor(D3DCOLOR_RGBA(0, 255, 0, 255));
	hudObject->addComponent(messageText);

	mPlayerHud = std::make_shared<PlayerHud>();
	mPlayerHud->setMessageComponent(messageText);
	mPlayerHud->setScoreComponent(scoreText);
	hudObject->addComponent(mPlayerHud);
	mGameEngine->addGameObject(kMenuLayer, hudObject);

	mPlayerHud->show(true);
}

std::shared_ptr<GameObject> Game::createBarrierBottom()
{
	std::shared_ptr<Texture> barrierTexture = mGameEngine->getTexture("png\\Objects\\Crate.png");
	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(mGameEngine->getDevice());

	
	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mGameEngine->getDevice());
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGameEngine->getGeometry(GEOMETRY::POLY_1X6);
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
	bonusObject->init(mGameEngine->getDevice());
	std::shared_ptr<Geometry> bonusGeometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> bonusTexture = mGameEngine->getTexture("png\\Objects\\StoneBlock.png");
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
	std::shared_ptr<Texture> barrierTexture = mGameEngine->getTexture("png\\Objects\\Crate.png");
	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(mGameEngine->getDevice());

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mGameEngine->getDevice());
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGameEngine->getGeometry(GEOMETRY::POLY_1X3);
	enemySprite->init(geom, barrierTexture);
	barrierBottom->addComponent(enemySprite);

	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->init(geom->getTopLeft(), geom->getBottomRight(), &mCollideableLayer);
	barrierBottom->addComponent(box);
	barrierBottom->setLocalScale(2.0f, 2.0f, 1.0f);
	barrierBottom->setLocalPosY(0.0f);

	std::shared_ptr<GameObject> barrierTop = std::make_shared<GameObject>();
	barrierTop->init(mGameEngine->getDevice());
	std::shared_ptr<Renderable> renderableTop = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geomTop = mGameEngine->getGeometry(GEOMETRY::POLY_1X2);
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
	bonusObject->init(mGameEngine->getDevice());
	std::shared_ptr<Geometry> bonusGeometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> bonusTexture = mGameEngine->getTexture("png\\Objects\\StoneBlock.png");
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
	std::shared_ptr<Texture> barrierTexture = mGameEngine->getTexture("png\\Objects\\Crate.png");

	std::shared_ptr<GameObject> barrierParent = std::make_shared<GameObject>();
	barrierParent->init(mGameEngine->getDevice());

	std::shared_ptr<GameObject> barrierBottom = std::make_shared<GameObject>();
	barrierBottom->init(mGameEngine->getDevice());
	std::shared_ptr<Renderable> enemySprite = std::make_shared<Renderable>();
	std::shared_ptr<Geometry> geom = mGameEngine->getGeometry(GEOMETRY::POLY_1X6);
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
	bonusObject->init(mGameEngine->getDevice());
	std::shared_ptr<Geometry> bonusGeometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> bonusTexture = mGameEngine->getTexture("png\\Objects\\StoneBlock.png");
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
	std::shared_ptr<GameObject> backgroundParent(std::make_shared<GameObject>());
	mBackgroundController = std::make_shared<BackgroundController>();
	backgroundParent->init(mGameEngine->getDevice());
	backgroundParent->addComponent(mBackgroundController);


	std::shared_ptr<Texture> texture = mGameEngine->getTexture("png\\BG.png");

	std::shared_ptr<GameObject> background = std::make_shared<GameObject>();
	background->init(mGameEngine->getDevice());
	std::shared_ptr<Renderable> renderable = std::make_shared<Renderable>();
	renderable->init(mGameEngine->getGeometry(GEOMETRY::POLY_1X1), texture);
	background->addComponent(renderable);
	background->setLocalScale(kBackgroundWidth, kBackgroundWidth, 1.0f);
	background->addLocalPos(0, 0.0f, 1.0f);

	backgroundParent->addChild(background);
	mBackgroundController->insertBackgroundObject(background);

	mBackgroundController->setPlayer(mPlayer);
	mGameEngine->addGameObject(kBackgroundLayer, backgroundParent);

}



void Game::update()
{
	
	mGameEngine->setCameraParams(D3DXVECTOR3(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset, 0.0f, -25.0f),
		D3DXVECTOR3(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset, 0.0f, 0.0f));
	mGameEngine->update();
	if (mIsOnMenu)
	{
		return;
	}


	mTimeSinceLastBarrierSpawn += GameEngine::mDeltaTime;
	if (mTimeSinceLastBarrierSpawn >= kBarrierTimeSpawn)
	{
		if (!mBarriersReserve.empty())
		{
			int randomIndex = getRandomInt(0, mBarriersReserve.size() - 1);
			
			auto curObj = mBarriersReserve[randomIndex];
			mBarriersReserve[randomIndex] = mBarriersReserve[mBarriersReserve.size() - 1];
			mBarriersReserve.pop_back();
			curObj->setEnabled(true);
			curObj->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kBarrierXStartOffset);
			
			mBarriersActive.insert(curObj);
			mTimeSinceLastBarrierSpawn = 0.0f;
		}
		
		
	}
	
	checkCollideables();

	std::list<std::shared_ptr<GameObject>> removeList;
	for (auto curObj : mBarriersActive)
	{

		//curObj->draw();
		if ((mPlayer->getGameObject()->getLocalPosX() - curObj->getLocalPosX()) >= kObjLiveMaxDistance)
		{
			curObj->setEnabled(false);
			removeList.push_back(curObj);
		}


	}

	for (auto curRemove : removeList)
	{
		mBarriersActive.erase(curRemove);
		mBarriersReserve.push_back(curRemove);
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
			mPlayerHud->setMessage("object collided!");
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
		if (curBonusBox->isIntersect(playerBoundingBox))
		{
			curBonusBox->getGameObject()->setEnabled(false);
			mPlayer->addScore(1);
			updateScoreText();
			mPlayerHud->setMessage("bonus acheived!");
			return;
		}
	}
	
	
}

void Game::updateScoreText()
{
	
	mPlayerHud->setScore("Score: " + std::to_string(mPlayer->getScore()));
}

void Game::render()
{
	mGameEngine->render();
}

void Game::processInput(WPARAM wParam, LPARAM lParam)
{
	
	if (wParam == VK_UP)
	{
		mPlayer->addUpImpulse();
	}


	else if (wParam == MK_LBUTTON)
	{
		if (mIsOnMenu)
		{

			D3DXVECTOR3 pickRayOrig, pickRayDir;
			mGameEngine->getMouseWorldRay(pickRayDir, pickRayOrig);
			
			for (auto curButton : mButtons)
			{
				if (curButton->checkClick(&pickRayOrig, &pickRayDir))
				{
					break;
				}
			}
			
		}
		else
		{
			mPlayer->addUpImpulse();
		}
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
	if (mPlayer->getScore() > mHighScoreDialog->getMinScore())
	{
		mHighScoreDialog->showDialog("Player", mPlayer->getScore());
		mHighScoreDialog->getGameObject()->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset);
	}
	else
	{
		showMainMenu();
	}
	
}

void Game::startPlay()
{
	mIsOnMenu = false;
	mPlayerHud->show(true);
	mPlayer->start();
	for (auto curObj : mBarriersActive)
	{
		curObj->setEnabled(false);
		mBarriersReserve.push_back(curObj);
	}
	mBarriersActive.clear();

	mGroundController->setStartPos();

	mMainMenu->setEnabled(false);
	updateScoreText();
	mTimeSinceLastBarrierSpawn = 0.0f;
}

void Game::createMainMenu()
{
	
	mMainMenu = std::make_shared<GameObject>();
	std::shared_ptr<Geometry> geometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> texture = mGameEngine->getTexture("png\\Menu.png");
	std::shared_ptr<Renderable> render = std::make_shared<Renderable>();

	mMainMenu->init(mGameEngine->getDevice());
	render->init(geometry, texture);
	mMainMenu->addComponent(render);
	mMainMenu->setLocalScale(13.0f, 18.0f, 1.0f);

	float startY = 4.0f;

	float buttonSpace = 1.0f;
	std::vector<std::string> buttonNames = { "png\\button_play.png", "png\\button_scores.png", "png\\button_exit.png" };
	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<GameObject> buttonObj = std::make_shared<GameObject>();
		std::shared_ptr<Geometry> buttonGeo = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
		std::shared_ptr<Texture> buttonTex = mGameEngine->getTexture(buttonNames[i]);
		std::shared_ptr<Renderable> buttonRender = std::make_shared<Renderable>();
		std::shared_ptr<BoundingBox> boundingBox = std::make_shared<BoundingBox>();
		std::shared_ptr<Button> button = std::make_shared<Button>();
		

		buttonObj->init(mGameEngine->getDevice());
		buttonRender->init(buttonGeo, buttonTex);
		boundingBox->init(buttonGeo->getTopLeft(), buttonGeo->getBottomRight(), nullptr);
		button->init(boundingBox.get());
		
		buttonObj->addComponent(buttonRender);
		buttonObj->addComponent(boundingBox);
		buttonObj->addComponent(button);
		
		buttonObj->setWorldScale(8.0f, kButtonHeight, 1.0f);
		buttonObj->setLocalPos(0.0f, startY - (kButtonHeight + buttonSpace)*i, -0.1f);
		mMainMenu->addChild(buttonObj);
		mButtons.push_back(button.get());
	}
	mButtons[0]->setFunc([this]() {this->startPlay(); });
	mButtons[1]->setFunc([this]() {
		this->showHighScore(); 
	});
	mButtons[2]->setFunc([this]() {SendMessage(mGameEngine->getHwnd(), WM_DESTROY, 0, 0); });



	mMainMenu->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	mMainMenu->setEnabled(false);
	mGameEngine->addGameObject(kMenuLayer, mMainMenu);
}

void Game::createHighscoreMenu()
{
	
	std::shared_ptr<GameObject> highScoreMenuObject(std::make_shared<GameObject>());
	std::shared_ptr<Geometry> geometry = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> texture = mGameEngine->getTexture("png\\Menu.png");
	std::shared_ptr<Renderable> render = std::make_shared<Renderable>();
	mHighScoreDialog = std::make_shared<HighScoreDialog>();

	highScoreMenuObject->init(mGameEngine->getDevice());
	render->init(geometry, texture);
	highScoreMenuObject->addComponent(render);
	highScoreMenuObject->setLocalScale(13.0f, 18.0f, 1.0f);
	highScoreMenuObject->addComponent(mHighScoreDialog);

	std::shared_ptr<GameObject> highScoreImgObj = std::make_shared<GameObject>();
	std::shared_ptr<Renderable> highScoreImgRend = std::make_shared<Renderable>();
	highScoreImgRend->init(mGameEngine->getGeometry(GEOMETRY::POLY_1X1), mGameEngine->getTexture("png\\highscore.png"));
	highScoreImgObj->init(mGameEngine->getDevice());
	highScoreImgObj->addComponent(highScoreImgRend);
	highScoreImgObj->setLocalScale(3.0f, 5.0f, 1.0f);
	highScoreImgObj->setLocalPos(D3DXVECTOR3(0.0f, 6.0f, -0.1f));
	highScoreMenuObject->addChild(highScoreImgObj);


	for (int i = 0; i < kHighscoreLines; ++i)
	{
		std::shared_ptr<TextComponent> nameTextComponent = std::make_shared<TextComponent>();
		nameTextComponent->init(mGameEngine->getFont());
		nameTextComponent->setText("Computer");
		//textComponent->setPos(kGameWidth / 2.0f - 100.0f, kGameHeight / 2 - 100.0f + 40 * i, kGameWidth + 100, kGameHeight / 2.0f - 100.0f + 20 * i);
		nameTextComponent->setPos(kGameWidth / 2 - 100, kGameHeight / 2 - 100 + 30 * i, kGameWidth, kGameHeight);
		highScoreMenuObject->addComponent(nameTextComponent);

		std::shared_ptr<TextComponent> scoreTextComponent = std::make_shared<TextComponent>();
		scoreTextComponent->init(mGameEngine->getFont());
		scoreTextComponent->setPos(kGameWidth/2 + 100, kGameHeight / 2 - 100 + 30 * i, kGameWidth, kGameHeight);
		highScoreMenuObject->addComponent(scoreTextComponent);

		std::shared_ptr<HighscoreLine> scoreLine = std::make_shared<HighscoreLine>();
		scoreLine->setTextComponent(nameTextComponent.get(), scoreTextComponent.get());
		scoreLine->setValue("Computer", 10 - i);
		highScoreMenuObject->addComponent(scoreLine);
		mHighScoreDialog->addHighScoreLine(scoreLine.get());
	}

	std::shared_ptr<GameObject> buttonObj = std::make_shared<GameObject>();
	std::shared_ptr<Geometry> buttonGeo = mGameEngine->getGeometry(GEOMETRY::POLY_1X1);
	std::shared_ptr<Texture> buttonTex = mGameEngine->getTexture("png\\button_close.png");
	std::shared_ptr<Renderable> buttonRender = std::make_shared<Renderable>();
	std::shared_ptr<BoundingBox> boundingBox = std::make_shared<BoundingBox>();
	std::shared_ptr<Button> button = std::make_shared<Button>();


	buttonObj->init(mGameEngine->getDevice());
	buttonRender->init(buttonGeo, buttonTex);
	boundingBox->init(buttonGeo->getTopLeft(), buttonGeo->getBottomRight(), nullptr);
	button->init(boundingBox.get());
	button->setFunc([this]() {this->showMainMenu(); });

	buttonObj->addComponent(buttonRender);
	buttonObj->addComponent(boundingBox);
	buttonObj->addComponent(button);

	buttonObj->setWorldScale(4.0f, kButtonHeight/1.5f, 1.0f);
	buttonObj->setLocalPos(0.0f, -5, -0.1f);
	highScoreMenuObject->addChild(buttonObj);
	mButtons.push_back(button.get());
	
	highScoreMenuObject->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	mGameEngine->addGameObject(kMenuLayer, highScoreMenuObject);
	mHighScoreDialog->closeDialog();
}

void Game::showMainMenu()
{
	mIsOnMenu = true;
	mPlayerHud->show(false);
	mMainMenu->setEnabled(true);
	mHighScoreDialog->closeDialog();
	mMainMenu->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset);
}

void Game::showHighScore()
{
	mIsOnMenu = true;
	mPlayerHud->show(false);
	mHighScoreDialog->showDialog();
	mMainMenu->setEnabled(false);
	mHighScoreDialog->getGameObject()->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset);
}