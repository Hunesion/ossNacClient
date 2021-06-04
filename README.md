# ossNacClient
이  프로젝트는  개방형OS 기반의 네트워크접근통제  클라이언트를  개발합니다.
### 개발대상 개방형OS
* 구름OS 
* 티맥스OS
* 하모니카OS

망간자료전송  솔루션은  분리된  망(일반적으로 인터넷이 되지 않는 업무망과 인터넷망) 간에  안전하게  파일을  송수신하는  기능을  기본으로  제공하며, 클립보드  연계  및  URL보내기(업무망->인터넷망)  등을  부가적인  편리기능으로  제공합니다.
기존  망간자료전송  솔루션의  클라이언트  프로그램이  윈도우OS 위주로  지원되고  있어  개방형OS를  도입하기  위해서는  탑재해야  하는  필수  보안프로그램  중  하나인  망간자료전송  클라이언트의  개발과  지원이  꼭  필요합니다.

# **주요 개발항목**
* UI Resource Template
* Main Application UI
* 파일  보내기
* Drag & Drop
* 파일  받기(UI)
* 파일  받기(자동)
* 다운로드  폴더열기
* 클립보드  수신
* 클립보드  송신
* 클립보드  필터링
* 클립보드  단축키
* 클립보드  파일 Read/Write
* URL 수신
* URL 파일 Read
* 브라우저  실행
* IP/MAC Check
* 메시지  알림
* WebView
* IPC 통신(Main <-> WebView)



*목표기능은 OS와 GTK 라이브러리의 지원여부에 따라 수정 또는 삭제될 수 있습니다.*

## **시작하기**
이  지침에  따라  개발  프로젝트에  참여할  수  있습니다.

### **빌드하기**
ossFileTransferClient를 빌드하려면 [WIKI:Build](https://github.com/HuneOpenUp/ossFileTransferClient/wiki/Build)를 참조하십시오.

## **전제조건**
이  프로젝트는  각각의  개방형OS를  공통적으로  지원하기  위해 GTK+ 3.0 기반으로  개발할  예정입니다.

기존  솔루션에  개방형OS 클라이언트를  추가하는  것이므로  윈도우용  클라이언트에서  지원하는  기능을  전환하여  개발합니다. 
다만, 기존  윈도우용  클라이언트의 UI 대신에  새로운 UI로  개발하기  위해  신규UI와  이에  따른  설계를  진행하고, 
기본적인 UI가  개발되는  시점부터  이를  기반으로  각각의  기능  모듈  단위로  소스를  공개합니다.

## **기여**
[CONTRIBUTION.md](https://github.com/HuneOpenUp/ossFileTransferClient/blob/master/CONTRIBUTING.md) 파일을  읽어  주십시오. 당사의  행동강령과  요청접수  절차에  대한  자세한  내용을  확인하실  수  있습니다.


## **라이선스**
이  프로젝트는 LGPL 2.1로  라이선스가  부여되었습니다. 자세한  내용은 [LICENSE](https://github.com/HuneOpenUp/ossFileTransferClient/blob/master/LICENSE) 파일을  참조하십시오.
