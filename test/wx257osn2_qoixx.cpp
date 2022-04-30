#include "qoiimpls.hpp"

#include <qoixx.hpp>

namespace impl::wx257osn2::qoixx
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi) : pixels{::qoixx::qoi::decode<std::vector<uint8_t>>(qoi)}
	{
	}
	const uint8_t* data() override
	{
		return pixels.first.data();
	}
	size_t size() override
	{
		return pixels.first.size();
	}
	uint32_t width() override
	{
		return pixels.second.width;
	}
	uint32_t height() override
	{
		return pixels.second.height;
	}
	uint8_t channels() override
	{
		return pixels.second.channels;
	}
	uint8_t colorspace() override
	{
		return static_cast<uint8_t>(pixels.second.colorspace);
	}
	std::pair<std::vector<uint8_t>, ::qoixx::qoi::desc> pixels;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, ImageDescription description) : header{description}
	{
		::qoixx::qoi::desc desc{description.width, description.height, description.channels,
		    static_cast<::qoixx::qoi::colorspace>(description.channels)};
		pixels = ::qoixx::qoi::encode<std::vector<std::uint8_t>>(raw, desc);
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
	ImageDescription header;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}

std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::wx257osn2::qoixx