#include "qoiimpls.hpp"

#include <qoi.hpp>

namespace impl::shadowmitia::libqoi
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	explicit Decoder(const std::vector<uint8_t>& qoi) : pixels{qoi::decode(qoi)}, header{qoi::get_header(qoi)}
	{
	}
	const uint8_t* data() override
	{
		return pixels.data();
	}
	size_t size() override
	{
		return pixels.size();
	}
	uint32_t width() override
	{
		return header.width;
	}
	uint32_t height() override
	{
		return header.height;
	}
	uint8_t channels() override
	{
		return header.channels;
	}
	uint8_t colorspace() override
	{
		return header.colorspace;
	}
	std::vector<uint8_t> pixels;
	qoi::header header;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription desc) :
	    pixels{qoi::encode(raw, desc.width, desc.height, desc.channels)}, header{qoi::get_header(pixels)}
	{
	}
	const uint8_t* data() override
	{
		return pixels.data();
	}
	size_t size() override
	{
		return pixels.size();
	}
	uint32_t width() override
	{
		return header.width;
	}
	uint32_t height() override
	{
		return header.height;
	}
	uint8_t channels() override
	{
		return header.channels;
	}
	uint8_t colorspace() override
	{
		return header.colorspace;
	}
	std::vector<uint8_t> pixels;
	qoi::header header;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}

std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::shadowmitia::libqoi
