#include "ScrollController.h"
#include "Scroll.h"
#include "NotepadForm.h"
#include "CharacterMetrics.h"
#include "Subject.h"
#include "Line.h"

ScrollController::ScrollController(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
	this->notepadForm->noteSize.cx = this->notepadForm->characterMetrics->GetNoteWidth(this->notepadForm->note); 

	Long index = this->notepadForm->note->GetLength();
	this->notepadForm->noteSize.cy = this->notepadForm->characterMetrics->GetHeight(index);

	RECT rect;
	this->notepadForm->GetClientRect(&rect);
	Long clientWidth = rect.right - rect.left;
	Long clientHeight = rect.bottom - rect.top; //

	Long maximum = this->notepadForm->noteSize.cx - clientWidth;
	Long lineSize = this->notepadForm->characterMetrics->GetWidthAverage();
	Long pageSize = clientWidth;//
	
	this->scrolls[0] = new HorizontalScroll(0, maximum, pageSize, lineSize, 0);

	maximum = this->notepadForm->noteSize.cy - clientHeight;
	lineSize = this->notepadForm->characterMetrics->GetHeight();
	pageSize = clientHeight;////
	
	this->scrolls[1] = new VerticalScroll(0, maximum, pageSize, lineSize, 0);

	this->notepadForm->AttachObserver(this);
}

ScrollController::ScrollController(const ScrollController& source) {
	this->notepadForm = source.notepadForm;
	this->scrolls[0] = new HorizontalScroll(*(dynamic_cast<HorizontalScroll*>(source.scrolls[0])));
	this->scrolls[1] = new VerticalScroll(*(dynamic_cast<VerticalScroll*>(source.scrolls[1])));

}

ScrollController::~ScrollController() {
	for (int i = 0; i < SCROLLCOUNT; i++) {
		if (this->scrolls[i] != 0) {
			delete this->scrolls[i];
		}
	}

	LONG style = ::GetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE);
	style = style & ~WS_VSCROLL;
	style = style & ~WS_HSCROLL;
	::SetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE, style);

	this->notepadForm->DettachObserver(this);
}

void ScrollController::Update() {
	RECT rect;
	Long lineWidth;
	Long clientWidth;
	Long clientHeight;
	Long index;
	Long maximum;
	Long minimum;
	Long position;
	Long lineSize;
	Long pageSize;
	SCROLLINFO scrollInfo = { 0, };
	LONG style;
	Long noteLength;
	Long previousPosition;

	this->notepadForm->GetClientRect(&rect);

	// 수평 스크롤 생성 부분

	// 노트 가로사이즈, 클라이언트 가로사이즈 구하는 부분
	noteLength = this->notepadForm->note->GetLength();
	if (noteLength <= 1 && this->notepadForm->current->GetLength() <= 0) { // 노트의 아무것도 없으면
		this->notepadForm->noteSize.cx = 0;
	}
	else {
		index = this->notepadForm->current->GetLength();
		lineWidth = this->notepadForm->characterMetrics->GetWidth(this->notepadForm->current, index);
		if (lineWidth > this->notepadForm->noteSize.cx) {
			this->notepadForm->noteSize.cx = lineWidth;
		}
	}
	clientWidth = rect.right - rect.left;
	style = ::GetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE);
	

	if (clientWidth < this->notepadForm->noteSize.cx) { // 클라이언트 영역이 더 작으면 스크롤 설정및 생성
		style = style | WS_HSCROLL;
		minimum = this->scrolls[0]->GetMinimum();
		maximum = this->notepadForm->noteSize.cx;//
		position = this->scrolls[0]->GetPosition();
		lineSize = this->notepadForm->characterMetrics->GetWidthAverage();
		pageSize = clientWidth;

		*this->scrolls[0] = HorizontalScroll(minimum, maximum, pageSize, lineSize, position);
	}
	else {
		style = style & ~WS_HSCROLL;	
		*this->scrolls[0] = HorizontalScroll(0, 0, 0, 0, 0);
		
	}
	scrollInfo = this->scrolls[0]->GetScrollInfo();

	// 수평 스크롤 처리
	this->notepadForm->SetScrollInfo(SB_HORZ, &scrollInfo, TRUE);

	// 수직 스크롤 처리
	index = this->notepadForm->note->GetLength();
	this->notepadForm->noteSize.cy = this->notepadForm->characterMetrics->GetHeight(index);
	clientHeight = rect.bottom - rect.top;
	
	
	style = style | WS_VSCROLL;
	minimum = this->scrolls[1]->GetMinimum();
	lineSize = this->notepadForm->characterMetrics->GetHeight();
	maximum = this->notepadForm->noteSize.cy;
	position = this->scrolls[1]->GetPosition();
	pageSize = clientHeight / lineSize * lineSize;
	
	// position 동기화
	if (clientHeight < maximum && (maximum + 1) < position + pageSize  ) {
		previousPosition = position;
		position -= (position + pageSize) - maximum;
	
		this->notepadForm->ScrollWindow(0, previousPosition - position);
	}
	
	
	if (clientHeight < maximum) {
		*this->scrolls[1] = VerticalScroll(minimum, maximum, pageSize, lineSize, position);
	}
	else {
		style = style & ~WS_VSCROLL;
		*this->scrolls[1] = VerticalScroll(0, 0, 0, 0, 0);
	}

	scrollInfo = this->scrolls[1]->GetScrollInfo();
	this->notepadForm->SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	::SetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE, style);
	this->notepadForm->RedrawWindow();
}

