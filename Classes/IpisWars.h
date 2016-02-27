#ifndef __IpisWars_H__
#define __IpisWars_H__

#include "cocos2d.h"

USING_NS_CC;
class IpisWars : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	Sprite* hand;
	Sprite* slipper;
	Sprite* edgeSp;
	PhysicsWorld* m_world;

	void setPhyWorld(PhysicsWorld* world) { m_world = world; };

	void IpisWars::check(float dt);

	bool onContactBegin(cocos2d::PhysicsContact &contact);
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(IpisWars);
	void update(float) override;
private:
	cocos2d::SEL_SCHEDULE selSched;
};

#endif // __IpisWars_SCENE_H__
