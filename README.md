## VSCode extension 설치
* C/C++ (ms-vscode.cpptools) → 코드 완성, 디버깅 지원
* CMake Tools (ms-vscode.cmake-tools) (선택) → CMake 프로젝트 관리
* CodeLLDB (macOS/Linux에서 LLDB 디버깅 시 필요)

## CMake shortcuts
* F7 : "CMake: Build"
* Ctrl+Shift+F5 : "CMake: Run Withdout Debugging"

## CMake commands
* cmake --build build
* cmake --build build --target run

## LLDB
Build:
```
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/mj/work/MyCmakePrj/build

$ cmake --build build
```

Execute LLDB:
```
$ lldb ./build/myapp
(lldb) target create "./build/myapp"
Current executable set to '/home/mj/work/MyCmakePrj/build/myapp' (x86_64).
(lldb) 
```

Set breakpoint and debug:
```
$ lldb ./build/myapp
(lldb) target create "./build/myapp"
Current executable set to '/home/mj/work/MyCmakePrj/build/myapp' (x86_64).
(lldb) breakpoint set --name main
Breakpoint 1: where = myapp`main + 12 at main.cpp:5:9, address = 0x0000000000001195

(lldb) run
Process 31455 launched: '/home/mj/work/MyCmakePrj/build/myapp' (x86_64)
Process 31455 stopped
* thread #1, name = 'myapp', stop reason = breakpoint 1.1
    frame #0: 0x0000555555555195 myapp`main at main.cpp:5:9
   2   	
   3   	
   4   	int main() {
-> 5   	    int n = 10;
   6   	    std::cout << "Hello world : " << n << std::endl;
   7   	    return 0;
   8   	}

(lldb) next
Process 31455 stopped
* thread #1, name = 'myapp', stop reason = step over
    frame #0: 0x000055555555519c myapp`main at main.cpp:6:18
   3   	
   4   	int main() {
   5   	    int n = 10;
-> 6   	    std::cout << "Hello world : " << n << std::endl;
   7   	    return 0;
   8   	}

(lldb) print n
(int) 10

(lldb) continue
Process 31455 resuming
Hello world : 10
Process 31455 exited with status = 0 (0x00000000) 
```

## LLDB with Vscode
Add `.vscode/launch.json` and debug by pressing `F5` in the Vscode.
```
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug with LLDB",
      "type": "lldb",
      "request": "launch",
      "program": "${workspaceFolder}/build/myapp",
      "args": [],
      "cwd": "${workspaceFolder}"
    }
  ]
}
```

