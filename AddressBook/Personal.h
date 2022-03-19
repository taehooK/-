//Personal.h
#ifndef _PERSONAL_H
#define _PERSONAL_H

#include <string>

using namespace std;
class Personal {
public:
	Personal(); // 디폴트 생성자
	Personal(string name, string address,
		string telephoneNumber, string emailAddress); // 생성자
	~Personal(); // 소멸자
	Personal(const Personal& source); // 복사 생성자

	bool IsEqual(const Personal& other);
	bool IsNotEqual(const Personal& other);
	bool operator==(const Personal& other);
	bool operator!=(const Personal& other);
	Personal& operator=(const Personal& other); // 치환 연산자
	
	string GetName() const;
	string GetAddress() const;
	string GetTelephoneNumber() const;
	string GetEmailAddress() const;
	

private:
	string name;
	string address;
	string telephoneNumber;
	string emailAddress;

};

inline string Personal::GetName() const {
	return const_cast<string&>(this->name);
}
inline string Personal::GetAddress() const {
	return const_cast<string&>(this->address);
}
inline string Personal::GetTelephoneNumber() const {
	return const_cast<string&>(this->telephoneNumber);
}
inline string Personal::GetEmailAddress() const {
	return const_cast<string&>(this->emailAddress);
}

#endif // !_PERSONAL_H
