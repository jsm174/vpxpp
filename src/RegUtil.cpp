#include "RegUtil.h"

#include <fstream>

RegUtil::RegUtil()
{
	Open();
}

RegUtil::~RegUtil()
{
}

RegUtil* RegUtil::SharedInstance()
{
	static RegUtil regUtil;
	return &regUtil;
}

void RegUtil::Open()
{
	try
	{
		std::ifstream fstream("vpxpp.json");
		fstream >> m_json;
	}

	catch (nlohmann::json::exception e)
	{
		m_json = nlohmann::json({});
	}
}

void RegUtil::Save()
{
	try
	{
		std::ofstream fstream("vpxpp.json");
		fstream << std::setw(4) << m_json << std::endl;
	}

	catch (nlohmann::json::exception e)
	{
	}
}

HRESULT RegUtil::LoadValue(const std::string& szKey, const std::string& szValue, std::string& buffer)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] != nullptr)
	{
		buffer = (*pNode)[szValue];
		return S_OK;
	}

	return S_FALSE;
}

HRESULT RegUtil::LoadValue(const std::string& szKey, const std::string& szValue, void* const szbuffer, const DWORD size)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] != nullptr)
	{
		// TODO: szbuffer = (*pNode)[szValue];
		return S_OK;
	}

	return S_FALSE;
}

HRESULT RegUtil::LoadValue(const std::string& szKey, const std::string& szValue, float& pfloat)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] != nullptr)
	{
		pfloat = (*pNode)[szValue];
		return S_OK;
	}

	return S_FALSE;
}

HRESULT RegUtil::LoadValue(const std::string& szKey, const std::string& szValue, int& pint)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] != nullptr)
	{
		pint = (*pNode)[szValue];
		return S_OK;
	}

	return S_FALSE;
}

HRESULT RegUtil::LoadValue(const std::string& szKey, const std::string& szValue, unsigned int& pint)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] != nullptr)
	{
		pint = (*pNode)[szValue];
		return S_OK;
	}

	return S_FALSE;
}

int RegUtil::LoadValueIntWithDefault(const std::string& szKey, const std::string& szValue, const int def)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] == nullptr)
	{
		return def;
	}

	return (*pNode)[szValue];
}

float RegUtil::LoadValueFloatWithDefault(const char* szKey, const char* szValue, const float def)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] == nullptr)
	{
		return def;
	}

	return (*pNode)[szValue];
}

bool RegUtil::LoadValueBoolWithDefault(const std::string& szKey, const std::string& szValue, const bool def)
{
	nlohmann::json* pNode = KeyToNode(szKey);

	if ((*pNode)[szValue] == nullptr)
	{
		return def;
	}

	return (*pNode)[szValue];
}

HRESULT RegUtil::SaveValue(const std::string& szKey, const std::string& szValue, const char* val)
{
	nlohmann::json* pNode = KeyToNode(szKey);
	(*pNode)[szValue] = val;

	Save();

	return S_OK;
}

HRESULT RegUtil::SaveValue(const std::string& szKey, const std::string& szValue, const std::string& val)
{

	nlohmann::json* pNode = KeyToNode(szKey);
	(*pNode)[szValue] = val;

	Save();

	return S_OK;
}

HRESULT RegUtil::SaveValueFloat(const std::string& szKey, const std::string& szValue, const float val)
{
	nlohmann::json* pNode = KeyToNode(szKey);
	(*pNode)[szValue] = val;

	Save();

	return S_OK;
}

HRESULT RegUtil::SaveValueInt(const std::string& szKey, const std::string& szValue, const int val)
{
	nlohmann::json* pNode = KeyToNode(szKey);
	(*pNode)[szValue] = val;

	Save();

	return S_OK;
}

HRESULT RegUtil::SaveValueBool(const std::string& szKey, const std::string& szValue, const bool val)
{
	nlohmann::json* pNode = KeyToNode(szKey);
	(*pNode)[szValue] = val;

	Save();

	return S_OK;
}

HRESULT RegUtil::DeleteValue(const std::string& szKey, const std::string& szValue)
{
	return S_OK;
}

std::vector<std::string> RegUtil::SplitKey(const std::string& szKey)
{
	std::vector<std::string> paths;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = szKey.find('\\', pos)) != std::string::npos)
	{
		std::string substring(szKey.substr(prev_pos, pos - prev_pos));

		paths.push_back(substring);

		prev_pos = ++pos;
	}

	paths.push_back(szKey.substr(prev_pos, pos - prev_pos));

	return paths;
}

nlohmann::json* RegUtil::KeyToNode(const std::string& szKey)
{
	nlohmann::json* pNode = &m_json;

	for (std::string name : SplitKey(szKey))
	{
		if ((*pNode)[name] == nullptr)
		{
			(*pNode)[name] = nlohmann::json({});
		}

		pNode = &((*pNode)[name]);
	}

	return pNode;
}