#include "StdAfx.h"
#include "DHtmlHelper.h"

HRESULT GetDocumentBody(IHTMLDocument2 *pDoc, IHTMLElement** pBody)
{
	HRESULT hresult = pDoc->get_body(pBody);

    return hresult;
}

HRESULT GetElementById(IHTMLDocument2 *pDoc, CString szId, IHTMLElement** pElem)
{
    HRESULT hr = S_OK;

    BSTR bstrID = szId.AllocSysString();
    VARIANT varName, varIndex;
    varName.vt = VT_BSTR;
    varName.bstrVal = bstrID;
    varIndex.vt = VT_I4;
    varIndex.intVal = 0;        

    IHTMLElementCollection *pColl;
    hr = pDoc->get_all(&pColl);

    if (hr == S_OK && pColl)
    {
        IDispatch* pDisp;
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK && pDisp)
        {
            hr = pDisp->QueryInterface(IID_IHTMLElement, (void**)pElem);
        }
        else
        {
            *pElem = NULL;
        }
    }
    
    ::SysFreeString(bstrID);

    return hr;
}

BOOL GetControlBoolAttribute(IHTMLElement* pElem, CString szAttributeName)
{
    HRESULT hr = S_OK;
    if (pElem)
    {
        VARIANT varAttr;
        varAttr.vt = VT_BOOL;
        BSTR bstrAttributeName = szAttributeName.AllocSysString();
        hr = pElem->getAttribute(bstrAttributeName, 0, &varAttr);
        ::SysFreeString(bstrAttributeName);        
        
        if (hr == S_OK && varAttr.vt == VT_BOOL)
        {
            return varAttr.boolVal;
        }
    }

    return FALSE;
}

BOOL GetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
    {
        return GetControlBoolAttribute(pElem, szAttributeName);
    }

    return FALSE;
}

CString GetControlStringAttribute(IHTMLElement* pElem, CString szAttributeName)
{
    HRESULT hr = S_OK;;
    if (pElem)
    {
        VARIANT varAttr;
        varAttr.vt = VT_BSTR;
        BSTR bstrAttributeName = szAttributeName.AllocSysString();
        hr = pElem->getAttribute(bstrAttributeName, 0, &varAttr);
        ::SysFreeString(bstrAttributeName);

        if (hr == S_OK && varAttr.vt == VT_BSTR)
        {
            return CString(varAttr.bstrVal);
        }
    }

    return "";
}

CString GetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
    {
        return GetControlStringAttribute(pElem, szAttributeName);
    }

    return "";
}

void SetControlBoolAttribute(IHTMLElement* pElem, CString szAttributeName, BOOL bValue)
{
    HRESULT hr = S_OK;
    if (hr == S_OK && pElem)
    {
        BSTR bstrAttributeName = szAttributeName.AllocSysString();
        VARIANT varAttr;
        varAttr.vt = VT_BOOL;
        varAttr.boolVal = bValue;
        pElem->setAttribute(bstrAttributeName, varAttr);
        ::SysFreeString(bstrAttributeName);
    }
}

void SetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, BOOL bValue)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
    {
        SetControlBoolAttribute(pElem, szAttributeName, bValue);
    }
}

void SetControlStringAttribute(IHTMLElement* pElem, CString szAttributeName, CString szValue)
{    
    HRESULT hr = S_OK;
    if (hr == S_OK && pElem)
    {
        BSTR bstrAttributeName = szAttributeName.AllocSysString();
        BSTR bstrValue = szValue.AllocSysString();
        VARIANT varAttr;
        varAttr.vt = VT_BSTR;
        varAttr.bstrVal = bstrValue;
        pElem->setAttribute(bstrAttributeName, varAttr);
        ::SysFreeString(bstrAttributeName);
    }
}

void SetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, CString szValue)
{    
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
    {
        SetControlStringAttribute(pElem, szAttributeName, szValue);
    }
}
