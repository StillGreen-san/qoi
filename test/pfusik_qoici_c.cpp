#include "qoiimpls.hpp"

#include <QOI.h>

namespace impl::pfusik::qoici::c
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi) : decoder{::QOIDecoder_New()}
	{
		QOIDecoder_Decode(decoder, qoi.data(), qoi.size());
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(QOIDecoder_GetPixels(decoder));
	}
	size_t size() override
	{
		return width() * height() * channels();
	}
	uint32_t width() override
	{
		return QOIDecoder_GetWidth(decoder);
	}
	uint32_t height() override
	{
		return QOIDecoder_GetHeight(decoder);
	}
	uint8_t channels() override
	{
		return impl::channels(QOIDecoder_HasAlpha(decoder));
	}
	uint8_t colorspace() override
	{
		return impl::colorspace(QOIDecoder_IsLinearColorspace(decoder));
	}
	~Decoder() override
	{
		::QOIDecoder_Delete(decoder);
	}
	::QOIDecoder* decoder;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription description) :
	    encoder{::QOIEncoder_New()}, description{description}
	{
		QOIEncoder_Encode(encoder, description.width, description.height, reinterpret_cast<const int*>(raw.data()),
		    alpha(description.channels), linear(description.colorspace));
	}
	const uint8_t* data() override
	{
		return QOIEncoder_GetEncoded(encoder);
	}
	size_t size() override
	{
		return QOIEncoder_GetEncodedSize(encoder);
	}
	uint32_t width() override
	{
		return description.width;
	}
	uint32_t height() override
	{
		return description.height;
	}
	uint8_t channels() override
	{
		return description.channels;
	}
	uint8_t colorspace() override
	{
		return description.colorspace;
	}
	~Encoder() override
	{
		::QOIEncoder_Delete(encoder);
	}
	::QOIEncoder* encoder;
	ImageDescription description;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}

std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::pfusik::qoici::c
