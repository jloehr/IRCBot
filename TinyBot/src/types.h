/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TinyBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TinyBot.  If not, see <http://www.gnu.org/licenses/>.
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