#include "StdAfx.h"
#include "DHtmlHelper.h"


HRESULT GetElementById(IHTMLDocument2 *pDoc, CString szId, IHTMLElement** pElem)
{
    *pElem = NULL;
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
    }
    
    ::SysFreeString(bstrID);

    return hr;
}

HRESULT GetElementByName(IHTMLDocument2 *pDoc, CString szAttributeValue, IHTMLElement** pElem)
{
    return GetElementByAttribute(pDoc, "name", szAttributeValue, pElem);
}

HRESULT GetElementByAttribute(IHTMLDocument2 *pDoc, CString szAttributeName, CString szAttributeValue, IHTMLElement** pElem)
{
    IHTMLElementCollection *pColl;
    HRESULT hr = pDoc->get_all(&pColl);

    *pElem = NULL;
    long len;
    pColl->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        VARIANT varName, varIndex;
        varName.vt = VT_UINT;
        varName.lVal = i;
        varIndex.vt = VT_I4;
        varIndex.intVal = 0;

        IDispatch* pDisp; 
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK)
        {
            hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem);
            if (hr == S_OK)
            {                
                VARIANT varAttr;
                varAttr.vt = VT_BSTR;
                BSTR bstrAttributeName = szAttributeName.AllocSysString();
                hr = (*pElem)->getAttribute(bstrAttributeName, 0, &varAttr);
                ::SysFreeString(bstrAttributeName);

                if (hr == S_OK && varAttr.vt == VT_BSTR)
                {
                    CString strAttrValue(varAttr.bstrVal);
                    if (strAttrValue == szAttributeValue)
                    {
                        break;
                    }
                }
            }
            *pElem = NULL;
        }
    }

    return hr;
}


HRESULT GetElementByInnerHtml(IHTMLDocument2 *pDoc, CString szInnerHtml, IHTMLElement** pElem)
{
    IHTMLElementCollection *pColl;
    HRESULT hr = pDoc->get_all(&pColl);

    *pElem = NULL;
    long len;
    pColl->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        VARIANT varName, varIndex;
        varName.vt = VT_UINT;
        varName.lVal = i;
        varIndex.vt = VT_I4;
        varIndex.intVal = 0;

        IDispatch* pDisp; 
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK)
        {
            hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem);
            if (hr == S_OK)
            {
                BSTR tempInnerHTML;
                (*pElem)->get_innerHTML(&tempInnerHTML);

                if (szInnerHtml == tempInnerHTML)
                {
                    ::SysFreeString(tempInnerHTML);
                    break;
                }
                ::SysFreeString(tempInnerHTML);
            }
            *pElem = NULL;
        }
    }

    return hr;
}

HRESULT GetElementByOuterHtml(IHTMLDocument2 *pDoc, CString szOuterHtml, IHTMLElement** pElem)
{
    IHTMLElementCollection *pColl;
    HRESULT hr = pDoc->get_all(&pColl);

    *pElem = NULL;
    long len;
    pColl->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        VARIANT varName, varIndex;
        varName.vt = VT_UINT;
        varName.lVal = i;
        varIndex.vt = VT_I4;
        varIndex.intVal = 0;

        IDispatch* pDisp; 
        hr = pColl->item(varName, varIndex, &pDisp);
        if (hr == S_OK)
        {
            hr = pDisp->QueryInterface( IID_IHTMLElement, (void**)pElem);
            if (hr == S_OK)
            {
                BSTR tempOuterHTML;
                (*pElem)->get_outerHTML(&tempOuterHTML);

                if (szOuterHtml == tempOuterHTML)
                {
                    ::SysFreeString(tempOuterHTML);
                    break;
                }
                ::SysFreeString(tempOuterHTML);
            }
            *pElem = NULL;
        }
    }

    return hr;
}


CString GetControlValue(IHTMLDocument2 *pDoc, CString szId)
{
    return GetControlStringAttribute(pDoc, szId, "value");
}

BOOL GetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
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

CString GetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
    if (hr == S_OK && pElem)
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

void SetControlValue(IHTMLDocument2 *pDoc, CString szId, CString szValue)
{
    SetControlStringAttribute(pDoc, szId, "value", szValue);
}

void SetControlBoolAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, BOOL bValue)
{
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
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

void SetControlStringAttribute(IHTMLDocument2 *pDoc, CString szId, CString szAttributeName, CString szValue)
{    
    HRESULT hr = S_OK;
    IHTMLElement* pElem = NULL;
    hr = GetElementById(pDoc, szId, &pElem);
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