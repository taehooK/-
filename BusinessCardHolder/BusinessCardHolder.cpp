// BusinessCardHolder.cpp

#include "BusinessCardHolder.h"

BusinessCardHolder::BusinessCardHolder()
{
	this->length = 0;
	this->current = 0;
}

BusinessCardHolder::BusinessCardHolder(const BusinessCardHolder& source)
	:businessCards(source.businessCards)
{
	this->length = source.length;
	this->current = &(this->businessCards.GetCurrent()->GetObject());
}

BusinessCard* BusinessCardHolder::PutIn(BusinessCard businessCard)
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.AppendFromTail(businessCard);
	this->length++;
	this->current = &(node->GetObject());

	return this->current;
}

void BusinessCardHolder::Find(string name, BusinessCard* *(*indexes), Long *count)
{
	LinkedList<BusinessCard>::Node* (*nodes);
	this->businessCards.LinearSearchDuplicate(&name, &nodes, count, ComparePersonalNames);
	
	*indexes = (BusinessCard**)calloc(this->length, sizeof(BusinessCard*));
	Long i = 0;
	while (i < *count)
	{
		(*indexes)[i] = &(nodes[i]->GetObject());
		i++;
	}

	if (nodes != 0)
	{
		delete[] nodes;
	}
}

BusinessCard* BusinessCardHolder::FindByCompanyName(string companyName)
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.LinearSearchUnique(&companyName, CompareCompanyNames);
	BusinessCard *index = 0;

	if (node != 0)
	{
		index = &(node->GetObject());
	}

	return index;
}

BusinessCard BusinessCardHolder::PutOut(BusinessCard *index)
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.LinearSearchUnique(index, CompareBusinessCardLinks);
	BusinessCard businessCard = node->GetObject();
	this->businessCards.Delete(node);
	this->length--;
	LinkedList<BusinessCard>::Node *current;
	current = this->businessCards.GetCurrent();

	if (current != 0)
	{
		this->current = &(current->GetObject());
	}
	else {
		this->current = 0;
	}

	return businessCard;
}

BusinessCard* BusinessCardHolder::First()
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.First();
	this->current = &(node->GetObject());

	return this->current;
}

BusinessCard* BusinessCardHolder::Previous()
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.Previous();
	this->current = &(node->GetObject());

	return this->current;
}

BusinessCard* BusinessCardHolder::Next()
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.Next();
	this->current = &(node->GetObject());

	return this->current;
}

BusinessCard* BusinessCardHolder::Last()
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.Last();
	this->current = &(node->GetObject());

	return this->current;
}

BusinessCard* BusinessCardHolder::Move(BusinessCard* index)
{
	LinkedList<BusinessCard>::Node *node;
	node = this->businessCards.LinearSearchUnique(index, CompareBusinessCardLinks);
	node = this->businessCards.Move(node);
	this->current = &(node->GetObject());

	return this->current;
}

BusinessCardHolder& BusinessCardHolder::operator=(const BusinessCardHolder& source)
{
	this->businessCards = source.businessCards;
	this->length = source.length;
	this->current = &(this->businessCards.GetCurrent()->GetObject());

	return *this;
}

BusinessCard& BusinessCardHolder::operator[](Long index)
{
	return this->businessCards[index];
}

