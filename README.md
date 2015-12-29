# PrinterMonitorLib
这是监控打印机状态的C++ DLL项目，可以非常方便的查询到当前打印机正在打印的文件状态，可以用于监控文档是否打印成功，打印机是否缺纸，打印机是否异常等状态。
使用C++编写的WIN32项目 DLL文件，项目编译后会生成 PrinterMonitorLib.dll 文件，可供.NET 使用。


<br/>PrinterMonitorLib 提供了2个方法：<br/>

 int GetJobs(LPSTR printNamestr);  //传入打印机名称，返回当前打印任务数量。<br/>
 
 void GetJobInfo(JOBINFO *refJOBINFROArray[],int length); //引用传入打印任务结构体数组、长度。打印任务详情将会赋值到传入的结构中。
 


<a href="https://github.com/CaoMengLong/PrinterMonitorLib/wiki/%E5%A6%82%E4%BD%95%E5%9C%A8C%23%E9%A1%B9%E7%9B%AE%E4%B8%AD%E4%BD%BF%E7%94%A8-PrinterMonitorLib">如何在C#项目中使用 PrinterMonitorLib</a>

