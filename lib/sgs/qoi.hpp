#pragma once

#include <cstdint>

#include <algorithm>
#include <array>
#include <exception> // TODO do without?
#include <iterator>
#include <vector>

// TODO constrain templates
// TODO optimizations for continues storage

namespace sgs::qoi
{
namespace constants
{
constexpr size_t headerSize = 14;
constexpr size_t endMarkerSize = 8;
constexpr size_t magicBytes = 4;
constexpr size_t previousPixelsSize = 64;
constexpr uint8_t tagRGB = 0b11111110;
constexpr uint8_t tagRGBA = 0b11111111;
constexpr uint8_t tagMask2 = 0b11000000;
constexpr uint8_t tagIndex = 0b00000000;
constexpr uint8_t tagDiff = 0b01000000;
constexpr uint8_t tagLuma = 0b10000000;
constexpr uint8_t tagRun = 0b11000000;
constexpr size_t tagRunMaxLength = 62;
} // namespace constants

enum class Channels : uint8_t
{
	RGB = 3,
	RGBA = 4
};
enum class Colorspace : uint8_t
{
	sRGB = 0,
	Linear = 1
};
struct Header
{
	static constexpr uint8_t magic[constants::magicBytes]{'q', 'o', 'i', 'f'};
	uint32_t width{0};
	uint32_t height{0};
	Channels channels{Channels::RGBA};
	Colorspace colorspace{Colorspace::Linear};
};
template<typename TData>
struct DataPair
{
	Header header;
	TData data;
};

namespace helpers
{
#ifdef CMAKE_CXX_BIG_ENDIAN
constexpr bool isLittleEndian = false;
#else
constexpr bool isLittleEndian = true;
#endif

template<typename TResult, typename TInt>
inline TResult pushByte(TResult value, TInt byte)
{
	value <<= 8U;
	return value | byte;
}

template<typename TIterator>
inline uint32_t read32BE(TIterator it)
{
	if constexpr(isLittleEndian)
	{
		uint32_t value = 0;
		value = pushByte(value, *it++);
		value = pushByte(value, *it++);
		value = pushByte(value, *it++);
		value = pushByte(value, *it);
		return value;
	}
	else
	{
		return *reinterpret_cast<const uint32_t*>(&*it);
	}
}

inline size_t rawBufferSize(const Header& header)
{
	return header.width * header.height * static_cast<size_t>(header.channels);
}
inline size_t qoiBufferSizeMin(const Header& header)
{
	return constants::headerSize + constants::endMarkerSize +
	       ((header.width * header.height * static_cast<size_t>(header.channels)) / constants::tagRunMaxLength);
}

struct Pixel
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

template<typename TContainer>
inline void push_back(DataPair<TContainer>& dataPair, Pixel pixel)
{
	dataPair.data.insert(end(dataPair.data), &pixel.red, &pixel.red + static_cast<size_t>(dataPair.header.channels));
}
template<typename TContainer>
inline void push_back(DataPair<TContainer>& dataPair, Pixel pixel, size_t count)
{
	for(; count != 0; --count)
	{
		push_back(dataPair, pixel);
	}
}

inline size_t index(Pixel pixel)
{
	return (pixel.red * 3U + pixel.green * 5U + pixel.blue * 7U + pixel.alpha * 11U) & 0b111111U;
}
} // namespace helpers

using DataVector = std::vector<uint8_t>; // TODO make templates and remove direct vector dependency

Header readHeader(const DataVector& qoiData)
{
	if(qoiData.size() < constants::headerSize)
	{
		throw std::exception{"insufficient data"};
	}

	auto qoiIt = next(cbegin(qoiData), constants::magicBytes);

	if(!std::equal(cbegin(qoiData), qoiIt, std::cbegin(Header::magic)))
	{
		throw std::exception{"magic bytes mismatch"};
	}

	Header header;

	header.width = helpers::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.height = helpers::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.channels = static_cast<Channels>(*qoiIt);
	std::advance(qoiIt, 1);

	header.colorspace = static_cast<Colorspace>(*qoiIt);

	return header;
}

DataPair<DataVector> decode(const DataVector& qoiData)
{
	if(qoiData.size() < constants::headerSize + constants::endMarkerSize)
	{
		throw std::exception{"insufficient data"};
	}

	DataPair<DataVector> dataPair{readHeader(qoiData)};
	dataPair.data.reserve(helpers::rawBufferSize(dataPair.header));

	const auto qoiEnd = std::prev(cend(qoiData), constants::endMarkerSize);
	auto qoiIt = std::next(cbegin(qoiData), constants::headerSize);

	std::array<helpers::Pixel, constants::previousPixelsSize> previousPixels{};
	helpers::Pixel lastPixel{0, 0, 0, 255};

	// TODO handle corrupted data (chunks of wrong size)
	for(size_t target = dataPair.data.capacity(); qoiIt != qoiEnd && dataPair.data.size() < target;)
	{
		if(qoiIt[0] == constants::tagRGB)
		{
			lastPixel = helpers::Pixel{qoiIt[1], qoiIt[2], qoiIt[3], lastPixel.alpha};
			helpers::push_back(dataPair, lastPixel);
			previousPixels[helpers::index(lastPixel)] = lastPixel;
			advance(qoiIt, 4);
			continue;
		}
		if(qoiIt[0] == constants::tagRGBA)
		{
			lastPixel = helpers::Pixel{qoiIt[1], qoiIt[2], qoiIt[3], qoiIt[4]};
			helpers::push_back(dataPair, lastPixel);
			previousPixels[helpers::index(lastPixel)] = lastPixel;
			advance(qoiIt, 5);
			continue;
		}
		switch(qoiIt[0] & constants::tagMask2)
		{
		case constants::tagIndex:
			lastPixel = previousPixels[qoiIt[0]];
			helpers::push_back(dataPair, lastPixel);
			advance(qoiIt, 1);
			break;
		case constants::tagDiff:
			lastPixel = helpers::Pixel{static_cast<uint8_t>(lastPixel.red + (((qoiIt[0] & 0b00110000U) >> 4U) - 2U)),
			    static_cast<uint8_t>(lastPixel.green + (((qoiIt[0] & 0b00001100U) >> 2U) - 2U)),
			    static_cast<uint8_t>(lastPixel.blue + ((qoiIt[0] & 0b00000011U) - 2U)), lastPixel.alpha};
			helpers::push_back(dataPair, lastPixel);
			previousPixels[helpers::index(lastPixel)] = lastPixel;
			advance(qoiIt, 1);
			break;
		case constants::tagLuma:
		{
			const uint8_t greenDiff = (qoiIt[0] & static_cast<uint8_t>(~constants::tagMask2)) - 32U;
			lastPixel = helpers::Pixel{
			    static_cast<uint8_t>(lastPixel.red + (((qoiIt[1] & 0b11110000U) >> 4U) - 8U) + greenDiff),
			    static_cast<uint8_t>(lastPixel.green + greenDiff),
			    static_cast<uint8_t>(lastPixel.blue + ((qoiIt[1] & 0b00001111U) - 8U) + greenDiff), lastPixel.alpha};
			helpers::push_back(dataPair, lastPixel);
			previousPixels[helpers::index(lastPixel)] = lastPixel;
			advance(qoiIt, 2);
			break;
		}
		case constants::tagRun:
			helpers::push_back(dataPair, lastPixel, (qoiIt[0] & static_cast<uint8_t>(~constants::tagMask2)) + 1);
			advance(qoiIt, 1);
			break;
		default:
			throw std::exception{"unknown chunk"};
		}
	}

	return dataPair;
}

DataVector encode([[maybe_unused]] const Header& header, [[maybe_unused]] const DataVector& rawData)
{
	DataVector data;
//	data.reserve(helpers::qoiBufferSizeMin(header));

//	const auto rawEnd = cend(rawData);
//	auto rawIt = cbegin(rawData);

//	std::array<helpers::Pixel, constants::previousPixelsSize> previousPixels{};
//	helpers::Pixel lastPixel{0, 0, 0, 255};

	return data;
}

// Header readHeader(const TData& qoiData);
// TDataPair<TData> decode(const TData& qoiData);
// DataPair<TContainer> decode(const TData& qoiData);
// Header decode(const TData& qoiData, TData& rawData);
// TData encode(const TDataPair<TData>& rawData);
// TData encode(const Header& header, const TData& rawData);
// TContainer encode(const DataPair<TData>& rawData);
// TContainer encode(const Header& header, const TData& rawData);
} // namespace sgs::qoi
