// Personal.cpp
#include "Personal.h"

Personal::Personal()
	:name(""), address(""), telephoneNumber(""), emailAddress(""){

}
Personal::Personal(string name, string address,
	string telephoneNumber, string emailAddress) {
	this->name = name;
	this->address = address;
	this->telephoneNumber = telephoneNumber;
	this->emailAddress = emailAddress;
}

Personal::~Personal() {
} // ¼Ò¸êÀÚ
Personal::Personal(const Personal& source) {
	this->name = source.name;
	this->address = source.address;
	this->telephoneNumber = source.telephoneNumber;
	this->emailAddress = source.emailAddress;
}

bool Personal::IsEqual(const Personal& other) {
	bool ret = false;
	if (this->name.compare(other.name) == 0 &&
		this->address.compare(other.address) == 0 &&
		this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->emailAddress.compare(other.emailAddress) == 0) {
		ret = true;
	}
	return ret;
}
bool Personal::IsNotEqual(const Personal& other) {
	bool ret = false;
	if (this->name.compare(other.name) != 0 ||
		this->address.compare(other.address) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->emailAddress.compare(other.emailAddress) != 0) {
		ret = true;
	}
	return ret;
}
bool Personal::operator==(const Personal& other) {
	bool ret = false;
	if (this->name.compare(other.name) == 0 &&
		this->address.compare(other.address) == 0 &&
		this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->emailAddress.compare(other.emailAddress) == 0) {
		ret = true;
	}
	return ret;
}
bool Personal::operator!=(const Personal& other) {
	bool ret = false;
	if (this->name.compare(other.name) != 0 ||
		this->address.compare(other.address) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->emailAddress.compare(other.emailAddress) != 0) {
		ret = true;
	}
	return ret;
}
Personal& Personal::operator=(const Personal& other) {
	this->name = other.name;
	this->address = other.address;
	this->telephoneNumber = other.telephoneNumber;
	this->emailAddress = other.emailAddress;
	
	return *this;
} 

