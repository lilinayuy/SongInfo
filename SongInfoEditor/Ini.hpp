/*
注册表数据格式
[ITS]
key=data,comment
*/

#pragma once

#include <strsafe.h>

#define MAX_INI_KEY 250
#define MAX_INI_VALUE 128
class CIni
{
public:
	CIni(void) {}
	~CIni(void) {}

	CString m_strIniFileName;
	CString m_strIniFilePath;
	//AutoIniFileName	使用和exe相同名字的配置文件
	void AutoIniFileName(void)
	{
		CString strTemp = AfxGetApp()->m_pszHelpFilePath;
		m_strIniFilePath = strTemp.Left(strTemp.ReverseFind('\\') + 1);
		m_strIniFileName = strTemp.Left(strTemp.ReverseFind('.'))+_T(".INI");
		return;
	}
	//AssignIniFileName	设置指定文件名的配置文件
	void AssignIniFileName(LPCTSTR filename)
	{
		m_strIniFileName = filename;
		return;
	}

	CString GetAppPath(void){
		return m_strIniFilePath;
	}

	TCHAR m_strValue[MAX_INI_VALUE];
	CString m_strKey;
	CString m_strData;
	CString m_strComm;
	//读取或更新键值 app 分类	key 键值	data 设置的值	datadefault 默认键值	返回值 读取到的键值
	//读取时data设为_T("")空值	写入时不需要默认键值和返回值
	//写入字符串
	CString UpdateKey(CString app, CString key, CString data, CString datadefault=_T(""))
	{
		// read value of key
		memset(m_strValue,0,MAX_INI_VALUE);
		::GetPrivateProfileString(app, key, _T(""), m_strValue, MAX_INI_VALUE, m_strIniFileName);
		// get data and comment from value
		if (m_strValue[0] == 0)
		{
			// key not exist
			m_strData = datadefault;
			m_strComm = _T("未知用途");
		}
		else
		{
			int comma = CString(m_strValue).Find(_T(','));
			m_strValue[comma] = 0;
			m_strData = m_strValue;
			m_strComm = m_strValue+comma+1;
			if (data != _T("")) m_strData = data;
		}
		// write value of key
		StringCchPrintf(m_strValue, MAX_INI_VALUE-1, _T("%s,%s"), m_strData, m_strComm);
		::WritePrivateProfileString(app, key, m_strValue, m_strIniFileName);

		return m_strData; 
	}
	//读取或更新键值 app 分类	key 键值	data 设置的值	datadefault 默认键值	返回值 读取到的键值
	//读取时data设为_T("")空值	写入时不需要默认键值和返回值
	//写入整数
	CString UpdateKey(CString app, CString key, int data, CString datadefault=_T(""))
	{
		CString str;
		str.Format(_T("%d"),data);
		return UpdateKey(app, key, str, datadefault);
	}
};
