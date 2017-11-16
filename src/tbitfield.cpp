// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if ( len < 0 ) throw len;
	BitLen = len;
	MemLen = BitLen / ( 8 * sizeof(TELEM) ) + 1;
	pMem = new TELEM[MemLen];

	for ( int i = 0; i < MemLen; i++ )
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for ( int i = 0; i < MemLen; i++ )
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ( ( n < 0 ) || ( n > BitLen ) ) throw n;
	return n/(sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ( ( n < 0 ) || ( n > BitLen ) ) throw n;
	return 1 << ( n % ( sizeof(TELEM) * 8 ) );
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ( ( n < 0 ) || ( n > BitLen ) )	throw n;
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{	
	if ( ( n < 0 ) || ( n > BitLen ) )	throw n;
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ( ( n < 0 ) || ( n > BitLen ) ) 	throw n;
	return ( pMem[GetMemIndex(n)] & GetMemMask(n)) >> ( n % ( sizeof(TELEM) * 8 + 1 ) );
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if ( BitLen != bf.BitLen ) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for ( int i = 0; i < MemLen; i++ )
		pMem[i] = bf.pMem[i];

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ( BitLen != bf.BitLen ) return 0;
	for ( int i = 0; i < MemLen - 1; i++ )	{
		if ( pMem[i] != bf.pMem[i] )
			return 0;
	}
	for ( int i = BitLen % ( sizeof(TELEM) * 8 ); i < BitLen; i++ )	{
		if( GetBit(i) != bf.GetBit(i) )	return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if ( BitLen != bf.BitLen ) return 1;
	for ( int i = 0; i < MemLen; i++ ) {
		if ( pMem[i] != bf.pMem[i] ) return 1;
	}
	for ( int i = BitLen % ( sizeof(TELEM) * 8 ); i < BitLen; i++ ) {
		if ( GetBit(i) != bf.GetBit(i) ) return 1;
	}
	
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField tbf(1);

	if ( BitLen > bf.BitLen )	{
		tbf = *this;
		for ( int i = 0; i < bf.MemLen; i++ ) tbf.pMem[i] |= bf.pMem[i];
	}	else if ( BitLen <= bf.BitLen )	{
		tbf = bf;
		for ( int i = 0; i < MemLen; i++ ) tbf.pMem[i] |= pMem[i];
	}
	
	return tbf;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField tbf(1);

	if ( BitLen > bf.BitLen ) {
		tbf = *this;
		for ( int i = 0; i < bf.MemLen; i++ ) tbf.pMem[i] &= bf.pMem[i];
		for ( int i = bf.MemLen; i < MemLen; i++ ) tbf.pMem[i] = 0;
	}	else if ( BitLen <= bf.BitLen ) {
		tbf = bf;
		for ( int i = 0; i < MemLen; i++ ) tbf.pMem[i] &= pMem[i];
		for ( int i = MemLen; i < bf.MemLen; i++ ) tbf.pMem[i] = 0;
	}

	return tbf;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tbf(BitLen);
	int BitInByte = sizeof(TELEM) * 8,
		mask = INT_MAX >> ( BitInByte - ( BitLen % BitInByte + 1 ) );

	for ( int i = 0; i < MemLen - 1; i++) tbf.pMem[i] = ~pMem[i];
	tbf.pMem[MemLen - 1] = mask & ~pMem[MemLen - 1];

	return tbf;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char sym;
	istr >> sym;
	while ( sym == '0' || sym == '1' ) {
		if ( sym == '0' )
			bf.ClrBit(i);
		else bf.SetBit(i);
		i++;
		istr >> sym;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for ( int i = 0; i < bf.GetLength(); i++ )	{
		if ( bf.GetBit(i) )	ostr << 1;
		else	ostr << 0;
	}
	return ostr;
}
