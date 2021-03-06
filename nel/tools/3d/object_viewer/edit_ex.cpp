// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// edit_ex.cpp : implementation file
//

#include "std_afx.h"
#include "edit_ex.h"

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx() : _Listener(NULL), _Type(StringType)
{
}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	PostMessage(EM_SETSEL, 0, -1);	
	Invalidate();	
}

sint		CEditEx::getSInt() const
{
	nlassert(_Type == SIntType);
	sint ret = 0;
	NLMISC::fromString(getString(), ret);
	return ret;
}

uint		CEditEx::getUInt() const
{
	nlassert(_Type == UIntType);
	uint ret = 0;
	NLMISC::fromString(getString(), ret);
	return ret;
}
float		CEditEx::getFloat() const
{
	nlassert(_Type == FloatType);
	return (float) ::atof(getString().c_str());
}

std::string CEditEx::getString() const
{
	char buf[128];
	GetWindowText(buf, sizeof(buf));
	return std::string(buf);
}

void		CEditEx::setSInt(sint value)
{
	nlassert(_Type == SIntType);
	char buf[16];
	sprintf(buf, "%d", (int) value);
	setString(buf);
}

void		CEditEx::setUInt(uint value)
{
	nlassert(_Type == UIntType);
	char buf[16];
	sprintf(buf, "%d", (int) value);
	setString(buf);
}

void		CEditEx::setFloat(float value)
{
	nlassert(_Type == FloatType);
	char buf[16];
	sprintf(buf, "%g", (double) value);
	setString(buf);
}

void CEditEx::setString(const char *value)
{
	SetWindowText(value);
}

void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == 13) // return pressed ?
	{
		if (isValid())
		{
			if (_Listener)
			{
				_Listener->editExValueChanged(this);
			}
		}
		else
		{
			MessageBox("Invalid value", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
	}	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


bool CEditEx::isValid()
{
	int iValue;
	float fValue;
	switch(_Type)
	{
		case SIntType:
			return sscanf(getString().c_str(), "%d", &iValue) == 1;
		break;
		case UIntType:
			return sscanf(getString().c_str(), "%d", &iValue) == 1 && iValue >= 0;
		break;
		case FloatType:
			return sscanf(getString().c_str(), "%f", &fValue) == 1;
		break;
		default:
			return true;	
		break;
	}
}
