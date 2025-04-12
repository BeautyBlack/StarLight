/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef UTILITIES_MESSENGER_3DBA2512BA5E4BB3A908AC73639AE9FA
#define UTILITIES_MESSENGER_3DBA2512BA5E4BB3A908AC73639AE9FA

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <utilities/Messenger/MessengerKey.h>

#include <utilities/Messenger/ExportFlagDefine.h>

#include <string>
#include <functional>
#include <map>

#define MESSAGE_GROUP_ID_TYPE unsigned long long

class UTILITIESMESSENGER_API CMessengerOwner
{
public:
	CMessengerOwner();
	CMessengerOwner(MESSAGE_GROUP_ID_TYPE groupId);

	MESSAGE_GROUP_ID_TYPE GetGroupId() const
	{
		return m_GroupId;
	}

private:
	//AUTO_PROPERTY_REF(GroupId, STRING_TYPE);
	AUTO_PROPERTY_REF(GroupId, MESSAGE_GROUP_ID_TYPE);
};

/*
 * 消息回调附注类
 */
class UTILITIESMESSENGER_API CMessenger
{
public:
	~CMessenger();
	
public:
	/*
	 * Description
	 * 注册消息
	 */
	void Regist(EMessengerKey key, CMessengerOwner *pOwner, std::function<void*(void*)> func);

	/*
	 * Description
	 * 发送消息
	 */
	void SendMessage(EMessengerKey key, CMessengerOwner *pSender, void* pParam);

	/*
	 * Description
	 * 发送消息
	 */
	void SendMessage(EMessengerKey key, void* pParam);
	
	/*
	 * Description
	 * 发送消息
	 */
	void SendMessage(EMessengerKey key, void* pParam, std::function<void(void*)> callback);

	/*
	 * Description
	 * 注销具体实例消息
	 */
	void Unregist(EMessengerKey key, CMessengerOwner *pOwner);

	/*
	 * Description
	 * 注销消息
	 */
	void Unregist(EMessengerKey key);
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CMessenger);

	// 由于很少使用同一个id绑定多个回调,暂时定为4以节省动态调整资源
	using FunctionCallbackMap = std::map<MESSAGE_GROUP_ID_TYPE, std::function<void*(void*)>>;
	using FuctionCallback = std::map<EMessengerKey, FunctionCallbackMap*>;
	
	AUTO_PROPERTY_REF_NO_SET(FunctionMap, FuctionCallback);
};

#endif
