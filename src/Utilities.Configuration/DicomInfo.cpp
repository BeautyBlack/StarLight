#include "pch.h"
#include "utilities/Configuration/DicomInfo.h"

CDicomInfo::CDicomInfo()
{
	SetServerPort(9512);
}


void CDicomInfo::LoadFrom(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	const Json::Value &referenceJsonRoot = (*pJsonRoot);

	const auto dicomRoot = referenceJsonRoot[GetDicomNodeJsonKey()];

	if (dicomRoot.empty())
		return;

	auto port = dicomRoot[GetServerPortJsonKey()];

	if (!port.empty())
		SetServerPort(port.asInt());

	auto aet = dicomRoot[GetLocalAETJsonKey()];

	if (!aet.empty())
		SetLocalAET(aet.asCString());

	auto peerAET = dicomRoot[GetPeerAETJsonKey()];
	if ( !peerAET.empty())
		SetPeerAET(peerAET.asCString());

	auto peerIp = dicomRoot[GetPeerAddressJsonKey()];
	if (!peerIp.empty())
		SetPeerAddress(peerIp.asCString());

	auto peerPort = dicomRoot[GetPeerPortJsonKey()];
	if (!peerPort.empty())
		SetPeerPort(peerPort.asInt());
}

void CDicomInfo::SaveTo(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	Json::Value &referenceJsonRoot = (*pJsonRoot);

	auto &dicomRoot = referenceJsonRoot[GetDicomNodeJsonKey()];

	dicomRoot[GetServerPortJsonKey()] = GetServerPort();
	dicomRoot[GetLocalAETJsonKey()] = GetLocalAET();

	dicomRoot[GetPeerAETJsonKey()] = GetPeerAET();
	dicomRoot[GetPeerAddressJsonKey()] = GetPeerAddress();
	dicomRoot[GetPeerPortJsonKey()] = GetPeerPort();
}
