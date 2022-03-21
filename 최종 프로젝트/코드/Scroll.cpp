#include "Scroll.h"

Scroll::Scroll() {
	this->position = 0;
	this->minimum = 0;
	this->maximum = 0;
	this->lineSize = 0;
	this->pageSize = 0;
}

Scroll::Scroll(Long position, Long minimum, Long maximum, Long lineSize, Long pageSize) {
	this->position = position;
	this->minimum = minimum;
	this->maximum = maximum;
	this->lineSize = lineSize;
	this->pageSize = pageSize;
}

Scroll::Scroll(const Scroll& source) {
	this->position = source.position;
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->lineSize = source.lineSize;
	this->pageSize = source.pageSize;
}

Scroll::~Scroll() {

}

Long Scroll::Up() {
	this->position += this->lineSize;
	if (this->position > (this->maximum - this->pageSize + 1)) {
		this->position = this->maximum - this->pageSize + 1; // 1�� ���ϴ� ������ ������� ȣȯ�� ����
	}
	return this->position;
}

Long Scroll::Down() {
	this->position -= this->lineSize;
	if (this->position < this->minimum) {
		this->position = this->minimum;
	}
	return this->position;
}
Long Scroll::PageUp() {
	this->position += this->pageSize;
	if (this->position > (this->maximum - this->pageSize + 1)) {
		this->position = (this->maximum - this->pageSize + 1);
	}
	return this->position;
}

Long Scroll::PageDown() {
	this->position -= this->pageSize;
	if (this->position < this->minimum) {
		this->position = this->minimum;
	}
	return this->position;
}

Long Scroll::Move(Long position) {
	this->position = position;
	return this->position;
}

Long Scroll::Rotate(short delta) {
	this->position += -delta / WHEEL_DELTA * this->lineSize;
	if (this->position > (this->maximum - this->pageSize + 1)) {
		this->position = this->maximum - this->pageSize + 1;
	}
	else if (this->position < this->minimum) {
		this->position = this->minimum;
	}

	return this->position;
}

SCROLLINFO Scroll::GetScrollInfo() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = this->minimum;
	scrollInfo.nMax = this->maximum;
	scrollInfo.nPage = this->pageSize;
	scrollInfo.nPos = this->position;

	return scrollInfo;
}

Scroll& Scroll::operator=(const Scroll& source) {
	this->position = source.position;
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->lineSize = source.lineSize;
	this->pageSize = source.pageSize;

	return *this;
}