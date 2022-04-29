#include "qoiimpls.hpp"

#include <MiniQOI.h>

void decode()
{
	mQOI::Descriptor desc;
	desc.push(0x00);
	if(desc.done());
	uint32_t width, height;
	desc.verify(&width, &height);

	mQOI::Decoder dec(0);
	uint8_t adv = dec.take(nullptr);
	while(mQOI::RGBA* pix = dec.pop());
	if(dec.done());
}
