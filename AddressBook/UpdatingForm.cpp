//UpdatingForm.cpp
#include "UpdatingForm.h"
#include "FindingForm.h"
#include <afxcmn.h>
#include "AddresssBook.h"
#include <afxdb.h>

BEGIN_MESSAGE_MAP(UpdatingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnRecordButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT, OnCorrectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnEraseButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ARRANGE, OnArrangeButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERSONALS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

UpdatingForm::UpdatingForm(CWnd *parent)
	:CDialog(UpdatingForm::IDD, parent) {
	this->addressBook = NULL;
}

BOOL UpdatingForm::OnInitDialog() {
	CDialog::OnInitDialog();
	this->addressBook = new AddressBook(10000);
	CString number;
	Long i=0;
	Personal personal;
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;

	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(
		0, "번호", LVCFMT_LEFT, 50);
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(
		1, "성명", LVCFMT_LEFT, 100);
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(
		2, "주소", LVCFMT_LEFT, 250);
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(
		3, "전화번호", LVCFMT_LEFT, 150);
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertColumn(
		4, "이메일주소", LVCFMT_LEFT, 200);

	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	Load();

	if (this->addressBook->GetLength() > 0) {
		while (i < this->addressBook->GetLength()) {
			number.Format("%d", i+1);
			((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(i, number);
			Personal personal = this->addressBook->GetAt(i);
			name = CString(personal.GetName().c_str());
			((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 1, name);

			address = CString(personal.GetAddress().c_str());
			((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 2, address);

			telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
			((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 3, telephoneNumber);

			emailAddress = CString(personal.GetEmailAddress().c_str());
			((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 4, emailAddress);
			i++;
		}
		
	}

	return FALSE;
}
void UpdatingForm::OnRecordButtonClicked() {
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(name);
	GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->GetWindowText(telephoneNumber);
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);
	Long index = this->addressBook->Record((LPCSTR)name, (LPCSTR)address,
		(LPCSTR)telephoneNumber, (LPCSTR)emailAddress);
	Insert(index);

	CString number;
	number.Format("%d", index + 1);
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(index, number);
	Personal personal = this->addressBook->GetAt(index);
	name = CString(personal.GetName().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 1, name);

	address = CString(personal.GetAddress().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 2, address);

	telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 3, telephoneNumber);

	emailAddress = CString(personal.GetEmailAddress().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 4, emailAddress);
}
void UpdatingForm::OnFindButtonClicked() {
	FindingForm findingForm;
	findingForm.DoModal();
}
void UpdatingForm::OnCorrectButtonClicked() {
	CString address;
	CString telephoneNumber;
	CString emailAddress;

	GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->GetWindowText(telephoneNumber);
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);
	Long index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();
	index = this->addressBook->Correct(index, (LPCSTR)address, (LPCSTR)telephoneNumber,
		(LPCSTR)emailAddress);
	Modify(index);
	Personal personal = this->addressBook->GetAt(index);
	address = CString(personal.GetAddress().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 2, address);

	telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 3, telephoneNumber);

	emailAddress = CString(personal.GetEmailAddress().c_str());
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 4, emailAddress);

}
void UpdatingForm::OnEraseButtonClicked() {
	CString number;
	Long index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();
	this->addressBook->Erase(index);
	Delete(index);

	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteItem(index);

	while (index < this->addressBook->GetLength()) {
		number.Format("%d", index + 1);
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(index, 0, number);
		index++;
	}
}
void UpdatingForm::OnArrangeButtonClicked() {
	Long i = 0;
	Personal personal;
	CString number;
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;

	this->addressBook->Arrange();
	Save();
	((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->DeleteAllItems();
	while (i < this->addressBook->GetLength()) {
		personal = this->addressBook->GetAt(i);
		number.Format("%d", i + 1);
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->InsertItem(i, number);
		name = CString(personal.GetName().c_str());
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 1, name);

		address = CString(personal.GetAddress().c_str());
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 2, address);

		telephoneNumber = CString(personal.GetTelephoneNumber().c_str());
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 3, telephoneNumber);

		emailAddress = CString(personal.GetEmailAddress().c_str());
		((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->SetItemText(i, 4, emailAddress);
		i++;
	}

}
void UpdatingForm::OnListViewItemDoubleClicked(
	NMHDR *pNotifyStruct, LRESULT *result) {
	Long index = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetSelectionMark();
	CString name = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 1);
	CString address = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 2);
	CString telephoneNumber = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 3);
	CString emailAddress = ((CListCtrl*)GetDlgItem(IDC_LIST_PERSONALS))->GetItemText(index, 4);

	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(name);
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	GetDlgItem(IDC_EDIT_TELEPHONENUMBER)->SetWindowText(telephoneNumber);
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);

}
void UpdatingForm::OnClose() {
	if (this->addressBook != NULL) {
		Save();
		delete this->addressBook;
	}
	EndDialog(0);
}

