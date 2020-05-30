#include "Sync.h"

HANDLE mutex, thread;

BOOL WINAPI
DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH: {
		wprintf(L"DllMain: attach process\n");
		mutex = CreateMutexExW(
			NULL, NULL,
			1, // CREATE_MUTEX_INITIAL_OWNER, -- not defined?
			EVENT_ALL_ACCESS
		);
		
		thread = CreateThread(
			NULL, 0,
			&ThreadMain, NULL,
			0, NULL
		);
		
		ReleaseMutex(mutex);
		break;
	}
	case DLL_PROCESS_DETACH: {
		wprintf(L"DllMain: detach process\n");
		break;
	}
	case DLL_THREAD_ATTACH: {
		wprintf(L"DllMain: attach thread\n");
		break;
	}
	case DLL_THREAD_DETACH: {
		wprintf(L"DllMain: detach thread\n");
		break;
	}
	}
	
	return TRUE;
}

long unsigned int ThreadMain() {
	wprintf(L"ThreadMain: pre-lock\n");
	WaitForSingleObject(mutex, INFINITE);
	wprintf(L"ThreadMain: post-lock\n");
}

BOOL Test() {
	return TRUE;
}