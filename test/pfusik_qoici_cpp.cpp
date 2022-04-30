#include "qoiimpls.hpp"

#include <QOI.hpp>

namespace impl::pfusik::qoici::cpp
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi)
	{
		decoder.decode(qoi.data(), qoi.size());
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(decoder.getPixels());
	}
	size_t size() override
	{
		return width() * height() * channels();
	}
	uint32_t width() override
	{
		return decoder.getWidth();
	}
	uint32_t height() override
	{
		return decoder.getHeight();
	}
	uint8_t channels() override
	{
		return impl::channels(decoder.hasAlpha());
	}
	uint8_t colorspace() override
	{
		return impl::colorspace(decoder.isLinearColorspace());
	}
	QOIDecoder decoder;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription description) :
	    description{description}
	{
		encoder.encode(description.width, description.height, reinterpret_cast<const int*>(raw.data()),
		    alpha(description.channels), linear(description.colorspace));
	}
	const uint8_t* data() override
	{
		return encoder.getEncoded();
	}
	size_t size() override
	{
		return encoder.getEncodedSize();
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
	QOIEncoder encoder;
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
} // namespace impl::pfusik::qoici::cpp
