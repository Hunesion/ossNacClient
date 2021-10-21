#ifndef _HUNE_CORE_CORE_H_
#define _HUNE_CORE_CORE_H_
#include "macro.h"
#include "model/ClientConfig.h"
#include "model/Define.h"
#include "model/StringResource.h"
#include "model/GlobalVar.h"
#include "StringUtils.h"
#include "IonexLogic.h"
#include "Event.h"
#include "EventManager.h"
#include "FileUtils.h"
#include "Logger.h"


BEGIN_HUNE_CORE

bool hune_core_init(); 
void hune_core_dispose(); 
END_HUNE_CORE

#endif /* _HUNE_CORE_CORE_H_ */