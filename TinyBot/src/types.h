#pragma once

#include <utility> 
#include <string> 
#include <vector>
#include <map>

typedef std::pair<std::string *, std::string *> StringPair;
typedef std::vector<StringPair *> StringPairVector;
typedef std::pair<StringPair *, StringPairVector *> StringPairStringVectorPair;
typedef std::vector<StringPairStringVectorPair *> StringPairStringVectorPairVector;

class CServer;
class CChannel;

typedef std::vector<CServer * > ServerVector;
typedef std::map<std::string, CChannel * > ChannelMap;