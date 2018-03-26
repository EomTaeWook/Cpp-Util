#include "SHA1.h"
#include <sstream>
#include <iomanip>

/*
sha1.cpp - source code of
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
std::string SHA1::ComputeHash(char* plainText)
{
	std::string _plainText(plainText);
	return ComputeHash(_plainText);
}

std::string SHA1::ComputeHash(std::string& plainText)
{
	size_t size = 0;
	unsigned int read = 0;
	while (true)
	{
		if (_BLOCK_BYTES > plainText.size())
			size = plainText.size();
		else if ((read + 1) * _BLOCK_BYTES < plainText.size())
			size = _BLOCK_BYTES;
		else
			size = plainText.size() - ((read + 1) * _BLOCK_BYTES);
		_buffer.append(plainText.substr(read * _BLOCK_BYTES, size));
		if (_buffer.size() != _BLOCK_BYTES)
		{
			return Final();
		}
		uint32_t block[_BLOCK_INTS];
		Buffer_To_Block(_buffer, block);
		Transform(_digest, block, _transforms);
		_buffer.clear();
		read++;
	}
	return Final();
}
std::string SHA1::Final()
{
	uint64_t total_bits = (_transforms * _BLOCK_BYTES + _buffer.size()) * 8;
	_buffer += (char)0x80;
	size_t oriSize = _buffer.size();
	while (_buffer.size() < _BLOCK_BYTES)
	{
		_buffer += (char)0x00;
	}
	uint32_t block[_BLOCK_INTS];
	Buffer_To_Block(_buffer, block);

	if (oriSize > _BLOCK_BYTES - 8)
	{
		Transform(_digest, block, _transforms);
		for (size_t i = 0; i < _BLOCK_INTS - 2; i++)
		{
			block[i] = 0;
		}
	}
	block[_BLOCK_INTS - 1] = (uint32_t)total_bits;
	block[_BLOCK_INTS - 2] = (total_bits >> 32);
	Transform(_digest, block, _transforms);

	/* Hex std::string */
	std::ostringstream result;

	for (size_t i = 0; i < sizeof(_digest) / sizeof(_digest[0]); i++)
	{
		result << std::hex << std::setfill('0') << std::setw(8);
		result << (_digest[i] & 0xffffffff);
	}
	/* Reset for next run */
	Reset();
	return result.str();
}
uint32_t SHA1::Rol(const uint32_t value, const size_t bits)
{
	return (value << bits) | (value >> (32 - bits));
}
uint32_t SHA1::Blk(const uint32_t block[_BLOCK_INTS], const size_t i)
{
	return Rol(block[(i + 13) & 15] ^ block[(i + 8) & 15] ^ block[(i + 2) & 15] ^ block[i], 1);
}

