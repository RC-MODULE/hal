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
*  ������ ��������� (����������� lib-����������, �����, �������) ��������� �� ��������� �������� Makefile. ��� ����������� ������� ��������� Makefile-��  ��� Windows ��������� ������������� GNU **make**.
��� �� Windows ������������� ������ make [3.81](https://sourceforge.net/projects/gnuwin32/files/make/3.81/)  (3.82- ��������� ���� �����������). make 3.81 - ������ � ������ [GnuWin32-lite](https://github.com/RC-MODULE/gnuwin32-lite) (��.����)

* ��� ����������� ������ ���������������� Makefile-�� (������/��������/��������� �������� � .�.�.), � ����� �����-������������� ������ Makefile ��� � �� Windows, ��� � � Linux  ������������ UNIX-������� (rm,cp,which,...).  � ����� �� Windows ���������� �� ������� �� ������ [GnuWin](http://gnuwin32.sourceforge.net/), � ���� ���� ��������� �� ��������������� ���������. 
����������� ����������� �������� ���� ����������� GnuWin ������ �������: make,cmake,gzip � ��. ����� ���������� � ������� online-����������� [GnuWin32-lite](https://github.com/RC-MODULE/gnuwin32-lite)

* NeuroMatrix NMGCC-SDK  
  ��� ������ ��������� gcc ������������ ��� NeuroMatrix 

* Leqacy NeuroMatrix SDK  (���������� ������ SDK)
  ��� ������ ��������� ��� NeuroMatrix ��������� NMSDK ������� �� ���� 3.07

* x86/x64 SDK   
  ��� ������ host-���������  �������� Gnu GCC ��� Microsoft Visual Studio.
  http://www.mingw.org/   
  http://win-builds.org/doku.php    
[Visual Studio 2005 Express](http://apdubey.blogspot.ru/2009/04/microsoft-visual-studio-2005-express.html)  
[Visual Studio Express](https://visualstudio.microsoft.com/ru/vs/older-downloads/)  



## ������ NeuroMatrix ���������  GCC  ������������ 
  ������ �������������� �������� ```make``` �� ��������������� ����� ����� */make/nmpp_<board>*. 
  
```
nmpp> cd make/nmpp-mc12101
nmpp/make/nmpp-nmc3> make nmc
nmpp/make/nmpp-nmc3> make DEBUG=y
```
## ������ NeuroMatrix ��������� Legacy  ������������ 
  ������ ��������� ������������ �������������� �������� ```make``` � ������ ```legacy``` �� ��������������� ����������� ����� */make/nmpp_archictecture*. 
  
```
nmpp> cd make/nmpp-nmc3
nmpp/make/nmpp-nmc3> make legacy
nmpp/make/nmpp-nmc3> make legacy DEBUG=y
```

## ������ x86/x64 ���������  
  ��������� ����� �������� �c������������ ���������� [**premake5**](https://premake.github.io/).  
 ���������������� ������ ��� ������ SDK � ������� ��� ����� ��������   
 
```\nmpp\make\nmpp-x86-x64> make vs2005 ```  
```\nmpp\make\nmpp-x86-x64> make vs2015 ```  
```\nmpp\make\nmpp-x86-x64> make vs2017 ```  
��� � ������� ������:  vs2008, vs20015, vs2017 , unix, mingw ...
����������� ��������� SDK   

## ��������� ���������� ���������  
��� �������� ����������� ��������� � ����������� ��������, � ����� �������� � ������  ������������� ������������ ���������� ��������� **NMPP**. ������� ���������� **NMPP** � ��������� �� ���� � ������������� ����� NMPP ����� � ������� �������  
```\nmpp\make\> make setenv```


