#pragma once
#include "Component.h"
class GameController : public Component
{
public:
	GameController();
	~GameController();

	bool init();
	void setPlayer(std::shared_ptr<Bird> player);
	void setPlayerBoundBox(std::shared_ptr<BoundingBox> playerBoundBox);
	void addBarrier(std::shared_ptr<GameObject> barrier);
	void setGroundController(std::shared_ptr<GroundObjectController> groundController);
	std::shared_ptr<Bird> getPlayer();
	void setPlayerHud(std::shared_ptr<PlayerHud> playerHud);
	void clean();
	void update()override;


protected:


	

protected:
	
};

