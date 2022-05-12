#pragma once

#include <memory>
#include <vector>

namespace impl
{
struct IImageData
{
	virtual const uint8_t* data() = 0;
	virtual size_t size() = 0;
	virtual uint32_t width() = 0;
	virtual uint32_t height() = 0;
	virtual uint8_t channels() = 0;
	virtual uint8_t colorspace() = 0;
	virtual ~IImageData() = default;
};
struct ImageDescription
{
	uint32_t width;
	uint32_t height;
	uint8_t channels;
	uint8_t colorspace;
};
inline bool alpha(uint8_t channels)
{
	return channels == 4;
}
inline bool linear(uint8_t colorspace)
{
	return colorspace == 1;
}
inline uint8_t channels(bool alpha)
{
	return alpha ? 4 : 3;
}
inline uint8_t colorspace(bool linear)
{
	return linear ? 1 : 0;
}

namespace stillgreensan::qoi
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
//std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace stillgreensan::qoi

namespace pfusik::qoici
{
namespace c
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace c
namespace cpp
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace cpp
} // namespace pfusik::qoici

namespace phoboslab::qoi
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace phoboslab::qoi

namespace shadowmitia::libqoi
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace shadowmitia::libqoi

//namespace shraiwi::miniqoi
//{
//namespace c
//{
//std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
//} // namespace c
//namespace cpp
//{
//std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
//} // namespace cpp
//} // namespace shraiwi::miniqoi

namespace wx257osn2::qoixx
{
std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi);
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc);
} // namespace wx257osn2::qoixx
} // namespace impl
