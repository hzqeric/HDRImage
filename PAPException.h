#pragma once

#include <iostream>
#include <string>

using namespace std;

class PAPException
{
public:
	PAPException(const string message);
	~PAPException();
	string getMessage();
protected:
	string _message;
};

