// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp = 10) : BitField(mp)
{
	MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
	MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
	MaxPower = bf.GetLength();
	BitField = bf;
}

TSet::operator TBitField()
{
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	if ( ( Elem < 0 ) || ( Elem > MaxPower ) ) return -1;
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	if ( ( Elem < 0 ) || ( Elem > MaxPower ) ) throw Elem;
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	if ( ( Elem < 0 ) || ( Elem > MaxPower ) ) throw Elem;
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
	if ( MaxPower != s.MaxPower ) MaxPower = s.MaxPower;
	BitField = s.BitField;
	return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
	if ( MaxPower != s.MaxPower ) return 0;
	else return ( BitField == s.BitField );
}

int TSet::operator!=(const TSet &s) const // сравнение
{
	if ( MaxPower != s.MaxPower ) return 1;
	else return ( BitField != s.BitField );
}

TSet TSet::operator+(const TSet &s) // объединение
{
	return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	if (IsMember(Elem) == 1)
		return *this;
	else
	{
		BitField.SetBit(Elem);
		return *this;
	}
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	if (IsMember(Elem) == 0)
		return *this;
	else
	{
		BitField.ClrBit(Elem);
		return *this;
	}
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	
	return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
	
	return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
	int i;
	istr >> i;
	while (i >= 0) {
		if (i == 0) s.BitField.ClrBit(i);
		else s.BitField.SetBit(i);
		istr >> i;
	}
	return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
	for (int i = 0; i < s.BitField.GetLength(); i++)
	{
		if (s.IsMember(i) == 1) 
			ostr << i << " ";
	}
	return ostr;
}