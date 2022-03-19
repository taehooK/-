#ifndef _EDITWORKS_H
#define _EDITWORKS_H

typedef signed long int Long;

class Glyph;
class EditWork {
public:
	EditWork();
	EditWork(Glyph* glyph, Long row, Long column);
	EditWork(const EditWork& source);
	~EditWork();
	EditWork& operator=(const EditWork& other);

	Glyph* GetGlyph();
	Long GetRow();
	Long GetColumn();

private:
	Glyph *glyph;
	Long row;
	Long column;
};

inline Long EditWork::GetRow(){
	return this->row;
}

inline Long EditWork::GetColumn() {
	return this->column;
}

class Adding : public EditWork {
public:
	Adding();
	Adding(Glyph* glyph, Long row, Long column);
	Adding(const Adding& source);
	~Adding();
	Adding& operator=(const Adding& other);
};

class Removing : public EditWork {
public:
	Removing();
	Removing(Glyph* glyph, Long row, Long column);
	Removing(const Removing& source);
	~Removing();
	Removing& operator=(const Removing& other);
};



#endif // !_EDITWORKS_H
