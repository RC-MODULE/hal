# Hardware abstraction Level 

## ����������
���������� ������������ ���������-��������� ����� NeuroMatrix ����������� � �������������� �������.  
����������� ��������� � ��������� ����������� ����c-������������� ����������. 

���������� ������������:
* �������� ����������� ������ , ����� �������, ������������� ����� ������ � NMC ������ (�������� ����������� ��� ����������� �������� � ������ - ����)
* �������� ������ � ������ 
* ������ �� ������������, ����������, DMA � ������������ 
* ������������ ����� printf �� �������
 
�������������� ������: ��51.03 , M�76.01, ��77.07, ��12101


# ��������� HAL 
## ��������� ����������
* NeuroMatrix NMGCC-SDK  
  ��� ������ ��������� ��� NeuroMatrix  gcc ������������ 

* Leqacy NeuroMatrix SDK  (���������� ������ SDK)
  ��� ������ ��������� ��� NeuroMatrix ���������� ������������ ���������  NMSDK ������� �� ���� 3.07

* x86/x64 SDK   
  ��� �������� NeuroMatrix ������� �� ������� NMPP ��� x86/x64 �������� ������ ��������� � �������   Gnu GCC.  http://www.mingw.org/  ��� http://win-builds.org/doku.php ��� Microsoft Visual Studio, � � ��������� �������� Express:  
[Visual Studio 2005 Express](http://apdubey.blogspot.ru/2009/04/microsoft-visual-studio-2005-express.html)  
[Visual Studio Express](https://visualstudio.microsoft.com/ru/vs/older-downloads/)  

*  ������ ��������� (����������� lib-����������, �����, �������) ��������� �� Makefile ��������. ��� ����������� ������� ��������� Makefile-��  ��� Windows ��������� ������������� GNU **make**.
��� �� Windows ������������� ������ make [3.81](https://sourceforge.net/projects/gnuwin32/files/make/3.81/)  (3.82- ��������� ���� �����������). 

* ��������� ��������� �������� x86/x64 ���������/������, � �.�. ��� VisualStudio, �������������� [premake5](https://premake.github.io/)  ������� �� ���� premake-5.0.0-alpha14

* ��� ����������� ������ ���������������� Makefile-�� (������/��������/��������� �������� � .�.�.), � ����� �����-������������� ������ Makefile ��� � �� Windows, ��� � � Linux  ������������ :    UNIX-�������:   
[rm,cp,mkdir,rmdir,...](http://gnuwin32.sourceforge.net/packages/coreutils.htm )  
[find](http://gnuwin32.sourceforge.net/packages/findutils.htm ) (������� ������������� � gfind.exe ��� ���������� ���������
 � ��������� Windows\System32\find.exe)  

* ���� � premake � GnuWin ��������� ������ ���� ��������� � PATH 



## ������ NeuroMatrix ���������  NMC-GCC  ������������ 
  ������ �������������� �������� ```make nmcgcc``` �� ��������������� ����� ����� */make/hal_board*. 
  
```
hal> cd make-mc12101
hal/make-mc12101> make nmcgcc
```
## ������ NeuroMatrix ��������� Legacy  ������������ 
  ������ ��������� ������������ �������������� �������� ```make``` � ������ ```legacy``` �� ��������������� ����� ����� */make/hal_board*. 
  
```
hal> cd make-mc12101
hal/make-mc12101> make legacy
```

## ������ x86/x64 ���������  
  ��������� �������� ��������� �c������������ ���������� [**premake5**](https://premake.github.io/).  
  ���������������� ������ ��� ������ SDK � ������� ��� ����� ��������   
 
```\hal\make-mc12101> make vs2015 ```  

��� � ������� ������:  vs2008, vs20015, vs2017 , unix, mingw ...
����������� ��������� SDK   

> �� ��������� ������� 
> ```\hal\make-mc12101> make ```   �����������
> ```\hal\make-mc12101> make nmcgcc vs2015 ```  

## ��������� ���������� ���������  

��� �������� ����������� ��������� � ����������� ��������, � ����� �������� � ������  ������������� ������������ ���������� ��������� **HAL**. ������� ���������� **HAL** � ��������� �� ���� � ������������� ����� hal ����� � ������� �������  
```\hal> make setenv```


