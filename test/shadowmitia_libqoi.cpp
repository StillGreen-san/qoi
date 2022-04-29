#include "qoiimpls.hpp"

#include <qoi.hpp>

void encode()
{
	std::vector<unsigned char> pix;
	auto data = qoi::encode(pix, 0, 0, 4);
}
void decode()
{
	std::vector<unsigned char> data;
	auto pix = qoi::decode(data);
	bool vld = qoi::is_valid(data);
	auto desc = qoi::get_header(data);
}
