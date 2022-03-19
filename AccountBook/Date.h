#ifndef _DATE_H
#define _DATE_H

typedef signed short int Short;
enum Month { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };
enum WeekDay { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6 };
class Date {
public:
	Date(); //持失切 Constructor
	Date(Short year, Month month, Short day);
	Date(const char(*date));
	~Date(); //社瑚切 Destructor
	Date(const Date & source);
	static Date Today();
	Date Yesterday();
	Date Tomorrow();
	Date PreviousDate(Short days);
	Date NextDate(Short days);
	bool IsEqual(const Date& other);
	bool IsNotEqual(const Date& other);
	bool IsLesserThan(const Date& other);
	bool IsGreaterThan(const Date& other);
	Short GetYear() const;
	Short GetMonth() const;
	Short GetDay() const;
	Short GetWeekDay() const;
	operator char*();
	Date& operator --();
	Date operator --(int);
	Date& operator ++();
	Date operator ++(int);
	Date operator -(short days);
	Date operator +(short days);
	bool operator ==(const Date& other);
	bool operator !=(const Date& other);
	bool operator <(const Date& other);
	bool operator <=(const Date& other);
	bool operator >(const Date& other);
	bool operator >=(const Date& other);
	Date& operator =(const Date& source);
private:
	Short year;
	Month month;
	Short day;
	WeekDay weekDay;
};
inline Short Date::GetYear() const {
	return this->year;
}
inline Short Date::GetMonth() const {
	return this->month;
}
inline Short Date::GetDay() const {
	return this->day;
}
inline Short Date::GetWeekDay() const {
	return this->weekDay;
}

#endif //_DATE_H