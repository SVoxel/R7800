//File: bitstream.cpp
//The implementation of the CBitStream class
#include <stdio.h>
#include "bitstream.h"

CBitStream::CBitStream(BYTE* byData, UINT nSize)
{
	m_pBuf = NULL;
	m_nSize = m_dwIndex = 0;
	Create(byData, nSize);
}

CBitStream::CBitStream()
{
	m_pBuf = NULL;
	m_nSize = m_dwIndex = 0;
	m_pBuf = NULL;
}

CBitStream::~CBitStream()
{
	if(m_pBuf)
		delete[] m_pBuf;
}

BOOL CBitStream::Create(BYTE* byData, UINT nBufLen)
{
	if(m_pBuf)
		delete[] m_pBuf;

	m_pBuf = new BYTE[nBufLen];

	if(m_pBuf)
	{
		memcpy(m_pBuf, byData, nBufLen);
		m_nSize = nBufLen;
		m_dwIndex = 0;
		return TRUE;
	}
	else
	{
		//CDebugger::Instance()->Out("[CBitStream]Error. Not enough memory\xd\xa");
		return FALSE;
	}
}

BOOL CBitStream::InitBuffer(UINT nBufLen)
{
	if(nBufLen > m_nSize)
	{
		if(m_pBuf)
			delete[] m_pBuf;
		m_pBuf = new BYTE[nBufLen];

		if(m_pBuf)
		{
			memset(m_pBuf, 0, nBufLen);
			m_nSize = nBufLen;
			m_dwIndex = 0;
			return TRUE;
		}
		else 
			return FALSE;
	}
	else
	{
		m_nSize = nBufLen;
		return TRUE;
	}
}

BOOL CBitStream::Seek(DWORD dwIndex)
{
	if(dwIndex > m_nSize * BITSOFBYTE - 1)
		return FALSE;

	if(dwIndex == Begin)
		dwIndex = 0;
	else if(dwIndex == End)
		dwIndex = m_nSize * BITSOFBYTE - 1;
	else
		dwIndex = dwIndex;
	return TRUE;
}

int CBitStream::Read(BYTE* pBuf, UINT nBitCount)
{
	int iRemainIndex = m_dwIndex % BITSOFBYTE;
	int iBufIndicator = m_dwIndex / BITSOFBYTE;
	int iByteCount = (nBitCount - 1) / BITSOFBYTE + 1;
        int i;

	for(i = 0; i < iByteCount && iBufIndicator < m_nSize - 1; i++, iBufIndicator++)
	{
		pBuf[i] = (m_pBuf[iBufIndicator] << iRemainIndex) | 
			(m_pBuf[iBufIndicator + 1] >> (BITSOFBYTE - iRemainIndex));
	}
	if(i == iByteCount)
	{
		m_dwIndex += nBitCount;
		return nBitCount;
	}
	else
	{
		pBuf[i] = m_pBuf[iBufIndicator] << iRemainIndex;

		if(nBitCount <= BITSOFBYTE - iRemainIndex)	//Enough bits to read
		{
			m_dwIndex += nBitCount;
			return nBitCount;
		}
		else
		{
			m_dwIndex += BITSOFBYTE * (i + 1) - iRemainIndex;
			return BITSOFBYTE * (i + 1) - iRemainIndex;
		}
	}
}

int CBitStream::Write(BYTE *pBuf, UINT nBitCount)
{
	int iRemainIndex = m_dwIndex % BITSOFBYTE;
	int iBufIndicator = m_dwIndex / BITSOFBYTE;
	int iByteCount = (nBitCount - 1) / BITSOFBYTE + 1;
	int i;

	printf("Byte:%c index:%d m_pBuf:%s ", *pBuf, m_dwIndex, m_pBuf);
	for(i = 0; i < iByteCount && iBufIndicator < m_nSize - 1; i++, iBufIndicator++)
	{
		m_pBuf[iBufIndicator] |= pBuf[i] >> iRemainIndex;
		m_pBuf[iBufIndicator + 1] |= pBuf[i] << (BITSOFBYTE - iRemainIndex);
	}
	if(i == iByteCount)
	{
		m_dwIndex += nBitCount;
		return nBitCount;
	}
	else
	{
		m_pBuf[iBufIndicator] |= pBuf[i] >> iRemainIndex;

		if(nBitCount <= BITSOFBYTE - iRemainIndex)	//Enough bits to read
		{
			m_dwIndex += nBitCount;
			return nBitCount;
		}
		else
		{
			m_dwIndex += BITSOFBYTE * (i + 1) - iRemainIndex;
			return BITSOFBYTE * (i + 1) - iRemainIndex;
		}
	}
}

int CBitStream::CopyBuffer(BYTE* pBuf, UINT nBufLen)
{
	printf("Bitstream %s.\n", m_pBuf);
	if(nBufLen > m_nSize)
	{
		memcpy(pBuf, m_pBuf, m_nSize);
		return m_nSize;
	}
	else
	{
		memcpy(pBuf, m_pBuf, nBufLen);
		return nBufLen;
	}
}
