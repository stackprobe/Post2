#pragma once

#include "Service.h"
#include "Parser\all.h"

void AddBodyValue_xx(char *key, char *value);
void AddBodyBinValue_xx(char *key, autoBlock_t *value);
char *GetBodyValue(char *key);
autoBlock_t *GetBodyBinValue(char *key);
