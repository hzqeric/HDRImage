#include "stdafx.h"
#include "PAPException.h"


PAPException::PAPException(const string message)
{	
	cerr << "PAPException: " << message << endl;
	_message = message;
}


PAPException::~PAPException()
{
}


string PAPException::getMessage() {
	return _message;
}