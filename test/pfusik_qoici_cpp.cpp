#include "qoiimpls.hpp"

#include <QOI.hpp>

void encode()
{
	::QOIEncoder encoder;

	bool succ = encoder.encode(0, 0, nullptr, true, true);
	bool can = encoder.canEncode(0, 0, true);
	const uint8_t* data = encoder.getEncoded();
	int size = encoder.getEncodedSize();
}
void decode()
{
	::QOIDecoder decoder;

	bool succ = decoder.decode(nullptr, 0);
	const int* data = decoder.getPixels();
	int width = decoder.getWidth();
	int height = decoder.getHeight();
	bool alpha = decoder.hasAlpha();
	bool linear = decoder.isLinearColorspace();
}
