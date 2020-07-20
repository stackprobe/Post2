/*
	Keys, Values, BinKeys ... 表示可能な文字列である。

	文字列    保証される正規化
	-----------------------------
	Keys      line2JToken(, 1, 1)
	Values    line2JLine(, 1, 1, 1, 1)
	BinKeys   line2JToken(, 1, 1)
*/

#include "Parser.h"

static autoList_t *Keys;
static autoList_t *Values;
static autoList_t *BinKeys;
static autoList_t *BinValues;

void AddBodyValue_xx(char *key, char *value)
{
	errorCase(!key);
	errorCase(!value);

	errorCase(200 < getCount(Keys)); // パート多過ぎ！

	line2JToken(key, 1, 1);
	line2JLine(value, 1, 1, 1, 1);

	addElement(Keys, (uint)key);
	addElement(Values, (uint)value);
}
void AddBodyBinValue_xx(char *key, autoBlock_t *value)
{
	errorCase(!key);
	errorCase(!value);

	errorCase(200 < getCount(BinKeys)); // バイナリパート多過ぎ！

	line2JToken(key, 1, 1);

	addElement(BinKeys, (uint)key);
	addElement(BinValues, (uint)value);
}
static void ParseMain(void)
{
	char *contentType = GetHeaderValue("Content-Type");

	if(startsWithICase(contentType, "application/x-www-form-urlencoded"))
	{
		ParseFormUrlEncoded();
		return;
	}
	if(startsWithICase(contentType, "multipart/form-data"))
	{
		ParseMultiPartFormData();
		return;
	}
	error();
}
static void INIT(void)
{
	{
		static int passed;

		if(passed)
			return;

		passed = 1;
	}

	Keys = newList();
	Values = newList();
	BinKeys = newList();
	BinValues = newList();

	ParseMain();
}
char *GetBodyValue(char *key)
{
	uint index;

	errorCase(!key);

	INIT();

	index = findLineCase(Keys, key, 1);

	if(index == getCount(Keys))
	{
		static char *dummy;

		if(!dummy)
			dummy = strx("");

		return dummy;
	}
	return (char *)getElement(Values, index);
}
autoBlock_t *GetBodyBinValue(char *key)
{
	uint index;

	errorCase(!key);

	INIT();

	index = findLineCase(BinKeys, key, 1);

	if(index == getCount(Keys))
	{
		static autoBlock_t *dummy;

		if(!dummy)
			dummy = newBlock();

		return dummy;
	}
	return (autoBlock_t *)getElement(BinValues, index);
}
