#pragma once
#include "Mshtml.h"


HRESULT GetElementById(IHTMLDocument2 *pDoc, CString szId, IHTMLElement** pElem);

HRESULT GetElementByName(IHTMLDocument2 *pDoc, CString szAttributeValue, IHTMLElement** pElem);

HRESULT GetElementByAttribute(IHTMLDocument2 *pDoc, CString szAttributeName, CString szAttributeValue, IHTMLElement** pElem);

HRESULT GetElementByOuterHtml(IHTMLDocument2 *pDoc, CString szOuterHtml, IHTMLElement** pElem);

HRESULT GetElementByInnerHtml(IHTMLDocument2 *pDoc, CString szInnerHtml, IHTMLElement** pElem);

CString GetControlValue(IHTMLDocument2 *pDoc, CString szId);

BOOL GetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName);

CString GetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName);

void SetControlValue(IHTMLDocument2 *pDoc, CString szId, CString szValue);

void SetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, BOOL bValue);

void SetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, CString szValue);