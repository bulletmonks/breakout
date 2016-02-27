#include "WinScene.h"

USING_NS_CC;

Scene* WinScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WinScene::create();
	scene->addChild(layer);
	return scene;
}

bool WinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//floor sprite
	auto floorSprite = Sprite::create("floor.png");
	floorSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(floorSprite);

	//win image
	auto winSprite = Sprite::create("win.png");
	winSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(winSprite);

	return true;
}	