void UpdatingForm::Load() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.name, Personal.address, Personal.telephoneNumber, Personal.emailAddress FROM Personal;";

	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString name;
	CString address;
	CString telephoneNumber;
	CString emailAddress;

	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, name);
		rs.GetFieldValue((short)1, address);
		rs.GetFieldValue((short)2, telephoneNumber);
		rs.GetFieldValue((short)3, emailAddress);

		this->addressBook->Record((LPCTSTR)name, (LPCTSTR)address, (LPCTSTR)telephoneNumber,
			(LPCSTR)emailAddress);
		rs.MoveNext();
	}
	rs.Close();
	db.Close();
}
void UpdatingForm::Save() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql;
	Long i = 0;
	Personal personal;
	CString code("P0001");
	


	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653");
	sql.Format("SELECT Personal.code FROM Personal;");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	sql.Format("DELETE FROM Personal");
	db.ExecuteSQL(sql);

	while (i < this->addressBook->GetLength()) {
		personal = this->addressBook->GetAt(i);

		rs.GetFieldValue((short)0, code);
		sql.Format("INSERT INTO Personal(code, name, address, "
			"telephoneNumber, emailAddress) "
			"VALUES('%s', '%s', '%s', '%s', '%s');",
			(LPCSTR)code, personal.GetName().c_str(), personal.GetAddress().c_str(),
			personal.GetTelephoneNumber().c_str(), personal.GetEmailAddress().c_str());
		db.ExecuteSQL(sql);
		rs.MoveNext();
		i++;
	}
	rs.Close();
	db.Close();
}
void UpdatingForm::Insert(Long index) {
	Personal personal = this->addressBook->GetAt(index);
	CString code = MakeCode();
	CDatabase db;
	CString sql;
	sql.Format("INSERT INTO Personal(code, name, address,"
		"telephoneNumber, emailAddress) "
		"VALUES('%s', '%s', '%s', '%s', '%s');",
		(LPCSTR)code, personal.GetName().c_str(), personal.GetAddress().c_str(),
		personal.GetTelephoneNumber().c_str(), personal.GetEmailAddress().c_str());
	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653");
	db.ExecuteSQL(sql);
	db.Close();

}
void UpdatingForm::Delete(Long index) {
	CDatabase db;
	CString sql = "SELECT Personal.code FROM Personal; ";
	CRecordset rs(&db);
	CString code;
	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	rs.Move(index);

	rs.GetFieldValue((short)0, code);
	sql.Format("DELETE FROM Personal WHERE code='%s';", (LPCSTR)code);
	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}
void UpdatingForm::Modify(Long index) {
	Personal personal = this->addressBook->GetAt(index);
	CDatabase db;
	CRecordset rs(&db);
	CString code;
	CString sql = "SELECT Personal.code FROM Personal;";
	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	rs.Move(index);

	rs.GetFieldValue((short)0, code);
	sql.Format("UPDATE Personal SET address='%s', "
		"telephoneNumber='%s', emailAddress='%s' "
		"WHERE Personal.code='%s'; ",
		personal.GetAddress().c_str(), personal.GetTelephoneNumber().c_str(),
		personal.GetEmailAddress().c_str(), (LPCTSTR)code);
	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}

CString UpdatingForm::MakeCode() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal ORDER BY code DESC; ";
	CString code("P0001");

	db.OpenEx("DSN=AddressBook;UID=root;PWD=kim653;");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, code);
		code = code.Right(4);
		int codeNumber = atoi((LPCSTR)code);
		code.Format("P%04d", codeNumber + 1);
	}
	rs.Close();
	db.Close();
	return code;
}