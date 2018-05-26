static
NTSTATUS
GetUBRNumber(
	OUT	PULONG pulUBRNumber
)
{
	NTSTATUS	NtStatus;
	HANDLE	hRegKey = NULL;
	OBJECT_ATTRIBUTES Attributes;
	UNICODE_STRING	usRegKeyName, usRegValueName, usUBRNo;
	PKEY_VALUE_PARTIAL_INFORMATION 	pKeyValueInfo = NULL;
	ULONG	ulActualSize = 0;

	// 인수 검사
	if (pulUBRNumber == NULL)
	{
		NtStatus = STATUS_INVALID_PARAMETER;
		goto Final;
	}

	// 커널 정보가 저장된 레지스트리키를 OPEN 한다.
	RtlInitUnicodeString(&usRegKeyName, REGISTRY_KEY_WINDOWS_INFO);

	InitializeObjectAttributes(
		&Attributes,
		&usRegKeyName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);

	// 레지스트리 키 Open
	NtStatus = ZwOpenKey(
		&hRegKey,
		KEY_QUERY_VALUE,
		&Attributes
	);

	// 레지스트리 키 Open 실패
	if (NT_SUCCESS(NtStatus) != TRUE)
	{
		goto Final;
	}
	
	// 레지스트리의 'UBR' 유니코드 초기화
	RtlInitUnicodeString(&usRegValueName, REGISTRY_VALUE_KERNEL_UBR);

	// 필요한 VALUE 버퍼의 크기를 구한다.
	NtStatus = ZwQueryValueKey(
		hRegKey,
		&usRegValueName,
		KeyValuePartialInformation,
		NULL,
		0,
		&ulActualSize
	);

	// Value 버퍼 크기 구하는 것 실패 (의도적으로 크기를 구하기 위해 0으로 Query하여 기대한 결과)
	if (STATUS_BUFFER_TOO_SMALL != NtStatus)
	{
		goto Final;
	}

	pKeyValueInfo = ExAllocatePool(PagedPool, ulActualSize);

	// 메모리 할당 실패
	if (pKeyValueInfo == NULL)
	{
		NtStatus = STATUS_NO_MEMORY;
		goto Final;
	}

	// 레지스트리 'UBR'값을 쿼리해 온다.
	NtStatus = ZwQueryValueKey(
		hRegKey,
		&usRegValueName,
		KeyValuePartialInformation,
		pKeyValueInfo,
		ulActualSize,
		&ulActualSize
	);

	// 'UBR' 값 구하는 것 실패
	if (NT_SUCCESS(NtStatus) != TRUE)
	{
		goto Final;
	}

	//REG_DWORD 4byte 값을 읽어와야 하는데 pKeyValueInfo->Data는 char형이므로 int형으로 캐스팅해서 읽는다.
	*pulUBRNumber = *((PULONG)(pKeyValueInfo->Data));

Final:

	if (NULL != hRegKey)
	{
		ZwClose(hRegKey);
	}

	if (NULL != pKeyValueInfo)
	{
		ExFreePool(pKeyValueInfo);
	}

	return	NtStatus;
}
