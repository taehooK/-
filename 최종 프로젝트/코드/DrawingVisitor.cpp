#include "DrawingVisitor.h"
#include "CoordinateSystem.h"
#include "Point.h"
#include "Line.h"
#include "Texts.h"
#include "Arrow.h"
#include "LayerBoxControl.h"
#include "LayerBox.h"
#include "RectangleGraph.h"
#include "AddBox.h"
#include "Cell.h"
#include "Layout.h"
#include "ScrollControllers.h"
#include "Scroll.h"
#include "RoundRectangle.h"
#include "LayerToolTip.h"
#include "resource.h"
#include "Icon.h"
#include "Circle.h"
#include "Cross.h"
#include "Table.h"

DrawingVisitor::DrawingVisitor(CDC *dc, ScrollController *scrollController) {
	this->dc = dc;
	this->scrollController = scrollController;
}

DrawingVisitor::DrawingVisitor(const DrawingVisitor& source) {
	this->dc = source.dc;
	this->scrollController = source.scrollController;
}

DrawingVisitor::~DrawingVisitor() {

}

void DrawingVisitor::Visit(Point *point) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //�� ��Ÿ�� ����
	CBrush brush;
	brush.CreateSolidBrush(point->GetColor());

	//COLORREF oldColor = this->dc->SetBkColor(point->GetColor()); //���� ����
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	this->dc->Ellipse(point->GetX() - 2, point->GetY() - 2, point->GetX() + 2, point->GetY() + 2);

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	//this->dc->SetBkColor(oldColor);

	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(CoordinateSystem *coordinateSystem) {
	//��ǥ�� �׸��°� Line���� �� : Add�Ǿ��ִ� ��� Graph�� Accept ȣ���ϱ�
	//��Ʈ ����
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);

	CFont font;
	font.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "�ü�");
	oldFont = this->dc->SelectObject(&font);

	Array<CPoint>points(coordinateSystem->GetLength());
	Graph *graph;
	Long i = 0;
	while (i < coordinateSystem->GetLength()) {
		graph = coordinateSystem->GetAt(i);
		if (graph == dynamic_cast<Point*>(graph)) {
			CPoint point= CPoint(dynamic_cast<Point*>(graph)->GetX(), dynamic_cast<Point*>(graph)->GetY());
			points.Store(points.GetLength(), point);
		}
		else {
			graph->Accept(this);
		}

		i++;
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0)); //�� ��Ÿ�� ����
	CPen *oldPen = this->dc->SelectObject(&pen);
	this->dc->Polyline(&points.GetAt(0), points.GetLength());

	this->dc->SelectObject(oldFont);
	this->dc->SelectObject(oldPen);

}

void DrawingVisitor::Visit(Line *line) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, line->GetColor()); //�� ��Ÿ�� ����
	CBrush brush;
	brush.CreateSolidBrush(line->GetColor());
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	this->dc->MoveTo(line->GetStartX(), line->GetStartY());
	this->dc->LineTo(line->GetEndX(), line->GetEndY());

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);

	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(HorizontalText *text) {
	COLORREF oldColor = this->dc->SetTextColor(text->GetColor()); //���� ����

	this->dc->TextOutA(text->GetX(), text->GetY(), text->GetContent().c_str());

	this->dc->SetTextColor(oldColor);
}

void DrawingVisitor::Visit(VerticalText *text) {
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);
	logFont.lfEscapement = 900; //����
	logFont.lfWeight = 700;

	CFont font;
	font.CreateFontIndirectA(&logFont);
	oldFont = this->dc->SelectObject(&font);

	this->dc->TextOutA(text->GetX(), text->GetY(), text->GetContent().c_str());

	this->dc->SelectObject(oldFont);
}

void DrawingVisitor::Visit(RectangleGraph *rectangleGraph) {
	this->dc->Rectangle(&rectangleGraph->GetRect());
}

