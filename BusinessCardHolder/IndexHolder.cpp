#include "IndexHolder.h"
#include "BusinessCard.h"
#include "BusinessCardHolder.h"

IndexHolder::IndexHolder() {
	this->length = 0;
}
IndexHolder::IndexHolder(const IndexHolder& source)
	:indexes(source.indexes) {
	this->length = source.length;
}
IndexHolder::~IndexHolder() {

}
Index* IndexHolder::PutIn(BusinessCard *businessCardLink) {
	string companyName = businessCardLink->GetCompanyName();
	Index index(companyName);
	BinaryTree<Index>::Node *node = this->indexes.Search(index, CompareIndexes);
	if (node == 0) {
		node = this->indexes.Insert(index, CompareIndexes);
		this->length++;
	}
	Index *indexLink = &node->GetKey();
	indexLink->PutIn(businessCardLink);

	return indexLink;
}
Index* IndexHolder::Find(string companyName) {
	Index index(companyName);
	BinaryTree<Index>::Node *node = this->indexes.Search(index, CompareIndexes);
	Index *indexLink = 0;
	if (node != 0) {
		indexLink = &node->GetKey();
	}

	return indexLink;
}
Index* IndexHolder::PutOut(string companyName, BusinessCard *businessCardLink) {
	Index index(companyName);
	BinaryTree<Index>::Node *node = this->indexes.Search(index, CompareIndexes);
	Index *indexLink = &node->GetKey();
	Long location = indexLink->Find(businessCardLink);
	indexLink->PutOut(location);
	if (indexLink->GetLength() == 0) {
		this->indexes.Delete(index, CompareIndexes);
		this->length--;
		indexLink = 0;
	}

	return indexLink;
}
void IndexHolder::MakeList(Index* (*indexes), Long *count) {
	this->indexes.MakeKeys(indexes, count);
}
void IndexHolder::Arrange() {
	this->indexes.MakeBalance();
	BinaryTree<Index>::Node* node = this->indexes.GetRoot();
	Index *key = &node->GetKey();
	key->Arrange(node);

}
IndexHolder& IndexHolder::operator =(const IndexHolder& source) {
	this->indexes = source.indexes;
	this->length = source.length;

	return *this;
}
int CompareIndexes(void *one, void *other) {
	Index *one_ = static_cast<Index*>(one);
	Index *other_ = static_cast<Index*>(other);

	return one_->GetName().compare(other_->GetName());
}

