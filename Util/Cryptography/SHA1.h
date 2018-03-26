#pragma once
#include <string>
#include <fstream>
#include "NS.h"
/*
sha1.hpp - header of
============
SHA-1 in C++
============
100% Public Domain.
Original C Code
-- Steve Reid <steve@edmweb.com>
Small changes to fit into bglibs
-- Bruce Guenter <bruce@untroubled.org>
Translation to simpler C++ Code
-- Volker Grabsch <vog@notjusthosting.com>
Safety fixes
-- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/
NS_CRYPTOGRAPHY_BEGIN
class SHA1
{
public:
	SHA1() { Reset(); }
	virtual ~SHA1() {}
public:
	/*std::string ComputeHash(std::ifstream& stream);*/
	std::string ComputeHash(std::string& plainText);
	std::string ComputeHash(char* plainText);
private:
	static const uint64_t _BLOCK_INTS = 16;
	static const uint64_t _BLOCK_BYTES = _BLOCK_INTS * 4;
private:
	uint32_t Rol(const uint32_t value, const size_t bits);
	uint32_t Blk(const uint32_t block[_BLOCK_INTS], const size_t i);
	void R0(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
	void R1(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
	void R2(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
	void R3(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
	void R4(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i);
	void Transform(uint32_t digest[], uint32_t block[_BLOCK_INTS], uint32_t &transforms);
	void Buffer_To_Block(const std::string &buffer, uint32_t block[_BLOCK_INTS]);
	void Reset();
	std::string Final();
private:
	std::string _buffer;
	uint32_t _digest[5];
	uint32_t _transforms;
};
NS_CRYPTOGRAPHY_END