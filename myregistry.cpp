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

	// �μ� �˻�
	if (pulUBRNumber == NULL)
	{
		NtStatus = STATUS_INVALID_PARAMETER;
		goto Final;
	}

	// Ŀ�� ������ ����� ������Ʈ��Ű�� OPEN �Ѵ�.
	RtlInitUnicodeString(&usRegKeyName, REGISTRY_KEY_WINDOWS_INFO);

	InitializeObjectAttributes(
		&Attributes,
		&usRegKeyName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);

	// ������Ʈ�� Ű Open
	NtStatus = ZwOpenKey(
		&hRegKey,
		KEY_QUERY_VALUE,
		&Attributes
	);

	// ������Ʈ�� Ű Open ����
	if (NT_SUCCESS(NtStatus) != TRUE)
	{
		goto Final;
	}
	
	// ������Ʈ���� 'UBR' �����ڵ� �ʱ�ȭ
	RtlInitUnicodeString(&usRegValueName, REGISTRY_VALUE_KERNEL_UBR);

	// �ʿ��� VALUE ������ ũ�⸦ ���Ѵ�.
	NtStatus = ZwQueryValueKey(
		hRegKey,
		&usRegValueName,
		KeyValuePartialInformation,
		NULL,
		0,
		&ulActualSize
	);

	// Value ���� ũ�� ���ϴ� �� ���� (�ǵ������� ũ�⸦ ���ϱ� ���� 0���� Query�Ͽ� ����� ���)
	if (STATUS_BUFFER_TOO_SMALL != NtStatus)
	{
		goto Final;
	}

	pKeyValueInfo = ExAllocatePool(PagedPool, ulActualSize);

	// �޸� �Ҵ� ����
	if (pKeyValueInfo == NULL)
	{
		NtStatus = STATUS_NO_MEMORY;
		goto Final;
	}

	// ������Ʈ�� 'UBR'���� ������ �´�.
	NtStatus = ZwQueryValueKey(
		hRegKey,
		&usRegValueName,
		KeyValuePartialInformation,
		pKeyValueInfo,
		ulActualSize,
		&ulActualSize
	);

	// 'UBR' �� ���ϴ� �� ����
	if (NT_SUCCESS(NtStatus) != TRUE)
	{
		goto Final;
	}

	//REG_DWORD 4byte ���� �о�;� �ϴµ� pKeyValueInfo->Data�� char���̹Ƿ� int������ ĳ�����ؼ� �д´�.
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
