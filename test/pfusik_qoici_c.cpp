#include "qoiimpls.hpp"

#include <QOI.h>

void encode()
{
	::QOIEncoder* encoder = QOIEncoder_New();

	bool succ = ::QOIEncoder_Encode(encoder, 0, 0, nullptr, true, true);
	bool can = ::QOIEncoder_CanEncode(0, 0, true);
	const uint8_t* data = ::QOIEncoder_GetEncoded(encoder);
	int size = ::QOIEncoder_GetEncodedSize(encoder);

	::QOIEncoder_Delete(encoder);
}
void decode()
{
	::QOIDecoder* decoder = QOIDecoder_New();

	bool succ = ::QOIDecoder_Decode(decoder, nullptr, 0);
	const int* data = ::QOIDecoder_GetPixels(decoder);
	int width = ::QOIDecoder_GetWidth(decoder);
	int height = ::QOIDecoder_GetHeight(decoder);
	bool alpha = ::QOIDecoder_HasAlpha(decoder);
	bool linear = ::QOIDecoder_IsLinearColorspace(decoder);

	::QOIDecoder_Delete(decoder);
}
