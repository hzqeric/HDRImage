#include "stdafx.h"
#include <algorithm>
#include <functional>
#include "PAPStringUtil.h"


bool strCompareLC(const string str1, const string str2) {
	string tmp1 = string(str1);
	string tmp2 = string(str2);
	transform(str1.begin(), str1.end(), tmp1.begin(), tolower);
	transform(str2.begin(), str2.end(), tmp2.begin(), tolower);
	return ((tmp1.compare(tmp2)) == 0);
}

void trimFromLeft(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(),
		not1(ptr_fun<int, int>(isspace))));
}

void trimFromRight(std::string &s) {
	s.erase(find_if(s.rbegin(), s.rend(),
		not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

void trim(std::string &s) {
	trimFromLeft(s);
	trimFromRight(s);
}

string trimmedFromLeft(string s) {
	trimFromLeft(s);
	return s;
}

string trimmedFromRight(string s) {
	trimFromRight(s);
	return s;
}

string trimmed(string s){
	trim(s);
	return s;
}

string strToLower(string *str) {
	string result = string(*str);
	transform(str->begin(), str->end(), result.begin(), tolower);
	return result;
}