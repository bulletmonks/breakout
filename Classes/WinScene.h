#ifndef __WIN_SCENE_H__
#define __WIN_SCENE_H__

#include "cocos2d.h"

class WinScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(WinScene);
};

#endif // __WIN_SCENE_H__
