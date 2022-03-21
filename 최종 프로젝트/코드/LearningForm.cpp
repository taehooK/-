#include "LearningForm.h"
#include "Graph.h"
#include "LearningDialog.h"
#include "GraphMaker.h"
#include "Visitor.h"
#include "DrawingVisitor.h"
#include "CoordinateSystem.h"
#include "CompositeGraph.h"
#include "resource.h"
#include "Trainer.h"
#include "RoundRectangle.h"
#include "RecognizingForm.h"
#include "NeuronForm.h"
#include "Icon.h"
#include "Circle.h"
#include "Cross.h"
#include <string>

using namespace std;

BEGIN_MESSAGE_MAP(LearningForm, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP();

LearningForm::LearningForm(NeuronForm* neuronForm) {
	this->neuronForm = neuronForm;
	this->accuracyCoordinateSystem = NULL;
	this->lossCoordinateSystem = NULL;
	this->trainer = NULL;
	this->startButton = NULL;
	this->stopButton = NULL;
	this->reStartButton = NULL;
	this->endButton = NULL;
	this->timeText = NULL;
}

LearningForm::~LearningForm() {
	if (this->trainer != NULL) {
		delete this->trainer;
	}
	if (this->lossCoordinateSystem != NULL) {
		delete this->lossCoordinateSystem;
	}
	if (this->accuracyCoordinateSystem != NULL) {
		delete this->accuracyCoordinateSystem;
	}
	if (this->startButton != NULL) {
		delete this->startButton;
	}
	if (this->endButton != NULL) {
		delete this->endButton;
	}
	if (this->stopButton != NULL) {
		delete this->stopButton;
	}
	if (this->reStartButton != NULL) {
		delete this->reStartButton;
	}
	if (this->timeText != NULL)
	{
		delete this->timeText;
	}
}

int LearningForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	GraphMaker graphMaker;
	this->lossCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "����", CRect(100, 50, 700, 230), 600, 5);
	this->accuracyCoordinateSystem = graphMaker.Make("�ݺ�Ƚ��", "��Ȯ��", CRect(100, 350, 700, 530), 600, 1);

	RECT rect = { 600, 740, 700, 780 };
	COLORREF color = RGB(0, 0, 255);
	this->startButton = new RoundRectangle(rect, "�н� ����", color, false);

	this->progressBar.Create(PBS_SMOOTH, CRect(100, 650, 700, 700), this, IDC_PROGRESS_LEARNING);
	this->progressBar.ShowWindow(SW_HIDE);
	this->UpdateWindow();

	this->trainer = new Trainer(this);


	return 0;
}

