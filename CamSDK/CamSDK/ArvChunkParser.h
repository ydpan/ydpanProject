#pragma once
#include "BaseDataDefine.h"
class ArvChunkParser
{//Parser for extraction of chunk data from buffers
public:
	ArvChunkParser();
	~ArvChunkParser();
};
class ChunkParser {
public:
	ArvChunkParser *	arv_chunk_parser_new();
	double	arv_chunk_parser_get_float_value();
	lp_int64	arv_chunk_parser_get_integer_value();
	const char *	arv_chunk_parser_get_string_value();
};
