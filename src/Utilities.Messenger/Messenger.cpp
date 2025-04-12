#include "pch.h"

#include <utilities/Messenger/Messenger.h>

#include <utility>

#include "Foundations/Common/StringUtility.h"

CMessenger CMessenger::m_Instance;

static MESSAGE_GROUP_ID_TYPE GlobalMessengerOwnerGroupIdIndex = 1;

CMessengerOwner::CMessengerOwner()
{
	auto index = InterlockedIncrement(&GlobalMessengerOwnerGroupIdIndex);
	
	SetGroupId(index);
}

CMessengerOwner::CMessengerOwner(MESSAGE_GROUP_ID_TYPE groupId)
	: m_GroupId(groupId)
{
}

CMessenger::~CMessenger()
{
	for (auto& i : GetFunctionMap())
	{
		delete i.second;
	}

	GetFunctionMap().clear();
}

void CMessenger::Regist(EMessengerKey key, CMessengerOwner* pOwner, std::function<void*(void*)> func)
{
	if(pOwner == NULL_SYMBAL)
		return;
	
	auto keyFuncs = GetFunctionMap()[key];

	if(keyFuncs == NULL_SYMBAL)
	{
		keyFuncs = new FunctionCallbackMap();

		GetFunctionMap()[key] = keyFuncs;
	}
	
	(*keyFuncs)[pOwner->GetGroupId()] = std::move(func);
}

void CMessenger::SendMessage(EMessengerKey key, CMessengerOwner *pSender, void* pParam)
{
	if (pSender == NULL_SYMBAL)
		return;

	auto keyFuncs = GetFunctionMap()[key];

	if (keyFuncs == NULL_SYMBAL)
		return;
	
	auto &func = (*keyFuncs)[pSender->GetGroupId()];

	if (func)
		func(pParam);
}

void CMessenger::SendMessage(EMessengerKey key, void* pParam)
{
	auto keyFuncs = GetFunctionMap()[key];

	if (keyFuncs == NULL_SYMBAL)
		return;
	
	for (auto& i : (*keyFuncs))
	{
		i.second(pParam);
	}
}

void CMessenger::SendMessage(EMessengerKey key, void* pParam, std::function<void(void*)> callback)
{
	auto keyFuncs = GetFunctionMap()[key];

	if (keyFuncs == NULL_SYMBAL)
		return;

	for (auto& i : (*keyFuncs))
	{
		const auto result = i.second(pParam);

		if (callback != NULL_SYMBAL)
			callback(result);
	}
}

void CMessenger::Unregist(EMessengerKey key, CMessengerOwner* pOwner)
{
	if (pOwner == NULL_SYMBAL)
		return;
	
	auto keyFuncs = GetFunctionMap()[key];

	if (keyFuncs == NULL_SYMBAL)
		return;
	
	(*keyFuncs).erase(pOwner->GetGroupId());
}


void CMessenger::Unregist(EMessengerKey key)
{
	auto keyFuncs = GetFunctionMap()[key];

	GetFunctionMap().erase(key);

	if (keyFuncs != NULL_SYMBAL)
		delete keyFuncs;
}
