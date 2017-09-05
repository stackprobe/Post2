#include "all.h"

static autoList_t *Names;
static autoList_t *Values;

static void ParseEnv(char *text)
{
	autoList_t *lines = tokenize(text, '\n');
	char *line;
	uint index;

	Names = newList();
	Values = newList();

	foreach(lines, line, index)
	{
		autoList_t *tokens = tokenizeMinMax(line, '=', 1, 2, NULL);

		if(getCount(tokens) == 2)
		{
			addElement(Names,  (uint)strx(getLine(tokens, 0)));
			addElement(Values, (uint)strx(getLine(tokens, 1)));
		}
		releaseDim(tokens, 1);
	}
	releaseDim(lines, 1);
}
static char *UnparseEnv(void)
{
	autoBlock_t *buff = newBlock();
	char *name;
	uint index;

	foreach(Names, name, index)
	{
		if(index)
			ab_addChar(buff, '\n');

		ab_addLine(buff, name);
		ab_addChar(buff, '=');
		ab_addLine(buff, getLine(Values, index));
	}
	return unbindBlock2Line(buff);
}
static void ReleaseEnv(void)
{
	releaseDim(Names, 1);
	releaseDim(Values, 1);
}
char *EnvGetValue(char *env, char *name)
{
	uint index;
	char *ret;

	errorCase(!env);
	errorCase(m_isEmpty(name));

	ParseEnv(env);

	index = findLineCase(Names, name, 1);

	if(index == getCount(Names))
		ret = "";
	else
		ret = getLine(Values, index);

	ret = strx(ret);
	ReleaseEnv();
	return ret;
}
char *EnvSetValue_ccc(char *env, char *name, char *value)
{
	uint index;

	errorCase(!env);
	errorCase(m_isEmpty(name));
	errorCase(!value);

	ParseEnv(env);

	index = findLineCase(Names, name, 1);

	if(index == getCount(Names))
	{
		addElement(Names,  (uint)strx(name));
		addElement(Values, (uint)strx(value));
	}
	else
		strzp((char **)directGetPoint(Values, index), value);

	env = UnparseEnv();
	ReleaseEnv();
	return env;
}
char *EnvSetValue(char *env, char *name, char *value) // ret: strr(env)
{
	char *out = EnvSetValue_ccc(env, name, value);
	memFree(env);
	return out;
}

// c_
char *c_EnvGetValue(char *env, char *name)
{
	static char *stock;
	memFree(stock);
	return stock = EnvGetValue(env, name);
}
