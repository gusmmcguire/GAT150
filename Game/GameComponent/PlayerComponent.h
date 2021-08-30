#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
#include "Object/Actor.h"

class PlayerComponent : public gme::Component {
public:
	virtual ~PlayerComponent();

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const gme::Event& event);
	virtual void OnCollisionExit(const gme::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	float jumpSpeed{ 0 };
	
private:
	std::list<gme::Actor*> contacts;
};