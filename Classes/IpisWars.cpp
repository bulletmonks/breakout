#include "IpisWars.h"
#include "GameOverScene.h"
#include "WinScene.h"
USING_NS_CC;

bool leftPressed; 
bool rightPressed;
int width;

Scene* IpisWars::createScene()
{
	auto scene = Scene::createWithPhysics();
	Vect gravity(0.0f, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);
	auto layer = IpisWars::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool IpisWars::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	width = visibleSize.width;
	edgeSp = Sprite::create();

	//Physics of the game
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	boundBody->getShape(0)->setRestitution(1.0f);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setDensity(1.0f);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp->setPhysicsBody(boundBody);
	boundBody->setContactTestBitmask(0x000001);
	this->addChild(edgeSp);
	edgeSp->setTag(0);

	//background creation
	auto backgroundSprite = Sprite::create("floor.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);

	//slipper creation
	slipper = Sprite::create("slipper.png", Rect(0, 0, 71, 71));
	slipper->setPosition(100, 100);
	auto slipperBody = PhysicsBody::createCircle(slipper->getContentSize().width / 2.); // The physics body circle shape
	slipperBody->getShape(0)->setRestitution(1.0f);
	slipperBody->getShape(0)->setFriction(0.0f);
	slipperBody->getShape(0)->setDensity(1.0f);
	slipperBody->setGravityEnable(false); // Not set acceleration
	Vect force = Vect(1010000.0f, 1010000.0f); // Create a force Vector to act with the direction of 45 degree, because x = y 
	slipperBody->applyImpulse(force); // Push a force into the slipper edge
	slipper->setPhysicsBody(slipperBody); // Set Physics body
	slipperBody->setContactTestBitmask(0x000001); //
	slipper->setTag(1);
	this->addChild(slipper);

	//hand creation
	hand = Sprite::create("hand.png");
	auto handBody = PhysicsBody::createBox(hand->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	handBody->getShape(0)->setRestitution(1.0f);
	handBody->getShape(0)->setFriction(0.0f);
	handBody->getShape(0)->setDensity(10.0f);
	handBody->setGravityEnable(false);
	handBody->setDynamic(false); // Set static when reacting, no restitution, no changing position
	hand->setPosition(visibleSize.width / 2, 100);
	hand->setPhysicsBody(handBody);
	handBody->setContactTestBitmask(0x000001); // With reaction 
	slipper->setTag(2);
	this->addChild(hand);

	//creation of first batch of ipis
	for (int i = 0; i < 7; i++) {
		auto ipis = Sprite::create("ipis.png");
		auto ipisBody = PhysicsBody::createBox(ipis->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		ipisBody->getShape(0)->setDensity(10.0f);
		ipisBody->getShape(0)->setFriction(0.0f);
		ipisBody->getShape(0)->setRestitution(1.f);
		ipisBody->setDynamic(false);
		ipis->setPosition(75 + (100 * i), 700);
		ipisBody->setContactTestBitmask(0x000001);
		ipis->setPhysicsBody(ipisBody);
		ipis->setTag(3);
		this->addChild(ipis);
	}
	//creation of second batch of ipis
	for (int i = 0; i < 7; i++) {
		auto ipis = Sprite::create("ipis.png");
		auto ipisBody = PhysicsBody::createBox(ipis->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		ipisBody->getShape(0)->setDensity(10.0f);
		ipisBody->getShape(0)->setFriction(0.0f);
		ipisBody->getShape(0)->setRestitution(1.f);
		ipisBody->setDynamic(false);
		ipis->setPosition(75+(100*i), 600);
		ipisBody->setContactTestBitmask(0x000001);
		ipis->setPhysicsBody(ipisBody);
		ipis->setTag(3);
		this->addChild(ipis);
	}
	//creation of third batch of ipis
	for (int i = 0; i < 7; i++) {
		auto ipis = Sprite::create("ipis.png");
		auto ipisBody = PhysicsBody::createBox(ipis->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		ipisBody->getShape(0)->setDensity(10.0f);
		ipisBody->getShape(0)->setFriction(0.0f);
		ipisBody->getShape(0)->setRestitution(1.f);
		ipisBody->setDynamic(false);
		ipis->setPosition(75 + (100 * i), 500);
		ipisBody->setContactTestBitmask(0x000001);
		ipis->setPhysicsBody(ipisBody);
		ipis->setTag(3);
		this->addChild(ipis);
	}
	
	//keyboard controls
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();

		switch (keyCode) {

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftPressed = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightPressed = true;
			break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftPressed = FALSE;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightPressed = FALSE;
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, hand);
	this->scheduleUpdate();

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(IpisWars::onContactBegin, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->schedule(schedule_selector(IpisWars::check), 0);
	return true;
}

void IpisWars::check(float dt)
{
	bool isWin = true;
	Vector<PhysicsBody*> bodies = m_world->getAllBodies();

	//check if there are still ipis
	for each(PhysicsBody* body in bodies)
	{
		if (body->getNode()->getTag() == 3) 
		{
			isWin = false; // Not Win yet
		}
	}

	// Win condition
	if (isWin == true)
	{
		auto scene = WinScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
	}

}

bool IpisWars::onContactBegin(PhysicsContact& contact)
{
	// Get two objects that come in contact
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	// Check what kind of object
	int objectA = spriteA->getTag();
	int objectB = spriteB->getTag();

	if (objectA == 3) // is an ipis
	{
		this->removeChild(spriteA, true); // delete the ipis
	}

	if (objectB == 3)  // is an ipis
	{
		this->removeChild(spriteB, true); // delete the ipis
	}

	// Game Over condition
	if ((objectA == 0 || objectB == 0)& (slipper->getPositionY() <= hand->getPositionY()))
	{
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
	}

	return true;
}

void IpisWars::update(float delta) {
	int handX, handY;
	handX = hand->getPositionX();
	handY = hand->getPositionY();
	//movement of hand
	if (leftPressed == TRUE && handX > 50 ) {
		hand->setPosition((handX - 10), handY);
	}
	if (rightPressed == TRUE && handX < width - 50) {
		hand->setPosition((handX + 10), handY);
	}
}
