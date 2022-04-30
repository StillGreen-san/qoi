#include "qoiimpls.hpp"

#include <qoi.h>

namespace impl::phoboslab
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi) : pixels{::qoi_decode(qoi.data(), qoi.size(), &description, 0)}
	{
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(pixels);
	}
	size_t size() override
	{
		return width() * height() * channels();
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
	~Decoder() override
	{
		free(pixels);
	}
	::qoi_desc description;
	void* pixels;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription desc)
	{
		description.width = desc.width;
		description.height = desc.height;
		description.channels = desc.channels;
		description.colorspace = desc.colorspace;
		pixels = ::qoi_encode(raw.data(), &description, &length);
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(pixels);
	}
	size_t size() override
	{
		return length;
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
	::qoi_desc description;
	void* pixels;
	int length;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}

std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::phoboslab
