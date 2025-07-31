#include "stdafx.h"
#include "GameScript.h"
#include "GameScriptManager.h"
void GameScript::RegisterFields(TypeInfo& info) {

}

void GameScript::PostClone(CloneContext& ctx) {
    // 一般无需额外操作，留空
}
GameScript::GameScript() {
	//GameScriptManager::Instance().registerScript(this);
}
GameScript:: ~GameScript() {
	//GameScriptManager::Instance().unregisterScript(this);
}