#if 0
#include<iostream>
using namespace std;
int main(int arc, char *argv[])
{
	BusinessCard businessCard1("ȫ��", "����� ������", "02111", "021112", "hong.com",
		"ȫ�浿", " 01011111", "hong@", "����");
	BusinessCard businessCard2("����", "������ ������", "041111", "041112", "ko.com",
		"��浿", "0102222", "ko@", "����");
	BusinessCard businessCard3("����", "�뱸�� �޼���", "05223333", "0522333", "Lee.com",
		"ȫ�浿", "01022222", "hong2@", "���");
	BusinessCard businessCard4("ȫ��", "����� ������", "021112", "0211123", "hong.com",
		"���浿", "010333333", "jung@", "�븮");
	BusinessCardHolder businessCardHolder;
	BusinessCard *index;
	index = businessCardHolder.PutIn(businessCard1);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.FindByCompanyName("ȫ��");
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;
	index = businessCardHolder.PutIn(businessCard2);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.PutIn(businessCard3);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.PutIn(businessCard4);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.First();
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.Next();
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	BusinessCard businessCard = businessCardHolder.PutOut(index);
	cout << businessCard.GetCompanyName() << " " << businessCard.GetAddress() << " " << businessCard.GetCompanyTelephoneNumber() << " "
		<< businessCard.GetFaxNumber() << " " << businessCard.GetUrl() << " " << businessCard.GetPersonalName() << " " <<
		businessCard.GetPersonalTelephoneNumber() << " " << businessCard.GetEmailAddress() << " " << businessCard.GetPosition() << endl;
	index = businessCardHolder.PutIn(businessCard);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	BusinessCard *(*indexes);
	Long count;
	businessCardHolder.Find("ȫ�浿", &indexes, &count);
	Long i = 0;
	while (i < count) {
		cout << indexes[i]->GetCompanyName() << " " << indexes[i]->GetAddress() << " " << indexes[i]->GetCompanyTelephoneNumber() << " "
			<< indexes[i]->GetFaxNumber() << " " << indexes[i]->GetUrl() << " " << indexes[i]->GetPersonalName() << " " <<
			indexes[i]->GetPersonalTelephoneNumber() << " " << indexes[i]->GetEmailAddress() << " " << indexes[i]->GetPosition() << endl;
		i++;
	}

	index = businessCardHolder.Last();
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.Previous();
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.FindByCompanyName("ȫ��");
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	index = businessCardHolder.Move(indexes[0]);
	cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
		<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
		index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;

	BusinessCardHolder another(businessCardHolder);
	BusinessCard *previous = 0;
	index = another.First();
	while (index != previous) {
		cout << index->GetCompanyName() << " " << index->GetAddress() << " " << index->GetCompanyTelephoneNumber() << " "
			<< index->GetFaxNumber() << " " << index->GetUrl() << " " << index->GetPersonalName() << " " <<
			index->GetPersonalTelephoneNumber() << " " << index->GetEmailAddress() << " " << index->GetPosition() << endl;
		previous = index;
		index = another.Next();
	}

	businessCard = another.PutOut(index);
	businessCardHolder = another;
	cout << "����" << endl;
	i = 0;
	while (i < businessCardHolder.GetLength()) {
		cout << businessCardHolder[i].GetCompanyName() << " " << businessCardHolder[i].GetAddress() << " " << businessCardHolder[i].GetCompanyTelephoneNumber() << " "
			<< businessCardHolder[i].GetFaxNumber() << " " << businessCardHolder[i].GetUrl() << " " << businessCardHolder[i].GetPersonalName() << " " <<
			businessCardHolder[i].GetPersonalTelephoneNumber() << " " << businessCardHolder[i].GetEmailAddress() << " " << businessCardHolder[i].GetPosition() << endl;
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}
	

	return 0;

}
#endif

int ComparePersonalNames(void *one, void *other)
{
	BusinessCard *one_ = static_cast<BusinessCard*>(one);
	string *other_ = static_cast<string*>(other);

	return one_->GetPersonalName().compare(*other_);
}

int CompareCompanyNames(void *one, void *other)
{
	BusinessCard *one_ = static_cast<BusinessCard*>(one);
	string *other_ = static_cast<string*>(other);

	return one_->GetCompanyName().compare(*other_);
}

int CompareBusinessCardLinks(void *one, void *other)
{
	int ret;

	if (one < other)
	{
		ret = -1;
	}
	else if (one == other)
	{
		ret = 0;
	}
	else if (one > other)
	{
		ret = 1;
	}

	return ret;
}
