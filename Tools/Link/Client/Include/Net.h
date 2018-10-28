#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <switch.h>

void Net_init(u16 Port);
void Net_waitForPC(void);
void Net_exit(void);
const char *Net_waitRecieve(void);
void Net_sendResult(const char *Encoded);
bool Net_ok(void);

#ifdef __cplusplus
}
#endif