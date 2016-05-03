#pragma once
#include"cocos2d.h"
USING_NS_CC;

class menuitemlabel_my : public MenuItemLabel
{
public:
	static menuitemlabel_my * createMy(Node*label, const ccMenuCallback& callback, const ccMenuCallback& callbackMy);
	virtual void selected() override;
	void setCallback_my(const ccMenuCallback& callback);
protected:
	ccMenuCallback my_callback;
};

