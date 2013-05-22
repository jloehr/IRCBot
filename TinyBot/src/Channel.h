#pragma once

#include <string>

class CChannel
{
public:
	CChannel(const std::string * Name, const std::string * Pass);
	~CChannel();

private:
	const std::string m_Name;
	const std::string m_Pass;

};