void LearningForm::OnPaint() {
	CPaintDC dc(this);
	CDC memDC;
	CBitmap bitmap;
	CBitmap *oldBitmap;

	memDC.CreateCompatibleDC(&dc);

	CRect clientRect;
	this->GetClientRect(&clientRect);
	bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	oldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(&clientRect, RGB(248, 248, 248));

	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	CBrush *oldBrush = memDC.SelectObject(&brush);

	DrawingVisitor *drawingVisitor = new DrawingVisitor(&memDC);

	if (this->lossCoordinateSystem != NULL) {
		this->lossCoordinateSystem->Accept(drawingVisitor);
	}
	if (this->accuracyCoordinateSystem != NULL) {
		this->accuracyCoordinateSystem->Accept(drawingVisitor);
	}
	if (this->startButton != NULL) {
		this->startButton->Accept(drawingVisitor);
	}
	if (this->stopButton != NULL) {
		this->stopButton->Accept(drawingVisitor);
	}
	if (this->reStartButton != NULL) {
		this->reStartButton->Accept(drawingVisitor);
	}
	if (this->endButton != NULL) {
		this->endButton->Accept(drawingVisitor);
	}
	if (this->timeText != NULL && this->trainer->GetIsTraining() == true)
	{
		this->timeText->Accept(drawingVisitor);
	}

	//this->progressBar.ShowWindow(SW_SHOW);
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);

	if (drawingVisitor != NULL) {
		delete drawingVisitor;
	}

	memDC.SelectObject(oldBitmap);
	memDC.SelectObject(oldBrush);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void LearningForm::OnLButtonUp(UINT nFlags, CPoint point) {
	RECT rect;

	if (this->startButton != NULL) {
		delete this->startButton;
		this->startButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(0, 0, 255), false);
		this->Invalidate();
		rect = this->startButton->GetRect();
		if ((rect.left <= point.x && rect.right >= point.x) && (rect.top <= point.y && rect.bottom >= point.y)) {
			LearningDialog learningDialog((CWnd*)this->neuronForm);
			learningDialog.DoModal();
		}

	}
	else if (this->stopButton != NULL) { // RECT rect = { 500, 900, 600, 930 }; rect = { 630, 900, 730, 930 }; //  new RoundRectangle(rect, "�н� ����", color);
		delete this->stopButton;
		this->stopButton = new RoundRectangle(CRect(600, 740, 700, 780), "�ߴ�", RGB(255, 0, 0), false);
		rect = this->stopButton->GetRect();
		if ((rect.left <= point.x && rect.right >= point.x) && (rect.top <= point.y && rect.bottom >= point.y)) {
			this->trainer->Pause();
			this->reStartButton = new RoundRectangle(CRect(490, 740, 590, 780), "�����", RGB(0, 0, 255), false);
			this->endButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(255, 0, 0), false);
			delete this->stopButton;
			this->stopButton = NULL;
		}

	}
	else if (this->reStartButton != NULL && this->endButton != NULL) {
		delete this->reStartButton;
		this->reStartButton = new RoundRectangle(CRect(490, 740, 590, 780), "�����", RGB(0, 0, 255), false);
		delete this->endButton;
		this->endButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(255, 0, 0), false);
		RECT reStartrect = this->reStartButton->GetRect();
		RECT endRect = this->endButton->GetRect();
		if ((reStartrect.left <= point.x && reStartrect.right >= point.x) && (reStartrect.top <= point.y && reStartrect.bottom >= point.y)) {//3. ����� ��ư�� Ŭ������ ��
			this->trainer->Resume(); // 3.1.Ʈ���� �����忡�� ����� �Ѵ�.
			//3.2.����� ��ư�� ���ִ�.
			delete this->reStartButton;
			this->reStartButton = NULL;
			//3.3.���� ��ư�� ���ִ�.
			delete this->endButton;
			this->endButton = NULL;
			//3.4.�ߴ� ��ư�� �����.
			this->stopButton = new RoundRectangle(CRect(600, 740, 700, 780), "�ߴ�", RGB(255, 0, 0), false);
			if (this->neuronForm->recognizingForm->recognizeButton != NULL) {
				delete this->neuronForm->recognizingForm->recognizeButton;
				this->neuronForm->recognizingForm->recognizeButton = NULL;
				this->neuronForm->recognizingForm->Invalidate();
			}

		}
		else if ((endRect.left <= point.x && endRect.right >= point.x) && (endRect.top <= point.y && endRect.bottom >= point.y)) {//4. �н� ���� ��ư�� Ŭ������ ��
				//4.1.Ʈ���� �����忡�� �����Ѵ�.
			this->trainer->End();
			//4.2.�н� ���� ��ư�� �׸���.
			this->startButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(0, 0, 255), false);
			//4.3.����� ��ư�� ���ִ�.
			delete this->reStartButton;
			this->reStartButton = NULL;
			//4.4.�н� �����ư�� ���ִ�
			delete this->endButton;
			this->endButton = NULL;

			// 4.5�߷��ϴ� ���� �߷� ��ư�� �����.
			this->neuronForm->recognizingForm->recognizeButton = new Icon(CRect(30, 340, 100, 410), RGB(255, 255, 255), IDB_RECOGNIZE_OFF, false);
			this->neuronForm->recognizingForm->correctButton = new Circle(CRect(120, 355, 160, 395), RGB(0, 102, 255), false);
			this->neuronForm->recognizingForm->wrongButton = new Cross(CRect(190, 355, 230, 395), RGB(255, 0, 102), false);
			this->neuronForm->recognizingForm->Invalidate();
		}
	}

	this->Invalidate();
}

