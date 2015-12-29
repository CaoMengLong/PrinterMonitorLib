#include <iostream>
#include <string>
#include "windows.h"
#include "winspool.h"
#include <stdlib.h>
#include <cstring>
using namespace std;

int print_jobs =0; //当前打印机任务数量

struct JOBINFO {
	int JobId; //任务编号
	int Status; //任务状态
	int TotalPages; //任务打印页数
	char* pDocument; //任务打印文档名称
};

JOBINFO* JOBINFOARRAY;

wchar_t* CharToWchar(const char* c)  
{  
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);  
	m_wchar=new wchar_t[len+1];  
	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);  
	m_wchar[len]='\0';  
	return m_wchar;  
}  
char* WcharToChar(const wchar_t* wp)  
{  
	char *m_char;
	int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL);  
	m_char=new char[len+1];  
	WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL);  
	m_char[len]='\0';  
	return m_char;  
}  

//获取打印机JOBS详细信息 返回打印机当前任务数量。
extern "C" __declspec(dllexport) int GetJobs(LPSTR printNamestr){
	HANDLE hPrinter;  
	LPSTR printName = printNamestr;
	DWORD dwNeeded, dwReturne;
	JOB_INFO_2 * pJobInfo = NULL;

	//打开打印机
	if(!OpenPrinter(printName,&hPrinter,NULL)){		
		//result ="{\"code\":-1,\"msg\":\"打印机打开失败\"}";
		return -1;
	}
	//获得需要的内存数量
	if(!EnumJobs(hPrinter, 0, 127, 2, NULL, 0, &dwNeeded, &dwReturne))
	{
		if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
		{
			//result ="{\"code\":-2,\"msg\":\"打印机打印任务查询失败\"}";
			ClosePrinter( hPrinter );
			return -2;
		}
	}
	//分配内存
	pJobInfo = (JOB_INFO_2*)GlobalAlloc(GPTR, dwNeeded);
	if(!pJobInfo)
	{
		//分配内存失败
		ClosePrinter( hPrinter );
		//result ="{\"code\":-3,\"msg\":\"分配内存失败\"}";
		return -3;
	}

	//获得JOB_INFO_2的数组
	if(!EnumJobs( hPrinter, 0, 127, 2, (LPBYTE)pJobInfo, dwNeeded, &dwNeeded, &dwReturne ))
	{
		ClosePrinter( hPrinter );		
		GlobalFree((HGLOBAL)pJobInfo);
		//result ="{\"code\":-4,\"msg\":\"获得JOB_INFO_2的数组失败!\"}";
		return -4;

	} 

	//关闭打印机
	ClosePrinter(hPrinter); 

	if(dwReturne>0){
		print_jobs=dwReturne;

		JOBINFO *jobinfoarray=new JOBINFO[2];
		for(int j = 0 ; j< dwReturne; j++)
		{
			jobinfoarray[j].JobId=pJobInfo[j].JobId;
			jobinfoarray[j].Status=pJobInfo[j].Status;
			jobinfoarray[j].TotalPages=pJobInfo[j].TotalPages;
			//jobinfoarray[j].pDocument=CharToWchar(pJobInfo[j].pDocument);
			jobinfoarray[j].pDocument=pJobInfo[j].pDocument;
			//cout << "pDocument " << WcharToChar(jobinfoarray[j].pDocument) << endl
			cout << "pDocument " << jobinfoarray[j].pDocument << endl;
		}

		JOBINFOARRAY=jobinfoarray;

	}
	return dwReturne;

} 

//获取当前打印任务详细情况
extern "C" __declspec(dllexport) void GetJobInfo(JOBINFO *refJOBINFROArray[],int length){

	for(int j = 0 ; j< length; j++){
		refJOBINFROArray[j]->JobId = JOBINFOARRAY[j].JobId;
		refJOBINFROArray[j]->Status = JOBINFOARRAY[j].Status;
		refJOBINFROArray[j]->TotalPages = JOBINFOARRAY[j].TotalPages;
		refJOBINFROArray[j]->pDocument =JOBINFOARRAY[j].pDocument;
		cout << "GetJobInfo JobId " <<JOBINFOARRAY[j].pDocument << endl;
	}

}

