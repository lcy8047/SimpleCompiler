# Simple C compiler
                    2021. 12 .14
1. Synopsis
 - Simple C 언어의 컴파일러 소스코드입니다.

2. Files
 - flex input 파일인 lexinput.l, bison input 파일이자 문법이 정의된 grammar.y가 있습니다.
 - 또한 ast를 구성하고 코드를 생성하는 소스코드 ast.cpp, ast.hpp 파일이 있습니다.
 - 마지막으로 이를 모두 빌드하기 위한 makefile이 있습니다.
 - 빌드 후 생성되는 실행 파일 이름은 codeGen 입니다.

3. How to build
 - make 명령어로 빌드 할 수 있습니다.
 - ./codeGen < [Simple C source code] 명령어로 Simple C 소스코드를 컴파일 할 수 있습니다.
 - 컴파일 이후 tvm 머신 코드인 a.t 파일이 생성됩니다.

4. How to execute
 - Simple C Compiler는 tvm을 타겟으로 만들었습니다.
 - tvm은 https://github.com/woogyun/tvm 에서 다운받을 수 있습니다.
 - ./tvm a.t로 머신코드를 실행 할 수 있습니다.

5. Remark
 - 예제 소스코드인 add_example.sc, addfunc_example.sc, func_example.sc, example.sc 파일이 있습니다.