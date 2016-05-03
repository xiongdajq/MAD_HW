#include "menuitemlabel_my.h"

menuitemlabel_my * menuitemlabel_my::createMy(Node*label, const ccMenuCallback& callback, const ccMenuCallback& callbackMy)
{
	menuitemlabel_my *ret = new (std::nothrow) menuitemlabel_my();
	ret->initWithLabel(label, callback);
	ret->setCallback_my(callbackMy);
	ret->autorelease();
	return ret;
}

void menuitemlabel_my::selected()
{
	MenuItemLabel::selected();
	if (_enabled)
	{
		if (my_callback)
		{
			my_callback(this);
		}
#if CC_ENABLE_SCRIPT_BINDING
		if (kScriptTypeLua == _scriptType)
		{
			BasicScriptData data(this);
			ScriptEvent scriptEvent(kMenuClickedEvent, &data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
		}
#endif
	}

}

void menuitemlabel_my::setCallback_my(const ccMenuCallback & callback)
{
	my_callback = callback;
}
