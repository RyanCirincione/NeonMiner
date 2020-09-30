#include "Chunk.h"

#include <fstream>
#include <string>

Chunk::Chunk(int x, int y)
{
	chunkX = x;
	chunkY = y;
}

Chunk::~Chunk()
{
}

void Chunk::writeTo(std::string file) {
	std::fstream fileStream(file);

	while (true) {
		char numString[5];
		numString[4] = '\0';
		fileStream.read(numString, 4);
		int fileX = std::stoi(numString);
		fileStream.seekg(1, fileStream.cur); // Space
		fileStream.read(numString, 4);
		int fileY = std::stoi(numString);
		fileStream.seekg(1, fileStream.cur); // Space

		if (fileX == chunkX && fileY == chunkY) {
			//Convert 2d array to char array
			char* writeData = this->toString();

			fileStream.seekp(fileStream.tellg());
			fileStream.write(writeData, TILES_PER_CHUNK * TILES_PER_CHUNK);

			break;
		}
		else {
			fileStream.seekg(TILES_PER_CHUNK * TILES_PER_CHUNK, fileStream.cur);
		}

		int bookmark = fileStream.tellg();
		fileStream.seekg(0, fileStream.end);
		if ((int)fileStream.tellg() - bookmark < TILES_PER_CHUNK * TILES_PER_CHUNK) {
			fileStream.seekp(0, fileStream.end);
			char numOutputString[12 + TILES_PER_CHUNK * TILES_PER_CHUNK];
			sprintf_s(numOutputString, "\n%04d %04d ", chunkX, chunkY);
			fileStream.write(numOutputString, 11);

			char* writeData = this->toString();
			fileStream.write(writeData, TILES_PER_CHUNK * TILES_PER_CHUNK);

			break;
		}
		fileStream.seekg(bookmark + 2); // Return to current line, then move forward 2 to skip \n
	}
}

char* Chunk::toString() {
	char* result = new char[TILES_PER_CHUNK * TILES_PER_CHUNK + 1]; // 1 chars for each tile, +1 for \0
	for (int i = 0; i < TILES_PER_CHUNK * TILES_PER_CHUNK; i++) {
		result[i] = char('a' + data[i / TILES_PER_CHUNK][i % TILES_PER_CHUNK]);
	}
	result[TILES_PER_CHUNK * TILES_PER_CHUNK] = '\0';

	return result;
}

Chunk* Chunk::readChunkFrom(std::string file, int x, int y) {
	std::fstream fileStream(file);

	while (true) {
		char numString[5];
		fileStream.read(numString, 4);
		int fileX = std::stoi(numString);
		fileStream.seekg(1, fileStream.cur); // Space
		fileStream.read(numString, 4);
		int fileY = std::stoi(numString);
		fileStream.seekg(1, fileStream.cur); // Space

		if (fileX == x && fileY == y) {
			//Convert 2d array to char array
			char readData[TILES_PER_CHUNK * TILES_PER_CHUNK + 1];

			fileStream.read(readData, TILES_PER_CHUNK * TILES_PER_CHUNK);
			return Chunk::fromString(readData, x, y);
		}

		int bookmark = fileStream.tellg();
		fileStream.seekg(0, fileStream.end);
		if ((int)fileStream.tellg() - bookmark < TILES_PER_CHUNK * TILES_PER_CHUNK) {
			return NULL;
		}
		fileStream.seekg(bookmark);
	}
}

Chunk* Chunk::fromString(char* str, int x, int y) {
	Chunk* result = new Chunk(x, y);
	for (int i = 0; i < TILES_PER_CHUNK * TILES_PER_CHUNK; i++) {
		result->data[i / TILES_PER_CHUNK][i % TILES_PER_CHUNK] = str[i] - 'a';
	}

	return result;
}
