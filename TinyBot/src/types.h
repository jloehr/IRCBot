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

typedef std::vector<std::string> StringVector;
typedef std::set<std::string> StringSet;
typedef std::pair<std::string *, std::string *> StringPair;
typedef std::vector<StringPair *> StringPairVector;
typedef std::pair<StringPair *, StringPairVector *> StringPairStringVectorPair;
typedef std::vector<StringPairStringVectorPair *> StringPairStringVectorPairVector;

class CServer;
class CChannel;

typedef std::vector<CServer * > ServerVector;
typedef std::map<std::string, CChannel * > ChannelMap;