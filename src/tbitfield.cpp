// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw exception("Incorrect size of tbitfield!");
    this->BitLen = len;
    this->MemLen = (len + SizeTelem - 1) / SizeTelem;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < MemLen; i++) this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] this->pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw std::out_of_range("Index out of range!");
    return n / SizeTelem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) throw std::out_of_range("Index out of range!");
    TELEM tmp = 0;
    int pos = n % SizeTelem;
    TELEM one = 1;
    tmp = one << pos;
    return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw std::out_of_range("Index out of range!");
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[ind] |= mask;

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw std::out_of_range("Index out of range!");
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[ind] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw std::out_of_range("Index out of range!");
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return ((pMem[ind] & mask)!=0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf == this) return *this;
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    delete[] pMem;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < this->MemLen; i++) this->pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->BitLen != bf.BitLen) return 0;
    for (int i = 0; i < this->MemLen; i++) if (this->pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    bool flag = 1;
    int maxlen = this->BitLen;
    int minlen = this->BitLen;
    if (this->BitLen<bf.BitLen) maxlen = bf.BitLen;
    else {
        flag = 0;
        minlen = bf.BitLen;
    }
    TBitField res(maxlen);
    for (int i = 0; i < minlen; i++) {
        if (this->GetBit(i) || bf.GetBit(i)) res.SetBit(i);
    }
    for (int i = minlen; i < res.BitLen; i++) {
        if (flag == 0 && this->GetBit(i)) res.SetBit(i);
        else if (flag == 1 && bf.GetBit(i)) res.SetBit(i);
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxlen = std::max(this->BitLen, bf.BitLen);
    int minlen = std::min(this->BitLen, bf.BitLen);
    TBitField res(maxlen);
    for (int i = 0; i < res.MemLen; i++) res.pMem[i] = this->pMem[i] & bf.pMem[i];
    for (int i = minlen; i < res.BitLen; i++) res.ClrBit(i);
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < res.BitLen; i++) {
        if (this->GetBit(i) == 0) res.SetBit(i);
        else res.ClrBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        bool bit = 0;
        istr >> bit;
        if (bit == 1) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) cout << bf.GetBit(i);
    return ostr;
}
