#include "AddresssBook.h"

AddressBook::AddressBook(Long capacity)
	: personals(capacity) {
	this->capacity = capacity;
	this->length = 0;
}
AddressBook::AddressBook(const AddressBook& source)
	: personals(source.personals) {
	this->capacity = source.capacity;
	this->length = source.length;
}
AddressBook::~AddressBook() {
}
Long AddressBook::Record(string name, string address, string telephoneNumber, string emailAddress) {
	Long index;
	Personal personal(name, address, telephoneNumber, emailAddress);

	if (this->length < this->capacity) {
		index = this->personals.Store(this->length, personal);
	}
	else if (this->length >= this->capacity) {
		index = this->personals.AppendFromRear(personal);
		this->capacity++;
	}
	this->length++;

	return index;
}
void AddressBook::Find(string name, Long* (*indexes), Long *count) {
	this->personals.LinearSearchDuplicate(&name, indexes, count, CompareNames);
}
Long AddressBook::Correct(Long index, string address, string telephoneNumber, string emailAddress) {
	Personal personal(this->personals.GetAt(index).GetName(), address, telephoneNumber, emailAddress);
	this->personals.Modify(index, personal);

	return index;
}
Long AddressBook::Erase(Long index) {
	index = this->personals.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}
void AddressBook::Arrange() {
	this->personals.SelectionSort(ComparePersonals);
}
Personal& AddressBook::GetAt(Long index) {
	return this->personals.GetAt(index);
}
AddressBook& AddressBook::operator =(const AddressBook& source) {
	this->personals = source.personals;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}
Personal& AddressBook::operator[](Long index) {
	return this->personals[index];
}
int CompareNames(void *one, void *other) {
	Personal *one_ = static_cast<Personal*>(one);
	string *other_ = static_cast<string*>(other);

	return one_->GetName().compare(*other_);
}
int ComparePersonals(void *one, void *other) {
	Personal *one_ = static_cast<Personal*>(one);
	Personal *other_ = static_cast<Personal*>(other);

	return one_->GetName().compare(other_->GetName());
}

#if 0
#include <iostream>

using namespace std;
int main(int argc, char* argv[]) {
	//1. �ּҷ��� �����ϴ�.
	AddressBook addressBook(2);
	//2. ȫ�浿, �����, 0101111, hong@�� �����ϴ�.
	Long index = addressBook.Record("ȫ�浿", "�����", "0101111", "hong@");
	Personal personal;
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
	//3. ��浿, ������, 0102222, ko@�� �����ϴ�.
	index = addressBook.Record("��浿", "������", "0102222", "ko@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
	//�����ϴ�.
	AddressBook newAddressBook(addressBook);
	Long i = 0;
	while (i < newAddressBook.GetLength()) {
		personal = newAddressBook.GetAt(i);
		cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
		i++;
	}
	//4. ���浿, �뱸��, 0103333, jung@�� �����ϴ�.
	index = addressBook.Record("���浿", "�뱸��", "0103333", "jung@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
	//5. ȫ�浿, �λ��, 0104444, hong@�� �����ϴ�.
	index = addressBook.Record("ȫ�浿", "�λ��", "0104444", "hong2@");
	personal = addressBook.GetAt(index);
	cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
	//6. ȫ�浿�� ã��.
	Long(*indexes);
	Long count;
	addressBook.Find("ȫ�浿", &indexes, &count);
	i = 0;
	while (i < count) {
		personal = addressBook.GetAt(indexes[i]);
		cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}
	//7. ����°�� ����÷� �ٲٴ�.
	index = addressBook.Correct(2, "�����", "0103333", "jung@");
	personal = addressBook[index];
	cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
	//8. �ι�°�� �����.
	index = addressBook.Erase(1);
	if (index == -1) {
		cout << "���������ϴ�." << endl;
	}
	//9. �����ϴ�.
	addressBook.Arrange();
	i = 0;
	while (i < addressBook.GetLength()) {
		personal = addressBook.GetAt(i);
		cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
		i++;
	}
	//ġȯ�ϴ�.
	newAddressBook = addressBook;
	i = 0;
	while (i < newAddressBook.GetLength()) {
		personal = newAddressBook[i];
		cout << personal.GetName() << " " << personal.GetAddress() << " " << personal.GetTelephoneNumber() << " " << personal.GetEmailAddress() << endl;
		i++;
	}

	return 0;
}
#endif