void DrawingVisitor::Visit(LayerBox *layerBox) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, layerBox->GetColor()); //�� ��Ÿ�� ����
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));

	COLORREF oldColor = this->dc->SetBkColor(RGB(255, 255, 255)); //���� ����
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	RECT rect = layerBox->GetRect();
	Scroll *scroll = this->scrollController->GetScroll();
	rect.top -= scroll->GetPosition();
	rect.bottom -= scroll->GetPosition();

	this->dc->Rectangle(&rect);//�簢�� �׸���

	this->dc->MoveTo(rect.left, rect.bottom + 1);
	this->dc->LineTo(rect.right + 1, rect.bottom + 1);
	this->dc->MoveTo(rect.right + 1, rect.top);
	this->dc->LineTo(rect.right + 1, rect.bottom + 1);

	//��Ʈ ����
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);
	CFont font;
	font.CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "�ü�");

	oldFont = this->dc->SelectObject(&font);

	//��Ʈ ������ ���
	CSize size;
	CString type(layerBox->GetType().c_str());
	size = this->dc->GetTextExtent(type); //type�� dc�� �� �� ũ�� ��������
	Long typeWidth = size.cx;
	Long typeHeight = size.cy;

	//�ؽ�Ʈ �� x��ǥ=�簢�� ������ǥx + (�簢�� ��-������)/2
	Long x = rect.left + ((rect.right - rect.left) - typeWidth) / 2;
	//�ؽ�Ʈ �� y��ǥ=�簢�� ������ǥy + �簢�� ����/2 - ���ڳ���/2
	Long y = (rect.top + (rect.bottom - rect.top) / 2) - size.cy / 2;
	Text *text = new HorizontalText(x, y, layerBox->GetType(), layerBox->GetColor());

	text->Accept(this); //�ؽ�Ʈ �׸���
	if (text != NULL) {
		delete text;
	}

	this->dc->SelectObject(oldFont);

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	this->dc->SetBkColor(oldColor);

	font.DeleteObject();
	pen.DeleteObject();
	brush.DeleteObject();

}


void DrawingVisitor::Visit(LayerBoxControl *layerBoxControl) {
	Long i = 0;

	while (i < layerBoxControl->GetLength()) {
		layerBoxControl->GetAt(i)->Accept(this);
		i++;
	}
}

void DrawingVisitor::Visit(Arrow * arrow) {
	RECT rect = arrow->GetRect();
	CPen pen;
	pen.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	CPen *oldPen = this->dc->SelectObject(&pen);

	//rect�� ���ʳ� �߾����� �̵�
	dc->MoveTo(rect.left, rect.top + (rect.bottom - rect.top) / 2);
	//������ ������ �߾� ���μ� �߱�
	dc->LineTo(rect.right, rect.top + (rect.bottom - rect.top) / 2);
	//������ ������ �߾� ������ �밢�� �߱�
	dc->LineTo(rect.left + (rect.right - rect.left) / 2, rect.top);
	//�����ʳ� �߾����� �̵�
	dc->MoveTo(rect.right, rect.top + (rect.bottom - rect.top) / 2);
	//������ �� �߾ӿ��� �߾ӸǾƷ��� �밢�� �߱�
	dc->LineTo(rect.left + (rect.right - rect.left) / 2, rect.bottom);

	this->dc->SelectObject(oldPen);
}

void DrawingVisitor::Visit(AddBox* addBox) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, addBox->GetColor());
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);
	COLORREF oldColor = this->dc->SetBkColor(RGB(255, 255, 255)); //���� ����

	// 1. �簢�� ��ǥ�� �д´�.
	RECT rect = addBox->GetRect();
	Scroll *scroll = this->scrollController->GetScroll();
	rect.top -= scroll->GetPosition();
	rect.bottom -= scroll->GetPosition();

	// 2. �簢���� �׸���.
