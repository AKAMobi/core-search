// ATLSearch.h : CATLSearch 的声明

#pragma once
#include "resource.h"       // 主符号


// IATLSearch
[
	object,
	uuid("A5003C20-C0B8-437E-A1A1-13BD7716E168"),
	dual,	helpstring("IATLSearch 接口"),
	pointer_default(unique)
]
__interface IATLSearch : IDispatch
{
	[id(1), helpstring("方法asp_init_query")] HRESULT asp_init_query([in] LONG bk, [in] BSTR dd, LONG bigmem, [out,retval] LONG* ret);
	[id(2), helpstring("方法asp_done_query")] HRESULT asp_done_query([out,retval] LONG* ret);
	[id(3), helpstring("方法asp_highlight")] HRESULT asp_highlight([in] BSTR fn, [in] BSTR qn, [in] BSTR preTag, [in] BSTR postTag, [out,retval] BSTR* ret);
	[id(4), helpstring("方法asp_fetch_total")] HRESULT asp_fetch_total([out,retval] LONG* ret);
	[id(5), helpstring("方法asp_do_query")] HRESULT asp_do_query([in] LONG i, [in] LONG j, [in] BSTR q, [out,retval] VARIANT* ret);
	[id(6), helpstring("方法asp_highlight_content")] HRESULT asp_highlight_content([in] BSTR content, [in] LONG len, [in] BSTR qn, [in] BSTR preTag, [in] BSTR postTag, [out,retval] BSTR* ret);
};



// CATLSearch

[
	coclass,
	threading("apartment"),
	vi_progid("wASPExt.ATLSearch"),
	progid("wASPExt.ATLSearch.1"),
	version(1.0),
	uuid("8FF7A0F2-DA87-4231-972F-B7776E5A7E58"),
	helpstring("ATLSearch Class")
]
class ATL_NO_VTABLE CATLSearch : 
	public IATLSearch
{
public:
	CATLSearch()
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(asp_init_query)(LONG bk, BSTR dd, LONG bigmem, LONG* ret);
	STDMETHOD(asp_done_query)(LONG* ret);
	STDMETHOD(asp_highlight)(BSTR fn, BSTR qn, BSTR preTag, BSTR postTag, BSTR* ret);
	STDMETHOD(asp_fetch_total)(LONG* ret);
	STDMETHOD(asp_do_query)(LONG i, LONG j, BSTR q, VARIANT* ret);
	STDMETHOD(asp_highlight_content)(BSTR content, LONG len, BSTR qn, BSTR preTag, BSTR postTag, BSTR* ret);
};

