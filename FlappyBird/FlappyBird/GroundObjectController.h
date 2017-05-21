#pragma once
class GroundObjectController : public Component
{
public:
	GroundObjectController();
	~GroundObjectController();
	void update() override;
	void addGroundObject(GameObject* obj);
	void setPlayer(std::shared_ptr<PlayerBird> player);
	void setStartPos();
protected:
	std::vector<GameObject*> mGroundObjects;
	std::shared_ptr<PlayerBird> mPlayer = nullptr;
};