//������ �׸��� ó���� �ٲߴϴ�.
	if (addBox->GetIsClicked() == true) {
		this->dc->Rectangle(rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1);
		this->dc->Draw3dRect(&rect, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DHILIGHT));
	}
	else {
		this->dc->Rectangle(&rect);
		//��ü ȿ��
		this->dc->MoveTo(rect.left, rect.bottom + 1);
		this->dc->LineTo(rect.right + 1, rect.bottom + 1);
		this->dc->MoveTo(rect.right + 1, rect.top);
		this->dc->LineTo(rect.right + 1, rect.bottom + 1);

		this->dc->Draw3dRect(&rect, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DHIGHLIGHT));
	}



	// 3. +�� ��ġ�� ���Ѵ�.
	Long plusX = rect.left + (rect.right - rect.left) / 2; //�����߾�x
	Long plusY = rect.top + (rect.bottom - rect.top) / 2; //�����߾�y
	Long plusLength = static_cast<Long>((rect.bottom - rect.top)*0.6);
	//���ϱ� ��ȣ�� ���μ��� ����: �δ� �簢������ 60%

	if (addBox->GetIsClicked() == true) {
		plusX += 2;
		plusY += 2;
	}

	// 4. +�� �׸���.
	this->dc->MoveTo(plusX - plusLength / 2, plusY); //+�׸��� �����߾����� �̵�
	this->dc->LineTo(plusX + plusLength / 2, plusY); //�������ӱ߱�
	this->dc->MoveTo(plusX, plusY - plusLength / 2); //�߾Ӹ����� �̵�
	this->dc->LineTo(plusX, plusY + plusLength / 2); //�������ӱ߱�

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	pen.DeleteObject();
	brush.DeleteObject();

}

DrawingVisitor& DrawingVisitor::operator=(const DrawingVisitor& source) {
	this->dc = source.dc;

	return *this;
}

void DrawingVisitor::Visit(Cell* cell) {
	CBrush brush;
	brush.CreateSolidBrush(cell->GetColor());
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	RECT rect = cell->GetRect();
	Scroll *scroll = this->scrollController->GetScroll();
	rect.top -= scroll->GetPosition();
	rect.bottom -= scroll->GetPosition();

	RectangleGraph rectangleGraph(rect, cell->GetColor());
	rectangleGraph.Accept(this);

	this->dc->SelectObject(oldBrush);
	brush.DeleteObject();
}

void DrawingVisitor::Visit(Table* table) {
	Long i = 0;
	while (i < table->GetLength()) {
		table->GetAt(i)->Accept(this);
		i++;
	}
}

void DrawingVisitor::Visit(Layout *layout) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = this->dc->SelectObject(&pen);

	//��Ʈ ����
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);
	CFont font;
	font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "�ü�");

	oldFont = this->dc->SelectObject(&font);

	Long i = 0;
	while (i < layout->GetLength()) {
		layout->GetAt(i)->Accept(this);
		i++;
	}

	this->dc->SelectObject(oldFont);
	this->dc->SelectObject(oldPen);
	font.DeleteObject();
	pen.DeleteObject();
}

