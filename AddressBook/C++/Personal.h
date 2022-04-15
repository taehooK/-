//Personal.h
#ifndef _PERSONAL_H
#define _PERSONAL_H

#include <string>

using namespace std;
class Personal {
public:
	Personal(); // ����Ʈ ������
	Personal(string name, string address,
		string telephoneNumber, string emailAddress); // ������
	~Personal(); // �Ҹ���
	Personal(const Personal& source); // ���� ������

	bool IsEqual(const Personal& other);
	bool IsNotEqual(const Personal& other);
	bool operator==(const Personal& other);
	bool operator!=(const Personal& other);
	Personal& operator=(const Personal& other); // ġȯ ������
	
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
