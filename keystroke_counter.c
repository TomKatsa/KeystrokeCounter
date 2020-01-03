#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

long counter = 0;
HHOOK kb_hook;

BOOL WINAPI consoleHandler(DWORD signal) { 

    if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) { // Handling Ctrl-C and program closing.
    	
    	char msg[100];
    	char filename[100];
    	time_t t;
    	time(&t);
    	struct tm *tm = localtime(&t);
    	strftime(filename, 100, "%Y-%m-%d-%Hx%M.log", tm); //filename using time
    	printf("[*] Caught program interrupt.\n");
    	sprintf(msg, "[*] %ld strokes caught!\n", counter);
    	printf("[*] Saving to %s\n", filename);

    	FILE *file = fopen(filename, "w");
    	printf("%s\n", msg);
    	fprintf(file, "%s\n", msg); // saving to log in format Y-m-d-HHxMM.log
    	fclose(file);
        BOOL exit_status = UnhookWindowsHookEx(kb_hook);
        printf("[+] Unhook exit status = %s (%d)\n", (exit_status==0) ? "FAIL" : "SUCCESS", exit_status);
    	scanf("%d");
    	exit(0);
    }
    return TRUE;
}




KBDLLHOOKSTRUCT* kbd;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (wParam==WM_KEYDOWN) {
		// Potential keylogger code:
		//kbd = (KBDLLHOOKSTRUCT*)lParam;
		//UINT vkcode = kbd->vkCode;
		//char c;
		//c = MapVirtualKeyA(vkcode, 2);
		//printf("%c",c);
		
		counter++; // Incrementing key strokes
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(int argc, char const *argv[])
{

	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) { // Console interrupts handler
        printf("\nERROR: Could not set control handler"); 
        return 1;
    }

	HINSTANCE hInstance = GetModuleHandle(NULL);
	printf("[*] hInstance = %p\n", hInstance);
    printf("[*] Installing hook...\n");
    kb_hook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    printf("[+] kb_hook = %p\n", kb_hook);
    MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) // Windows Message Loop
	{ 
   		TranslateMessage(&msg);
   		DispatchMessage(&msg);
	} 


    BOOL exit_status = UnhookWindowsHookEx(kb_hook);
    printf("[+] Exit status = %d\n", exit_status);
    return 0;
}







