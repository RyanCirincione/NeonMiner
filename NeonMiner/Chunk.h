#pragma once

#include "Constants.h"
#include <string>

class Chunk
{
public:
	Chunk(int x, int y);
	~Chunk();
	void writeTo(std::string file);
	char* toString();
	static Chunk* readChunkFrom(std::string file, int x, int y);
	static Chunk* fromString(char* str, int x, int y);

	int chunkX, chunkY;
	int data[TILES_PER_CHUNK][TILES_PER_CHUNK];
};

