NTSTATUS DllInject(HANDLE hProcessID, PEPROCESS pepProcess, PKTHREAD pktThread)
{
	HANDLE hProcess;
	OBJECT_ATTRIBUTES oaAttributes={sizeof(OBJECT_ATTRIBUTES)};
	CLIENT_ID cidProcess;
	PVOID pvMemory = 0;
	DWORD dwSize = 0x1000;

	cidProcess.UniqueProcess = hProcessID;
	cidProcess.UniqueThread = 0;
	if (NT_SUCCESS(ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &oaAttributes, &cidProcess)))
	{
		if (NT_SUCCESS(ZwAllocateVirtualMemory(hProcess, &pvMemory, 0, &dwSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE)))
		{
			KAPC_STATE kasState;
			PKAPC pkaApc;
			PVOID FunctionAddress;

			KeStackAttachProcess((PKPROCESS)pepProcess, &kasState);

			FunctionAddress = GetProcAddressInModule(L"kernel32.dll", "LoadLibraryExW");
			if (!FunctionAddress) DbgPrint("GetProcAddressInModule error\n");

			wcscpy((PWCHAR)pvMemory, g_wcDllName);

			KeUnstackDetachProcess(&kasState);

			pkaApc = (PKAPC)ExAllocatePool(NonPagedPool, sizeof(KAPC));
			if (pkaApc)
			{
				KeInitializeApc(pkaApc, pktThread, 0, APCKernelRoutine, 0, (PKNORMAL_ROUTINE)FunctionAddress, UserMode, pvMemory);
				KeInsertQueueApc(pkaApc, 0, 0, IO_NO_INCREMENT);

				return STATUS_SUCCESS;
			}
		}
		else
		{
			DbgPrint("ZwAllocateVirtualMemory error\n");
		}

		ZwClose(hProcess);
	}
	else
	{
		DbgPrint("ZwOpenProcess error\n");
	}

	return STATUS_NO_MEMORY;
}