void LearningForm::OnLButtonDown(UINT nFlags, CPoint point) {
	RECT rect;

	if (this->startButton != NULL) {
		rect = this->startButton->GetRect();
		if ((rect.left <= point.x && rect.right >= point.x) && (rect.top <= point.y && rect.bottom >= point.y)) {
			delete this->startButton;
			this->startButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(0, 0, 255), true);
		}

	}
	else if (this->stopButton != NULL) { // RECT rect = { 500, 900, 600, 930 }; rect = { 630, 900, 730, 930 }; //  new RoundRectangle(rect, "�н� ����", color);
		rect = this->stopButton->GetRect();
		if ((rect.left <= point.x && rect.right >= point.x) && (rect.top <= point.y && rect.bottom >= point.y)) {
			delete this->stopButton;
			this->stopButton = new RoundRectangle(CRect(600, 740, 700, 780), "�ߴ�", RGB(255, 0, 0), true);
		}

	}
	else if (this->reStartButton != NULL && this->endButton != NULL) {
		RECT reStartrect = this->reStartButton->GetRect();
		RECT endButton = this->endButton->GetRect();
		if ((reStartrect.left <= point.x && reStartrect.right >= point.x) && (reStartrect.top <= point.y && reStartrect.bottom >= point.y)) {//3. ����� ��ư�� Ŭ������ ��
			delete this->reStartButton;
			this->reStartButton = new RoundRectangle(CRect(490, 740, 590, 780), "�����", RGB(0, 0, 255), true);
		}
		else if ((endButton.left <= point.x && endButton.right >= point.x) && (endButton.top <= point.y && endButton.bottom >= point.y)) {//4. �н� ���� ��ư�� Ŭ������ ��
			delete this->endButton;
			this->endButton = new RoundRectangle(CRect(600, 740, 700, 780), "�н� ����", RGB(255, 0, 0), true);
		}
	}

	this->Invalidate();
}

void LearningForm::OnMouseMove(UINT nFlags, CPoint point) {
	CRect rect;
	if (this->startButton != NULL) {
		rect = this->startButton->GetRect();
		if (rect.left <= point.x && rect.right >= point.x && rect.top <= point.y && rect.bottom >= point.y) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	}
	if (this->stopButton != NULL) {
		rect = this->stopButton->GetRect();
		if (rect.left <= point.x && rect.right >= point.x && rect.top <= point.y && rect.bottom >= point.y) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	}
	if (this->reStartButton != NULL) {
		rect = this->reStartButton->GetRect();
		if (rect.left <= point.x && rect.right >= point.x && rect.top <= point.y && rect.bottom >= point.y) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	}
	if (this->endButton != NULL) {
		rect = this->endButton->GetRect();
		if (rect.left <= point.x && rect.right >= point.x && rect.top <= point.y && rect.bottom >= point.y) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void LearningForm::OnClose() {
	if (this->lossCoordinateSystem != NULL) {
		delete this->lossCoordinateSystem;
	}
	if (this->accuracyCoordinateSystem != NULL) {
		delete this->accuracyCoordinateSystem;
	}
	if (this->trainer != NULL) {
		delete this->trainer;
	}
	if (this->startButton != NULL) {
		delete this->startButton;
	}
	if (this->endButton != NULL) {
		delete this->endButton;
	}
	if (this->stopButton != NULL) {
		delete this->stopButton;
	}
	if (this->reStartButton != NULL) {
		delete this->reStartButton;
	}

	CWnd::OnClose();
}


