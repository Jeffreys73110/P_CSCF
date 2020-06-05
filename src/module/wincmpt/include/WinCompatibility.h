/*****************************************************************************
 Windows compatibility v 5.0
 ======================================
 Last update : 2016.06.30
*****************************************************************************/

/* *****************************************************************************
 * Note for RTOS or unusual-OS platform:
 * If you want to avoid conflicts of standard library name or -
 * you want to use your own standard library name,
 * you should add #define AVOID_UOS_LIB_CONFLICTS -
 * before you #include "WinCompatibility.h" in your project.
 *
 * When you try to build wincmpt library from open source code,
 * Please don't define AVOID_UOS_LIB_CONFLICTS in your makefile !
 *
 * Example for your coding project:
 * #ifndef AVOID_UOS_LIB_CONFLICTS
 * #define AVOID_UOS_LIB_CONFLICTS
 * #endif
 * #include "WinCompatibility.h"
 * *****************************************************************************/

#include "BaseInclude.h"
#include "SimpleBase64.h"
#include "DataHexString.h"
#include "MD5.h"
#include "CAES.h"
#include "CSHA2.h"
#include "CRC32.h"
#include "CIniReader.h"
#include "MurmurHash.h"
#include "CSocket.h"
#include "IPvInfo.h"
#include "ErrnoCmpt.h"
#include "OMath.h"
#include "SRamAlloc.h"
#include "StdStatic.h"
#include "MemDB.h"
#include "MemPool.h"
#include "StringPatch.h"
#include "StrLinePool.h"
#include "SuitFIO.h"
#include "Threading.h"
#include "TryScanf.h"
#include "TrySleep.h"
#include "TyDebug.h"
#include "TypeDefine.h"
#include "UnixTimeDate.h"
#include "RBTree.h"
#include "CJson.h"
#include "CQueueEvent.h"
#include "XCurl.h"
