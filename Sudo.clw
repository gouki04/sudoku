; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Sudo.h"
LastPage=0

ClassCount=4
Class1=CSudoApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class2=CGameView
Resource2=IDR_MAINFRAME
Resource3=IDR_ACCELERATOR1

[CLS:CSudoApp]
Type=0
HeaderFile=Sudo.h
ImplementationFile=Sudo.cpp
Filter=N

[CLS:CGameView]
Type=0
HeaderFile=GameView.h
ImplementationFile=GameView.cpp
Filter=W
BaseClass=CWnd 
VirtualFilter=WC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_SUDOGUIZE




[CLS:CAboutDlg]
Type=0
HeaderFile=Sudo.cpp
ImplementationFile=Sudo.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342177294
Control5=IDC_TEXT,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_START
Command2=ID_RETURN
Command3=ID_APP_EXIT
Command4=ID_EASY
Command5=ID_MIDDLE
Command6=ID_HARD
Command7=ID_APP_ABOUT
Command8=ID_SUDOGUIZE
CommandCount=8

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=ID_RETURN
CommandCount=1