#if 0
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	BusinessCardHolder businessCardHolder;

	BusinessCard businessCard1("ȫ��", "�����", "021111", "021110", "hong.com", "ȫ�浿", "0101111", "hong@", "����");
	BusinessCard businessCard2("����", "������", "0432222", "0432220", "go.com", "��浿", "0102222", "go@", "�븮");
	BusinessCard businessCard3("ȫ��", "�����", "021111", "021110", "hong.com", "���浿", "0103333", "jung@", "����");
	BusinessCard businessCard4("����", "�λ��", "0513333", "0513330", "jung.com", "ȫ�浿", "0104444", "gil@", "���");

	BusinessCard *businessCardLink;
	BusinessCard* (*businessCardLinks);
	Long count;
	Long i;
	Long location;
	Long j;
	Index *indexLink;
	Index(*indexes);
	Long indexCount;

	IndexHolder indexHolder;

	businessCardLink = businessCardHolder.PutIn(businessCard1);
	indexLink = indexHolder.PutIn(businessCardLink);
	cout << indexLink->GetName() << endl;
	i = 0;
	while (i < indexLink->GetLength()) {
		cout << (*indexLink)[i]->GetCompanyName() << " " << (*indexLink)[i]->GetAddress() << " " << (*indexLink)[i]->GetCompanyTelephoneNumber()
			<< " " << (*indexLink)[i]->GetFaxNumber() << " " << (*indexLink)[i]->GetUrl() << " " << (*indexLink)[i]->GetPersonalName()
			<< " " << (*indexLink)[i]->GetPersonalTelephoneNumber() << " " << (*indexLink)[i]->GetEmailAddress() << " "
			<< (*indexLink)[i]->GetPosition() << endl;
		i++;
	}

	businessCardLink = businessCardHolder.PutIn(businessCard2);
	indexLink = indexHolder.PutIn(businessCardLink);
	cout << indexLink->GetName() << endl;
	i = 0;
	while (i < indexLink->GetLength()) {
		businessCardLink = indexLink->GetAt(i);
		cout << businessCardLink->GetCompanyName() << " " << businessCardLink->GetAddress() << " " << businessCardLink->GetCompanyTelephoneNumber()
			<< " " << businessCardLink->GetFaxNumber() << " " << businessCardLink->GetUrl() << " " << businessCardLink->GetPersonalName()
			<< " " << businessCardLink->GetPersonalTelephoneNumber() << " " << businessCardLink->GetEmailAddress() << " "
			<< businessCardLink->GetPosition() << endl;
		i++;
	}

	businessCardLink = businessCardHolder.PutIn(businessCard3);
	indexLink = indexHolder.PutIn(businessCardLink);
	cout << indexLink->GetName() << endl;
	i = 0;
	while (i < indexLink->GetLength()) {
		cout << (*indexLink)[i]->GetCompanyName() << " " << (*indexLink)[i]->GetAddress() << " " << (*indexLink)[i]->GetCompanyTelephoneNumber()
			<< " " << (*indexLink)[i]->GetFaxNumber() << " " << (*indexLink)[i]->GetUrl() << " " << (*indexLink)[i]->GetPersonalName()
			<< " " << (*indexLink)[i]->GetPersonalTelephoneNumber() << " " << (*indexLink)[i]->GetEmailAddress() << " "
			<< (*indexLink)[i]->GetPosition() << endl;
		i++;
	}

	businessCardLink = businessCardHolder.PutIn(businessCard4);
	indexLink = indexHolder.PutIn(businessCardLink);
	cout << indexLink->GetName() << endl;
	i = 0;
	while (i < indexLink->GetLength()) {
		cout << (*indexLink)[i]->GetCompanyName() << " " << (*indexLink)[i]->GetAddress() << " " << (*indexLink)[i]->GetCompanyTelephoneNumber()
			<< " " << (*indexLink)[i]->GetFaxNumber() << " " << (*indexLink)[i]->GetUrl() << " " << (*indexLink)[i]->GetPersonalName()
			<< " " << (*indexLink)[i]->GetPersonalTelephoneNumber() << " " << (*indexLink)[i]->GetEmailAddress() << " "
			<< (*indexLink)[i]->GetPosition() << endl;
		i++;
	}

	Index other(*indexLink);
	cout << other.GetName() << endl;
	i = 0;
	while (i < other.GetLength()) {
		cout << other[i]->GetCompanyName() << " " << other[i]->GetAddress() << " " << other[i]->GetCompanyTelephoneNumber()
			<< " " << other[i]->GetFaxNumber() << " " << other[i]->GetUrl() << " " << other[i]->GetPersonalName()
			<< " " << other[i]->GetPersonalTelephoneNumber() << " " << other[i]->GetEmailAddress() << " "
			<< other[i]->GetPosition() << endl;
		i++;
	}

	businessCardHolder.Find("ȫ�浿", &businessCardLinks, &count);
	businessCardLink = businessCardLinks[0];
	BusinessCard businessCard = businessCardHolder.PutOut(businessCardLink);
	string companyName = businessCard.GetCompanyName();
	indexLink = indexHolder.PutOut(companyName, businessCardLink);
	if (indexLink != 0) {
		cout << indexLink->GetName() << endl;
	}


	indexLink = indexHolder.Find("ȫ��");
	if (indexLink != 0) {
		cout << indexLink->GetName() << endl;
	}

	cout << endl;
	IndexHolder another(indexHolder);
	another.MakeList(&indexes, &indexCount);
	i = 0;
	while (i < indexCount) {
		j = 0;
		while (j < indexes[i].GetLength()) {
			businessCardLink = indexes[i].GetAt(j);
			cout << businessCardLink->GetCompanyName() << " " << businessCardLink->GetAddress() << " " << businessCardLink->GetCompanyTelephoneNumber()
				<< " " << businessCardLink->GetFaxNumber() << " " << businessCardLink->GetUrl() << " " << businessCardLink->GetPersonalName()
				<< " " << businessCardLink->GetPersonalTelephoneNumber() << " " << businessCardLink->GetEmailAddress() << " "
				<< businessCardLink->GetPosition() << endl;
			j++;
		}
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}


	indexHolder.Arrange();
	
	cout << endl;
	indexHolder.MakeList(&indexes, &indexCount);
	i = 0;
	while (i < indexCount) {
		j = 0;
		while (j < indexes[i].GetLength()) {
			businessCardLink = indexes[i].GetAt(j);
			cout << businessCardLink->GetCompanyName() << " " << businessCardLink->GetAddress() << " " << businessCardLink->GetCompanyTelephoneNumber()
				<< " " << businessCardLink->GetFaxNumber() << " " << businessCardLink->GetUrl() << " " << businessCardLink->GetPersonalName()
				<< " " << businessCardLink->GetPersonalTelephoneNumber() << " " << businessCardLink->GetEmailAddress() << " "
				<< businessCardLink->GetPosition() << endl;
			j++;
		}
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}

	businessCard = businessCardHolder.PutOut(businessCardLinks[1]);
	companyName = businessCard.GetCompanyName();
	indexLink = indexHolder.PutOut(companyName, businessCardLinks[1]);
	if (businessCardLinks != 0) {
		delete[] businessCardLinks;
	}
	another = indexHolder;
	another.MakeList(&indexes, &indexCount);
	i = 0;
	while (i < indexCount) {
		j = 0;
		while (j < indexes[i].GetLength()) {
			businessCardLink = indexes[i].GetAt(j);
			cout << businessCardLink->GetCompanyName() << " " << businessCardLink->GetAddress() << " " << businessCardLink->GetCompanyTelephoneNumber()
				<< " " << businessCardLink->GetFaxNumber() << " " << businessCardLink->GetUrl() << " " << businessCardLink->GetPersonalName()
				<< " " << businessCardLink->GetPersonalTelephoneNumber() << " " << businessCardLink->GetEmailAddress() << " "
				<< businessCardLink->GetPosition() << endl;
			j++;
		}
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}


	return 0;
}
#endif