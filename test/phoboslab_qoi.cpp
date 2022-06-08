#include "qoiimpls.hpp"

#include <qoi.h>

#include <cstdlib>

namespace impl::phoboslab::qoi
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	explicit Decoder(const std::vector<uint8_t>& qoi) :
	    pixels{::qoi_decode(qoi.data(), static_cast<int>(qoi.size()), &description, 0)}
	{
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(pixels);
	}
	size_t size() override
	{
		return static_cast<size_t>(width()) * height() * channels();
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
		free(pixels); // NOLINT(cppcoreguidelines-owning-memory,cppcoreguidelines-no-malloc,hicpp-no-malloc)
	}
	::qoi_desc description{};
	void* pixels;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription desc) :
	    description{desc.width, desc.height, desc.channels, desc.colorspace}
	{
		pixels = ::qoi_encode(raw.data(), &description, &length);
	}
	const uint8_t* data() override
	{
		return reinterpret_cast<const uint8_t*>(pixels);
	}
	size_t size() override
	{
		return static_cast<size_t>(length);
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
	int length{};
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}

std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::phoboslab::qoi
