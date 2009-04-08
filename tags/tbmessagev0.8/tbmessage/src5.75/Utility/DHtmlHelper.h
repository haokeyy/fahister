#pragma once
#include "Mshtml.h"

HRESULT GetDocumentBody(IHTMLDocument2 *pDoc, IHTMLElement** pBody);

HRESULT GetElementById(IHTMLDocument2 *pDoc, CString szId, IHTMLElement** pElem);

BOOL GetControlBoolAttribute(IHTMLElement *pElem, CString szAttributeName);

BOOL GetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName);

CString GetControlStringAttribute(IHTMLElement *pElem, CString szAttributeName);

CString GetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName);

void SetControlBoolAttribute(IHTMLElement *pElem, CString szAttributeName, BOOL bValue);

void SetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, BOOL bValue);

void SetControlStringAttribute(IHTMLElement *pElem, CString szAttributeName, CString szValue);

void SetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, CString szValue);