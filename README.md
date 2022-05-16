# x8632-injector
Injector for windows API hooking on x86_32 by KITRI BoB 9th

## 코드 개요
syntax : Injector.exe [Program Path] [Dll Path]

1. createProcess 함수를 이용해 프로그램을 suspend 상태로 자동 실행시킨다.<br>
  1-1. 이때 CreateProcess 함수에서 쓰이는  PROCESS_INFORMATION pi구조체의 dwProcessId를 이용해 현재 실행시킨 프로세스의 PID를 직접 받아온다.
2. dll 인젝션을 한다.
3. suspend 상태를 해제한다.<br><br>

### Location of ConsoleApplication2.cpp
x8632-injector/ConsoleApplication2/ConsoleApplication2/ConsoleApplication2/ConsoleApplication2.cpp<br><br><br>

### If the number of input arguments is not correct.
<img width="482" alt="3" src="https://user-images.githubusercontent.com/40741363/90559761-079b6a00-e1d9-11ea-96ff-5b3402738ee4.PNG"><br><br><br>

### If you enter the command correctly.
<img width="482" alt="4" src="https://user-images.githubusercontent.com/40741363/90559763-08340080-e1d9-11ea-949f-edb81dae0975.PNG"><br><br><br>

###  Dll injection succeeded
Hooking Dll is attached to the process.<br><br><br>
<img width="959" alt="2" src="https://user-images.githubusercontent.com/40741363/90559757-05d1a680-e1d9-11ea-9c10-71414f424dda.PNG"><br><br><br>

### Dll Hooking succeeded.
When you try to save a file,Dll Hooking succeeded.<br><br><br>
<img width="239" alt="1" src="https://user-images.githubusercontent.com/40741363/90559754-05391000-e1d9-11ea-870b-b9ee6e4e1112.PNG"><br><br><br>

### Demonstration video
https://youtu.be/1PdKAeWjAnw
