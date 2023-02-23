#pragma once

#define CheckError(e) if (e > 0) return e;
#define CheckFailed(hr, e) if (FAILED(hr)) return e;

template <typename T>
void safeRelease(T* &obj)
{
	if (obj != nullptr)
	{
		obj->Release();
		obj = nullptr;
	}
}