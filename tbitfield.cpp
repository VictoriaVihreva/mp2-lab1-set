// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
unsigned int bitInElem = sizeof(TELEM) * 8;
int shiftSize = bit_width(bitInElem - 1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw out_of_range("len can't be negative");
    BitLen = len;
    MemLen = (len-1) / bitInElem + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n >> shiftSize);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (1 << (n & (bitInElem - 1)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0)
        throw out_of_range("number of bit can't be negative");
    if (n >= BitLen)
        throw out_of_range("n is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0)
        throw out_of_range("number of bit can't be negative");
    if (n >= BitLen)
        throw out_of_range("n is out of range");
    pMem[GetMemIndex(n)] &= (~(GetMemMask(n)));;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0)
        throw out_of_range("number of bit can't be negative");
    if (n >= BitLen)
        throw out_of_range("n is out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[]pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField tmp(max(BitLen, bf.BitLen));
    for (int i = 0; i < tmp.MemLen; i++)
        tmp.pMem[i] = pMem[i] | bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField tmp(max(BitLen, bf.BitLen));
    for (int i = 0; i < tmp.MemLen; i++)
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp = (*this);
    for (int i = 0; i < tmp.BitLen; i++)
    {
        if (tmp.GetBit(i) == 0) 
            tmp.SetBit(i);
        else tmp.ClrBit(i);
    }
    return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    istr >> bf.BitLen;
    TELEM tel;
    TBitField tmp(bf.BitLen);
    for (int i = 0; i < tmp.BitLen; i++)
    {
        istr >> tel;
        if (tel == 1)
            tmp.SetBit(i);
    }
    bf = tmp;
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int len = bf.BitLen;
    for (int i = 0; i < len; i++)
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    return ostr;
}