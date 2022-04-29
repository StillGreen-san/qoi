#include "qoiimpls.hpp"

#include <mini_qoi.h>

void decode()
{
	::mqoi_desc_t desc;
	::mqoi_desc_init(&desc);
	::mqoi_desc_push(&desc, 0x00);
	if(::mqoi_desc_done(&desc));
	uint32_t width, height;
	uint8_t err = ::mqoi_desc_verify(&desc, &width, &height);

	::mqoi_dec_t dec;
	::mqoi_dec_init(&dec, width * height);
	uint8_t adv = ::mqoi_dec_take(&dec, nullptr);
	while(mqoi_rgba_t* pix = ::mqoi_dec_pop(&dec));
	if(::mqoi_dec_done(&dec));
}