void DrawingVisitor::Visit(RoundRectangle *roundRectangle) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, roundRectangle->GetColor()); //�� ��Ÿ�� ����
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));

	COLORREF oldColor = this->dc->SetBkColor(RGB(255, 255, 255)); //���� ����
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);


	RECT rect = roundRectangle->GetRect();
	//�׸��� �ڵ带 �����մϴ�.
	if (roundRectangle->GetIsClicked() == true) {
		this->dc->RoundRect(rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1, 10, 10); //���� �ձ� �簢��
		this->dc->Draw3dRect(&rect, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DHILIGHT));
	}
	else {
		this->dc->RoundRect(rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1, 10, 10); //��ü ȿ��
		this->dc->RoundRect(rect.left, rect.top, rect.right, rect.bottom, 10, 10); //���� �ձ� �簢��
		this->dc->Draw3dRect(&rect, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DHIGHLIGHT));
	}

	//��Ʈ ����
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);

	CFont font;
	font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "�ü�");
	oldFont = this->dc->SelectObject(&font);

	//��Ʈ ������ ���
	CSize size;
	CString content(roundRectangle->GetContent().c_str());
	size = this->dc->GetTextExtent(content); //type�� dc�� �� �� ũ�� ��������
	Long contentWidth = size.cx;
	Long contentHeight = size.cy;

	if (roundRectangle->GetIsClicked() == true) {
		rect.right += 2;
		rect.bottom += 2;
	}
	//�ؽ�Ʈ �� x��ǥ=�簢�� ������ǥx + (�簢�� ��-������)/2
	Long x = rect.left + ((rect.right - rect.left) - contentWidth) / 2;
	//�ؽ�Ʈ �� y��ǥ=�簢�� ������ǥy + �簢�� ����/2 - ���ڳ���/2
	Long y = (rect.top + (rect.bottom - rect.top) / 2) - size.cy / 2;
	Text *text = new HorizontalText(x, y, roundRectangle->GetContent(), roundRectangle->GetColor());
	text->Accept(this); //�ؽ�Ʈ �׸���
	if (text != NULL) {
		delete text;
	}

	this->dc->SelectObject(oldFont);

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	this->dc->SetBkColor(oldColor);

	font.DeleteObject();
	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(LayerToolTip *layerToolTip) {
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //�� ��Ÿ�� ����
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));

	//COLORREF oldColor; //= this->dc->SetBkColor(RGB(255, 255, 255)); //���� ����
	CPen *oldPen = this->dc->SelectObject(&pen);
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	//��Ʈ ����	
	CFont *oldFont;
	CFont *currentFont = this->dc->GetCurrentFont();
	LOGFONT logFont;
	currentFont->GetLogFont(&logFont);

	CFont font;
	font.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "�ü�");
	oldFont = this->dc->SelectObject(&font);

	Long i = 0;
	while (i < layerToolTip->GetLength()) {
		layerToolTip->GetAt(i)->Accept(this);
		i++;
	}

	this->dc->SelectObject(oldFont);

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	//this->dc->SetBkColor(oldColor);

	font.DeleteObject();
	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(Icon *icon) {
	CPen pen;
	//pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //�� ��Ÿ�� ����
	CBrush brush;
	//brush.CreateSolidBrush(RGB(235, 235, 235));

	CRect rect = icon->GetRect();

	CDC memDC;
	memDC.CreateCompatibleDC(this->dc);
	BITMAP bitmapInfo;
	CBitmap bitmap;
	CBitmap *oldBitmap;

	bitmap.LoadBitmapA(icon->GetID());

	bitmap.GetBitmap(&bitmapInfo);
	oldBitmap = memDC.SelectObject(&bitmap);
	this->dc->TransparentBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, RGB(255, 255, 255));

	memDC.SelectObject(oldBitmap);

	memDC.DeleteDC();
	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(Circle *circle) {
	CPen pen;
	CPen *oldPen;
	CBrush brush;
	brush.CreateSolidBrush(RGB(248, 248, 248));

	//COLORREF oldColor = this->dc->SetBkColor(RGB(248, 248, 248)); //���� ����
	CBrush *oldBrush = this->dc->SelectObject(&brush);

	RECT rect = circle->GetRect();
	//�׸��� �ڵ带 �����մϴ�.
	if (circle->GetIsClicked() == true) {
		pen.CreatePen(PS_SOLID, 13, RGB(0, 0, 255)); //�� ��Ÿ�� ����
		oldPen = this->dc->SelectObject(&pen);
		this->dc->Ellipse(rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1);
	}
	else {
		pen.CreatePen(PS_SOLID, 13, circle->GetColor()); //�� ��Ÿ�� ����
		oldPen = this->dc->SelectObject(&pen);
		this->dc->Ellipse(&rect);
	}

	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);
	//this->dc->SetBkColor(oldColor);

	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawingVisitor::Visit(Cross *cross) {
	CPen pen;
	CPen *oldPen;
	CBrush brush;
	brush.CreateSolidBrush(cross->GetColor());

	CBrush *oldBrush = this->dc->SelectObject(&brush);

	RECT rect = cross->GetRect();
	//�׸��� �ڵ带 �����մϴ�.
	if (cross->GetIsClicked() == true) {
		pen.CreatePen(PS_SOLID, 13, RGB(255, 0, 0)); //�� ��Ÿ�� ����
		oldPen = this->dc->SelectObject(&pen);
		this->dc->MoveTo(rect.left + 1, rect.top + 1);
		this->dc->LineTo(rect.right + 1, rect.bottom + 1);
		this->dc->MoveTo(rect.left + 1, rect.bottom + 1);
		this->dc->LineTo(rect.right + 1, rect.top + 1);
	}
	else {
		pen.CreatePen(PS_SOLID, 13, cross->GetColor()); //�� ��Ÿ�� ����
		oldPen = this->dc->SelectObject(&pen);
		this->dc->MoveTo(rect.left, rect.top);
		this->dc->LineTo(rect.right, rect.bottom);
		this->dc->MoveTo(rect.left, rect.bottom);
		this->dc->LineTo(rect.right, rect.top);
	}


	this->dc->SelectObject(oldPen);
	this->dc->SelectObject(oldBrush);

	pen.DeleteObject();
	brush.DeleteObject();
}