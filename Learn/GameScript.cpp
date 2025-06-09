#include "stdafx.h"
#include "GameScript.h"
#include "GameScriptManager.h"
ComponentType GameScript::GetType()
{
	return ComponentType::GameScript;
}
GameScript::GameScript() {
	//GameScriptManager::Instance().registerScript(this);
}
GameScript:: ~GameScript() {
	GameScriptManager::Instance().unregisterScript(this);
}