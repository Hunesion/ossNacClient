# ossNacClient
이  프로젝트는  개방형OS 기반의 네트워크접근통제(NAC) 클라이언트를  개발합니다.

# 개발대상 개방형OS
* 구름OS
* 티맥스OS
* 하모니카OS

네트워크접근통제 솔루션은 사용자 단말의 안정성을 확보하여 내부 네트워크에 접근을 허용하는 제품입니다. 
기존  네트워크접근통제 솔루션의  클라이언트  프로그램이  윈도우OS 위주로  지원되고  있어  개방형OS를  도입하기  위해서는  탑재해야  하는  필수  보안프로그램  중  하나인  네트워크접근통제  클라이언트의  개발과  지원이  꼭  필요합니다.

# 네트워크접근통제(NAC) 필요성
* 네트워크 사용 인증강화 필요
   - 네트워크 사용 단말기의 증가
   - 기업 내부 네트워크 사용현황 관리 필요성 증대
   - 비인가 사용자/단말 관리 필요 

   ![image](https://user-images.githubusercontent.com/65996550/142129383-88e1f20f-1fb6-493d-a040-b2ec410ba5ba.png)

* 보안사고 대응 필요
   - 보안사고 발생에 대한 내부 네트워크 접근 통제 필요
   - 필수 보안 프로그램 설치 및 실행 점검
   - 네트워크 접근 단말의 무결성 점검

   ![image](https://user-images.githubusercontent.com/65996550/142129489-77093234-8cce-4943-b7c9-42b46dd30863.png)

* 컴플라이언스 준수 필요
   - 개인정보보호법, 정보통신망법
   - ISMS, ISMS-P, ISO27001 등 정보보안 관련 인증 획득/운영이 필요한 경우
   - 전자금융감독규정
   - 국가 정보보안 기본 지침 

# 네트워크접근통제(NAC) 개념도

   ![image](https://user-images.githubusercontent.com/65996550/142129628-33ef261a-cfa0-454e-8f16-b14bc744b279.png)

# 네트워크접근통제(NAC) 특장점

   ![image](https://user-images.githubusercontent.com/65996550/142129756-1ffbc614-9b2f-4cc3-90a6-25200815afd5.png)


# **주요 개발항목** 
* 로그인UI / 사용자로그인
* Updater / NAC 클라이언트 업데이트 체크, 패치파일 수신 및 설치
* 클라이언트 무결성 검사 / NAC 클라이언트 모듈별 무결성 검사
* 정책수신 / 보안정책 및 스크립트 파일 수신
* 정책조회 / 수신한 보안정책 및 사용자 정책 조회
* 검사결과 수신 / 보안검사 수행결과 수신
* 검사결과 조회 / 수신된 보안검사 결과 조회
* 실패항목처리안내 / 보안검사 실패항목별 처리방안 안내 
* 자동처리 요청 / 보안검사 실패항목별 자동처리 요청 전달
* WebView / 정보조회 화면을 WebView 윈도우로 표시

*목표기능은 OS와 GTK 라이브러리의 지원여부에 따라 수정 또는 삭제될 수 있습니다.*

## **시작하기**
이  지침에  따라  개발  프로젝트에  참여할  수  있습니다.

### **빌드하기**
ossNacClient를 빌드하려면 [WIKI:Build](https://github.com/Hunesion/ossNacClient/wiki/Build)를 참조하십시오.

## **전제조건**
이  프로젝트는  각각의  개방형OS를  공통적으로  지원하기  위해 GTK+ 3.0 기반으로  개발할  예정입니다.

기존  솔루션에  개방형OS 클라이언트를  추가하는  것이므로  윈도우용  클라이언트에서  지원하는  기능을  전환하여  개발하며 각각의  기능  모듈  단위로  소스를  공개합니다.

## **기여**
[CONTRIBUTION.md](https://github.com/Hunesion/ossNacClient/blob/master/CONTRIBUTING.md) 파일을  읽어  주십시오. 당사의  행동강령과  요청접수  절차에  대한  자세한  내용을  확인하실  수  있습니다.


## **라이선스**
이  프로젝트는 LGPL 2.1로  라이선스가  부여되었습니다. 자세한  내용은 [LICENSE](https://github.com/Hunesion/ossNacClient/blob/master/LICENSE) 파일을  참조하십시오.
