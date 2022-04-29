#include "qoiimpls.hpp"

#include <qoixx.hpp>

void encode()
{
	std::vector<unsigned char> pix;
	qoixx::qoi::desc desc{0,0,4,qoixx::qoi::colorspace::linear};
	auto res = qoixx::qoi::encode<std::vector<std::uint8_t>>(pix, desc);
//	auto enc = qoixx::qoi::encode<std::pair<std::unique_ptr<uint8_t[]>, std::size_t>>(data, 0, desc);
}
void decode()
{
	std::vector<unsigned char> dat;
	auto res = qoixx::qoi::decode<std::vector<std::uint8_t>>(dat, 4);
}
