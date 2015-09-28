// ATLSearch.cpp : CATLSearch 的实现

#include "stdafx.h"
#include "ATLSearch.h"
#include ".\atlsearch.h"
#include "wQuery.h"


// CATLSearch


STDMETHODIMP CATLSearch::asp_init_query(LONG bk, BSTR dd, LONG bigmem, LONG* ret)
{
	CW2CT s(dd);

	*ret = init_query(bk,s,bigmem);

	return S_OK;
}

STDMETHODIMP CATLSearch::asp_done_query(LONG* ret)
{
	*ret = done_query();

	return S_OK;
}

STDMETHODIMP CATLSearch::asp_highlight(BSTR fn, BSTR qn, BSTR preTag, BSTR postTag, BSTR* ret)
{
	CW2CT s1(fn),s2(qn),s3(preTag),s4(postTag);
	char * s;
	s = highlight(s1,s2,s3,s4);
	CComBSTR s5(s);
	*ret = s5.Copy();

	return S_OK;
}

STDMETHODIMP CATLSearch::asp_highlight_content(BSTR content, LONG len, BSTR qn, BSTR preTag, BSTR postTag, BSTR* ret)
{
	CW2CT s1(content),s2(qn),s3(preTag),s4(postTag);
	char * s;
	s = highlight_content(s1,len,s2,s3,s4);
	CComBSTR s5(s);
	*ret = s5.Copy();

	return S_OK;
}

STDMETHODIMP CATLSearch::asp_fetch_total(LONG* ret)
{
	*ret = fetch_total();

	return S_OK;
}

STDMETHODIMP CATLSearch::asp_do_query(LONG i, LONG j, BSTR q, VARIANT* ret)
{
	int total,i0;
	CW2CT s(q);
	total = do_query(i,j,s);

	VariantInit(ret);
	ret->vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAY * psa;
	SAFEARRAYBOUND bounds[2];
	bounds[0].cElements = total;
	bounds[0].lLbound = 0;
	bounds[1].cElements = 3;
	bounds[1].lLbound = 0;
	psa = ::SafeArrayCreate(VT_VARIANT, 2, bounds);
	VARIANT ** VarArray;
	::SafeArrayAccessData(psa, (void**) &VarArray);
	for (i0=0;i0<total;i0++) {
		char s1[200],s2[200],s3[200];
		wfetch(s1,s2,s3);
		CComBSTR t1(s1),t2(s2),t3(s3);
		VarArray[i0][0].vt = VT_BSTR;
		VarArray[i0][0].bstrVal = t1.Copy();
		VarArray[i0][1].vt = VT_BSTR;
		VarArray[i0][1].bstrVal = t2.Copy();
		VarArray[i0][2].vt = VT_BSTR;
		VarArray[i0][2].bstrVal = t3.Copy();
	}
	SafeArrayUnaccessData(psa);
	ret->parray = psa;

	return S_OK;
}
