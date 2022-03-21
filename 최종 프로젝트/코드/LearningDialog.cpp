#include "LearningDialog.h"
#include "NeuronForm.h"
#include "LearningForm.h"
#include "afxcmn.h"
#include "NeuralNetwork.h"
#include "LearningForm.h"
#include "Trainer.h"
#include "GraphMaker.h"
#include "Graph.h"
#include "RoundRectangle.h"
#include "RecognizingForm.h"
#include "Icon.h"
BEGIN_MESSAGE_MAP(LearningDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnOKButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_BN_CLICKED(IDC_RADIO_SGD, OnSGDRadioButtonClicked)
	ON_BN_CLICKED(IDC_RADIO_ADAM, OnAdamRadioButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

LearningDialog::LearningDialog(CWnd *parent)
	: CDialog(LearningDialog::IDD, parent) {
	this->neuronForm = (NeuronForm*)parent;
}

BOOL LearningDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_RADIO_SGD))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_ADAM))->SetCheck(BST_UNCHECKED);

	return FALSE;
}

void LearningDialog::OnOKButtonClicked() {
	Long batchSize;
	Long repeatCount;
	double learningRate;
	double beta1;
	double beta2;
	CString string;

	GetDlgItem(IDC_EDIT_BATCH)->GetWindowTextA(string);
	batchSize = atoi(string);
	GetDlgItem(IDC_EDIT_REPEATCOUNT)->GetWindowTextA(string);
	repeatCount = atoi(string);
	GetDlgItem(IDC_EDIT_LEARNINGRATE)->GetWindowTextA(string);
	learningRate = atof(string);


	int checkOfSGD = ((CButton*)GetDlgItem(IDC_RADIO_SGD))->GetCheck();
	int checkOfAdam = ((CButton*)GetDlgItem(IDC_RADIO_ADAM))->GetCheck();

	if (this->neuronForm->learningForm->accuracyCoordinateSystem != NULL) {
		delete this->neuronForm->learningForm->accuracyCoordinateSystem;
	}
	if (this->neuronForm->learningForm->lossCoordinateSystem != NULL) {
		delete this->neuronForm->learningForm->lossCoordinateSystem;
	}

	GraphMaker graphMaker;
	this->neuronForm->learningForm->accuracyCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "��Ȯ��", CRect(100, 350, 700, 530), repeatCount, 1);

	this->neuronForm->learningForm->lossCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "����", CRect(100, 50, 700, 230), repeatCount, 5);

	this->neuronForm->learningForm->progressBar.ShowWindow(SW_NORMAL);
	this->neuronForm->learningForm->progressBar.SetRange32(0, repeatCount);

	//�н����� �н� ��ư�� ���ִ�.
	if (this->neuronForm->learningForm->startButton != NULL) {
		delete this->neuronForm->learningForm->startButton;
		this->neuronForm->learningForm->startButton = NULL;
	}
	//�н����� �ߴ� ��ư�� �����.
	this->neuronForm->learningForm->stopButton = new RoundRectangle(CRect(600, 740, 700, 780), "�ߴ�", RGB(255, 0, 0), false);

	if (checkOfSGD == BST_CHECKED) {
		this->neuronForm->learningForm->trainer->Train(batchSize, repeatCount, learningRate);
	}
	else if (checkOfAdam == BST_CHECKED) {
		GetDlgItem(IDC_EDIT_BETA1)->GetWindowTextA(string);
		beta1 = atof(string);
		GetDlgItem(IDC_EDIT_BETA2)->GetWindowTextA(string);
		beta2 = atof(string);
		this->neuronForm->learningForm->trainer->Train(batchSize, repeatCount, learningRate, beta1, beta2);
	}

	// �߷��ϴ� ���� �߷� �������� ���ִ�.
	if (this->neuronForm->recognizingForm->recognizeButton != NULL) {
		delete this->neuronForm->recognizingForm->recognizeButton;
		this->neuronForm->recognizingForm->recognizeButton = NULL;
	}
	if (this->neuronForm->recognizingForm->correctButton != NULL) {
		delete this->neuronForm->recognizingForm->correctButton;
		this->neuronForm->recognizingForm->correctButton = NULL;
	}
	if (this->neuronForm->recognizingForm->wrongButton != NULL) {
		delete this->neuronForm->recognizingForm->wrongButton;
		this->neuronForm->recognizingForm->wrongButton = NULL;
	}

	//�߷��ϴ� ���� �������� �ʱ�ȭ�Ѵ�.
	this->neuronForm->recognizingForm->recognizationResult = { 0, };
	Long i = 0;
	while (i < this->neuronForm->recognizingForm->results.GetLength()) {
		this->neuronForm->recognizingForm->results.Modify(i, 0.0);
		i++;
	}
	this->neuronForm->recognizingForm->isPaint = FALSE;
	this->neuronForm->recognizingForm->Invalidate();

	EndDialog(0);
}

void LearningDialog::OnCancelButtonClicked() {
	EndDialog(0);
}

void LearningDialog::OnSGDRadioButtonClicked() {
	GetDlgItem(IDC_STATIC_BETA1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BETA1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_BETA2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BETA2)->EnableWindow(FALSE);
}

void LearningDialog::OnAdamRadioButtonClicked() {
	GetDlgItem(IDC_STATIC_BETA1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BETA1)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_BETA2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BETA2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LEARNINGRATE)->SetWindowText("0.001");
	GetDlgItem(IDC_EDIT_BETA1)->SetWindowText("0.9");
	GetDlgItem(IDC_EDIT_BETA2)->SetWindowText("0.999");
}

void LearningDialog::OnClose() {
	EndDialog(0);
}