// (R0+R1), R2, R3, R4 are the different operations used in SHA1
void SHA1::R0(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
	z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + Rol(v, 5);
	w = Rol(w, 30);
}
void SHA1::R1(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
	block[i] = Blk(block, i);
	z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + Rol(v, 5);
	w = Rol(w, 30);
}
void SHA1::R2(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (w ^ x ^ y) + block[i] + 0x6ed9eba1 + Rol(v, 5);
	w = Rol(w, 30);
}
void SHA1::R3(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (((w | x) & y) | (w & x)) + block[i] + 0x8f1bbcdc + Rol(v, 5);
	w = Rol(w, 30);
}
void SHA1::R4(uint32_t block[_BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (w ^ x ^ y) + block[i] + 0xca62c1d6 + Rol(v, 5);
	w = Rol(w, 30);
}

// Digest a single 512-bit block. This is the core of the algorithm.
void SHA1::Transform(uint32_t digest[], uint32_t block[_BLOCK_INTS], uint32_t &transforms)
{
	// Copy digest[] to working vars
	uint32_t a = digest[0];
	uint32_t b = digest[1];
	uint32_t c = digest[2];
	uint32_t d = digest[3];
	uint32_t e = digest[4];

	// 4 rounds of 20 operations each. Loop unRolled.
	R0(block, a, b, c, d, e, 0);
	R0(block, e, a, b, c, d, 1);
	R0(block, d, e, a, b, c, 2);
	R0(block, c, d, e, a, b, 3);
	R0(block, b, c, d, e, a, 4);
	R0(block, a, b, c, d, e, 5);
	R0(block, e, a, b, c, d, 6);
	R0(block, d, e, a, b, c, 7);
	R0(block, c, d, e, a, b, 8);
	R0(block, b, c, d, e, a, 9);
	R0(block, a, b, c, d, e, 10);
	R0(block, e, a, b, c, d, 11);
	R0(block, d, e, a, b, c, 12);
	R0(block, c, d, e, a, b, 13);
	R0(block, b, c, d, e, a, 14);
	R0(block, a, b, c, d, e, 15);
	R1(block, e, a, b, c, d, 0);
	R1(block, d, e, a, b, c, 1);
	R1(block, c, d, e, a, b, 2);
	R1(block, b, c, d, e, a, 3);
	R2(block, a, b, c, d, e, 4);
	R2(block, e, a, b, c, d, 5);
	R2(block, d, e, a, b, c, 6);
	R2(block, c, d, e, a, b, 7);
	R2(block, b, c, d, e, a, 8);
	R2(block, a, b, c, d, e, 9);
	R2(block, e, a, b, c, d, 10);
	R2(block, d, e, a, b, c, 11);
	R2(block, c, d, e, a, b, 12);
	R2(block, b, c, d, e, a, 13);
	R2(block, a, b, c, d, e, 14);
	R2(block, e, a, b, c, d, 15);
	R2(block, d, e, a, b, c, 0);
	R2(block, c, d, e, a, b, 1);
	R2(block, b, c, d, e, a, 2);
	R2(block, a, b, c, d, e, 3);
	R2(block, e, a, b, c, d, 4);
	R2(block, d, e, a, b, c, 5);
	R2(block, c, d, e, a, b, 6);
	R2(block, b, c, d, e, a, 7);
	R3(block, a, b, c, d, e, 8);
	R3(block, e, a, b, c, d, 9);
	R3(block, d, e, a, b, c, 10);
	R3(block, c, d, e, a, b, 11);
	R3(block, b, c, d, e, a, 12);
	R3(block, a, b, c, d, e, 13);
	R3(block, e, a, b, c, d, 14);
	R3(block, d, e, a, b, c, 15);
	R3(block, c, d, e, a, b, 0);
	R3(block, b, c, d, e, a, 1);
	R3(block, a, b, c, d, e, 2);
	R3(block, e, a, b, c, d, 3);
	R3(block, d, e, a, b, c, 4);
	R3(block, c, d, e, a, b, 5);
	R3(block, b, c, d, e, a, 6);
	R3(block, a, b, c, d, e, 7);
	R3(block, e, a, b, c, d, 8);
	R3(block, d, e, a, b, c, 9);
	R3(block, c, d, e, a, b, 10);
	R3(block, b, c, d, e, a, 11);
	R4(block, a, b, c, d, e, 12);
	R4(block, e, a, b, c, d, 13);
	R4(block, d, e, a, b, c, 14);
	R4(block, c, d, e, a, b, 15);
	R4(block, b, c, d, e, a, 0);
	R4(block, a, b, c, d, e, 1);
	R4(block, e, a, b, c, d, 2);
	R4(block, d, e, a, b, c, 3);
	R4(block, c, d, e, a, b, 4);
	R4(block, b, c, d, e, a, 5);
	R4(block, a, b, c, d, e, 6);
	R4(block, e, a, b, c, d, 7);
	R4(block, d, e, a, b, c, 8);
	R4(block, c, d, e, a, b, 9);
	R4(block, b, c, d, e, a, 10);
	R4(block, a, b, c, d, e, 11);
	R4(block, e, a, b, c, d, 12);
	R4(block, d, e, a, b, c, 13);
	R4(block, c, d, e, a, b, 14);
	R4(block, b, c, d, e, a, 15);

	// Add the working vars back into digest[]
	digest[0] += a;
	digest[1] += b;
	digest[2] += c;
	digest[3] += d;
	digest[4] += e;

	// Count the number of transformations
	transforms++;
}
void SHA1::Buffer_To_Block(const std::string &buffer, uint32_t block[_BLOCK_INTS])
{
	/* Convert the std::string (byte buffer) to a uint32_t array (MSB) */
	for (size_t i = 0; i < _BLOCK_INTS; i++)
	{
		block[i] = (buffer[4 * i + 3] & 0xff)
			| (buffer[4 * i + 2] & 0xff) << 8
			| (buffer[4 * i + 1] & 0xff) << 16
			| (buffer[4 * i + 0] & 0xff) << 24;
	}
}
void SHA1::Reset()
{
	/* SHA1 initialization constants */
	_digest[0] = 0x67452301;
	_digest[1] = 0xefcdab89;
	_digest[2] = 0x98badcfe;
	_digest[3] = 0x10325476;
	_digest[4] = 0xc3d2e1f0;
	/* Reset counters */
	_buffer.clear();
	_transforms = 0L;
}
NS_CRYPTOGRAPHY_END