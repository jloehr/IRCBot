#pragma once
#include <utility> 
#include <string> 
#include <vector>

typedef std::pair<std::string, std::string> StringPair;
typedef std::vector<StringPair> StringPairVector;
typedef std::pair<StringPair, StringPairVector> StringPairStringVectorPair;
typedef std::vector<StringPairStringVectorPair> StringPairStringVectorPairVector;