변경사항
=============

### v0.1.0

+ BOND 포멧 HEAD 부분 읽기 구현.
+ 청크 헤더, 속성 청크 읽기 구현.
+ 속성 청크 데이터 형 변환 구현. STRING, DWORD, FILETIME.
+ hv3 포멧 여부 검사 구현.
+ hv3 상태 정보를 제공.

### v0.2.0

+ 속성 청크 데이터 형 변환 구현. GUID, UUID.
+ CRC32 검사를 위한 서브루틴 작성.

### v0.3.0

+ BOND 포멧 BODY 부분 읽기 구현.

#### v0.3.1

+ `파일이 아님' 오류 검사를 삭제.
+ getLastError 메소드를 통해 상태 확인을 하도록 변경.
	+ 기존에 상태 코드를 반환하던 메소드는 bool 형태로 성공여부를 반환하도록 수정.
+ HD PHOTO 1.O DEVICE PORTING KIT를 프로젝트에 포함.

#### v0.3.2

+ HD PHOTO 라이브러리를 jxrlib 1.1로 바꿈.
+ jxrlib를 사용하여 시험적인 hdp -> bmp 변환 서브루틴 작성.

#### v0.3.3

+ .HDP -> .BMP 변환 클래스 작성 - 마무리 단계.

### v0.4.0

+ \`HDP -> BMP -> JPEG'를 \`HDP -> JPEG(.PNG)' 순서로 변환 되도록 개선.

### v0.5.0

+ HdpConvert를 완성.
	+ HdpConvert::toJpeg 메소드 사용시 JPEG 파일의 화질이 손상되어 저장되던 문제 수정.
	+ HdpConvert를 싱글톤 객체로 수정.
	+ HdpConvert relrease 메소드를 추가하고, 외부에서 소멸자를 호출 할 수 없도록 수정.
	+ HdpConvert::hasAlphaChannel 메소드를 추가. 알파값 포함 여부를 확인 할 수 있도록함.
	+ 변환부에서 알파채널 존재 여부에 따라 데이터를 기록하도록 수정.
	+ jxrlib에서 반환되는 오류를 처리하는 부분 추가.
	+ 오류 처리를 위한 WMP_err 클래스를 추가.

#### v0.5.1

+ Unhv3Status 클래스에 상태 메시지를 반환하는 메소드와 상태 코드를 반환하는 메소드를 추가.

### v0.6.0

+ 이미지 데이터를 요구할 때 불러와서 처리하도록 수정.
	+ 데이터 반환 메소드를 추가.
+ Unhv3::extractOneTo를 추가. 선택된 인덱스에 대한 파일을 지정된 경로에 풀수 있게 함.
+ getFileItemCount 메소드를 추가. 포함된 파일 목록의 수를 반환.
+ getFileItem 메소드를 추가. 파일 정보를 가져옴.

### v0.7.0

+ 전체 압축 해제 메소드 Unhv3::extractAllTo를 추가.
