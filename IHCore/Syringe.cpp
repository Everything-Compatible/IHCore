#include<YRPP.h>
#include "Version.h"

//IHS 24/02/09
const DWORD YR_SIZE_1000 = 0x496110;
const DWORD YR_SIZE_1001 = 0x497110;
const DWORD YR_SIZE_1001_UC = 0x497FE0;
const DWORD YR_SIZE_NPATCH = 0x5AB000;

const DWORD YR_TIME_1000 = 0x3B846665;
const DWORD YR_TIME_1001 = 0x3BDF544E;

const DWORD YR_CRC_1000 = 0xB701D792;
const DWORD YR_CRC_1001_CD = 0x098465B3;
const DWORD YR_CRC_1001_TFD = 0xEB903080;
const DWORD YR_CRC_1001_UC = 0x1B499086;

SYRINGE_HANDSHAKE(pInfo)
{
	if (pInfo)
	{
		const char* AcceptMsg = "Found Yuri's Revenge %s. Applying " PRODUCT_FULLNAME_STR " . ";
		const char* PatchDetectedMessage = "Found %s. " PRODUCT_FULLNAME_STR " is not compatible with other patches.";

		const char* desc = nullptr;
		const char* msg = nullptr;
		bool allowed = false;

		// accept tfd and cd version 1.001
		if (pInfo->exeTimestamp == YR_TIME_1001)
		{

			// don't accept expanded exes
			switch (pInfo->exeFilesize)
			{
			case YR_SIZE_1001:
			case YR_SIZE_1001_UC:

				// all versions allowed
				switch (pInfo->exeCRC)
				{
				case YR_CRC_1001_CD:
					desc = "1.001 (CD)";
					break;
				case YR_CRC_1001_TFD:
					desc = "1.001 (TFD)";
					break;
				case YR_CRC_1001_UC:
					desc = "1.001 (UC)";
					break;
				default:
					// no-cd, network port or other changes
					desc = "1.001 (modified)";
				}
				msg = AcceptMsg;
				allowed = true;
				break;

			case YR_SIZE_NPATCH:
				// known patch size
				desc = "RockPatch or an NPatch-derived patch";
				msg = PatchDetectedMessage;
				break;

			default:
				// expanded exe, unknown make
				desc = "an unknown game patch";
				msg = PatchDetectedMessage;
			}
		}
		else if (pInfo->exeTimestamp == YR_TIME_1000)
		{
			// upgrade advice for version 1.000
			desc = "1.000";
			msg = "Found Yuri's Revenge 1.000 but " PRODUCT_FULLNAME_STR " requires version 1.001. Please update your copy of Yuri's Revenge first.";
		}
		else
		{
			// does not even compute...
			msg = "Unknown executable. " PRODUCT_FULLNAME_STR " requires Command & Conquer Yuri's Revenge version 1.001 (gamemd.exe).";
		}

		// generate the output message
		if (pInfo->Message)
		{
			sprintf_s(pInfo->Message, pInfo->cchMessage, msg, desc);
		}

		return allowed ? S_OK : S_FALSE;
	}

	return E_POINTER;
}
