/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
 *
*/
#pragma once

#include <utility> 
#include <string> 
#include <set>
#include <vector>
#include <map>

typedef struct ServerData {
public:
	std::string * Adress;
	std::string * Port;
	std::string * Pass;

	ServerData(std::string * Adress, std::string * Port, std::string * Pass)
	:Adress(Adress), Port(Port), Pass(Pass)
	{}

	void Flush()
	{
		if(Adress != NULL)
		{
			delete Adress;
		}

		if(Port != NULL)
		{
			delete Port;
		}

		if(Pass != NULL)
		{
			delete Pass;
		}
	}

} * ServerDataPointer;

typedef std::string * StringPointer;
typedef std::vector<std::string> StringVector;
typedef std::set<std::string> StringSet;
typedef std::pair<std::string *, std::string *> StringPair;
typedef std::vector<StringPair *> StringPairVector;
typedef std::pair<StringPair *, StringPairVector *> StringPairStringVectorPair;
typedef std::pair<ServerDataPointer, StringPairVector *> ServerDataStringVectorPair;
typedef std::vector<StringPairStringVectorPair *> StringPairStringVectorPairVector;
typedef std::vector<ServerDataStringVectorPair *> ServerDataStringVectorPairVector;

class CServer;
class CChannel;

typedef std::vector<CServer * > ServerVector;
typedef std::map<std::string, CChannel * > ChannelMap;