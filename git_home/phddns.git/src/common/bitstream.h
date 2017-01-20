//File: BitStream.h
//The header of the CBitStream class
#ifndef __BITSTREAM__H__
#define __BITSTREAM__H__
#include <string.h>
#include "global.h"


const UINT BITSOFBYTE = 8;

class CBitStream
{
//Constructors and destructors
public:
	CBitStream(BYTE* byData, UINT nBufLen);
	CBitStream();
	virtual ~CBitStream();
//Attributes
protected:
	BYTE*	m_pBuf;
	UINT	m_nSize;
	DWORD	m_dwIndex;
public:
	enum
	{
		Begin = 0,
		End = 0xffffffff,
	};
//Operations
public:
	virtual BOOL Create(BYTE* byData, UINT nBufLen);
	virtual BOOL InitBuffer(UINT nBufLen);
	virtual BOOL Seek(DWORD dwIndex);
	virtual int Read(BYTE* pBuf, UINT nBitCount);
	virtual int Write(BYTE* pBuf, UINT nBitCount);
	virtual int CopyBuffer(BYTE* pBuf, UINT nBufLen);
};

#endif