Observer* ScrollController::GetClone() {
	return new ScrollController(*this);
}

Long ScrollController::Left() {
	return this->scrolls[0]->Down();
}

Long ScrollController::Right() {
	return this->scrolls[0]->Up();
}

Long ScrollController::PageLeft() {
	return this->scrolls[0]->PageDown();
}

Long ScrollController::PageRight() {
	return this->scrolls[0]->PageUp();
}

Long ScrollController::Up() {
	return this->scrolls[1]->Down();
}

Long ScrollController::Down() {
	return this->scrolls[1]->Up();
}

Long ScrollController::PageUp() {
	return this->scrolls[1]->PageDown();
}

Long ScrollController::PageDown() {
	return this->scrolls[1]->PageUp();
}

Long ScrollController::Rotate(short delta) {
	return this->scrolls[1]->Rotate(delta);
}

Long ScrollController::MoveHorizontalScroll(Long position) {
	return this->scrolls[0]->Move(position);
}

Long ScrollController::MoveVerticalScroll(Long position) {
	return this->scrolls[1]->Move(position);
}

void ScrollController::SmartScrollToPoint(Long x, Long y) {
	Long distanceOfX = 0;
	Long distanceOfY = 0;
	Long positionX;
	Long previousX;
	Long positionY;
	Long previousY;
	RECT rect;
	this->notepadForm->GetClientRect(&rect);
	//클라이언트 영역 좌표를 구한다.
	Long clientLeft = this->scrolls[0]->GetPosition();
	Long clientRight = this->scrolls[0]->GetPosition() + rect.right - rect.left;
	Long clientTop = this->scrolls[1]->GetPosition() + this->scrolls[1]->GetLineSize();
	Long clientBottom = this->scrolls[1]->GetPosition() + rect.bottom - rect.top;

	//x가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 왼쪽에 있으면
	if (x < clientLeft) {
		//	1.x좌표의 거리와 클라이언트 시작점을 빼다
		distanceOfX = x - clientLeft;
	}//x가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 오른쪽에 있으면
	else if (x > clientRight) {
		//	1. x좌표와 클라이언트 끝점을 빼다
		distanceOfX = x - clientRight;
	}

	positionX = this->scrolls[0]->GetPosition();//현재 위치를 구한다.
	positionX += distanceOfX;//현재 위치에서 구한값을 더하다.
	positionX = this->scrolls[0]->Move(positionX);//수직 컨트롤에서 이동하다.
	// 전 위치를 구하다.
	// 스크롤을 설정하다.
	previousX = this->notepadForm->SetScrollPos(SB_HORZ, positionX, TRUE);
	positionX = this->notepadForm->GetScrollPos(SB_HORZ);
	this->scrolls[0]->Move(positionX);

	//클라이언트 영역 좌표를 구한다.
	if (y < clientTop ) {//y가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 위에 있으면
		distanceOfY = y - clientTop;//	1.y좌표의 거리와 클라이언트 시작점을 빼다
	}
	else if (y > clientBottom) {//y가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 위에 있으면
		distanceOfY = y - clientBottom;//	1. 클라이언트 끝점과 y좌표의 거리를 빼다.
		distanceOfY += this->notepadForm->characterMetrics->GetHeight() / 2;
	}
	
	positionY = this->scrolls[1]->GetPosition();//현재 위치를 구한다.
	positionY += distanceOfY;//현재 위치에서 구한값을 더하다.
	positionY = this->scrolls[1]->Move(positionY);//스크롤 컨트롤러에서 이동하다.
	// 전 위치를 구하다.
	// 스크롤을 설정하다.
	previousY = this->notepadForm->SetScrollPos(SB_VERT, positionY, TRUE);
	positionY = this->notepadForm->GetScrollPos(SB_VERT);
	this->scrolls[1]->Move(positionY);

	//메모장 윈도우를 스크롤한다.
	this->notepadForm->ScrollWindow(previousX - positionX, previousY - positionY);
	this->notepadForm->RedrawWindow();
}