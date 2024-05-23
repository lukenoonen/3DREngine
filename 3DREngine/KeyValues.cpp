#include "KeyValues.h"

CKeyValues::CKeyValues()
{

}

CKeyValues::~CKeyValues()
{
	std::unordered_map<const char *, CBaseAny *>::iterator itIterator;
	for (itIterator = m_mKeyValues.begin(); itIterator != m_mKeyValues.end(); itIterator++)
		delete itIterator->second;
}