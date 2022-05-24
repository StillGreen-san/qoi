#pragma once

#include <cstdint>

#include <algorithm>
#include <array>
#include <exception> // TODO do without?
#include <iterator>
#include <vector>

// TODO constrain templates

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
	static constexpr std::array<uint8_t, constants::magicBytes> magic{'q', 'o', 'i', 'f'};
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
template<typename TResult, typename TInt>
inline TResult pushByte(TResult value, TInt byte)
{
	value <<= 8U;
	return value | byte;
}

template<typename TIterator>
inline uint32_t read32BE(TIterator itr)
{
	uint32_t value = 0;
	value = pushByte(value, *itr++);
	value = pushByte(value, *itr++);
	value = pushByte(value, *itr++);
	value = pushByte(value, *itr);
	return value;
}

template<typename TIterator>
inline void writeBE(TIterator itr, uint32_t value)
{
	*itr++ = static_cast<uint8_t>((value & 0xff000000U) >> 24U);
	*itr++ = static_cast<uint8_t>((value & 0x00ff0000U) >> 16U);
	*itr++ = static_cast<uint8_t>((value & 0x0000ff00U) >> 8U);
	*itr = static_cast<uint8_t>((value & 0x000000ffU));
}

inline size_t rawBufferSize(const Header& header)
{
	return static_cast<size_t>(header.width) * header.height * static_cast<size_t>(header.channels);
}
inline size_t qoiBufferSizeMin(const Header& header)
{
	return constants::headerSize + constants::endMarkerSize +
	       ((static_cast<size_t>(header.width) * header.height * static_cast<size_t>(header.channels)) /
	           constants::tagRunMaxLength);
}

struct Pixel
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
	[[nodiscard]] inline bool operator==(const Pixel rhs) const
	{
		return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
	}
	[[nodiscard]] inline bool operator!=(const Pixel rhs) const
	{
		return red != rhs.red || green != rhs.green || blue != rhs.blue || alpha != rhs.alpha;
	}
};

template<typename TContainer>
inline void push_back(DataPair<TContainer>& dataPair, Pixel pixel)
{
	dataPair.data.push_back(pixel.red);
	dataPair.data.push_back(pixel.green);
	dataPair.data.push_back(pixel.blue);
	if(dataPair.header.channels == Channels::RGBA)
	{
		dataPair.data.push_back(pixel.alpha);
	}
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

DataVector encode(const Header& header, const DataVector& rawData)
{
	DataVector data;
	data.reserve(helpers::qoiBufferSizeMin(header));

	data.insert(end(data), std::cbegin(Header::magic), std::cend(Header::magic));

	data.resize(data.size() + 4);
	auto dataIt = prev(end(data), 4);
	helpers::writeBE(dataIt, header.width);

	data.resize(data.size() + 4);
	dataIt = prev(end(data), 4);
	helpers::writeBE(dataIt, header.height);

	data.push_back(static_cast<uint8_t>(header.channels));
	data.push_back(static_cast<uint8_t>(header.colorspace));

	const auto rawEnd = cend(rawData);
	auto rawIt = cbegin(rawData);

	std::array<helpers::Pixel, constants::previousPixelsSize> previousPixels{};
	helpers::Pixel lastPixel{0, 0, 0, 255};

	for(; rawIt != rawEnd; advance(rawIt, static_cast<ptrdiff_t>(header.channels)))
	{
		const helpers::Pixel currentPixel{
		    rawIt[0], rawIt[1], rawIt[2], header.channels == Channels::RGBA ? rawIt[3] : lastPixel.alpha};

		if(currentPixel == lastPixel)
		{
			uint8_t runCount = 1;
			for(;
			    next(rawIt, static_cast<ptrdiff_t>(header.channels)) != rawEnd && runCount < constants::tagRunMaxLength;
			    ++runCount, advance(rawIt, static_cast<ptrdiff_t>(header.channels)))
			{
				auto rawNext = next(rawIt, static_cast<ptrdiff_t>(header.channels));
				const helpers::Pixel nextPixel{rawNext[0], rawNext[1], rawNext[2],
				    header.channels == Channels::RGBA ? rawNext[3] : lastPixel.alpha};
				if(nextPixel != currentPixel)
				{
					break;
				}
			}
			data.push_back(
			    constants::tagRun | static_cast<uint8_t>(static_cast<uint8_t>(~constants::tagMask2) & (runCount - 1U)));
			continue;
		}

		const size_t idx = helpers::index(currentPixel);
		if(previousPixels[idx] == currentPixel)
		{
			lastPixel = currentPixel;
			data.push_back(constants::tagIndex | static_cast<uint8_t>(static_cast<size_t>(~constants::tagMask2) & idx));
			continue;
		}

		if(lastPixel.alpha != currentPixel.alpha)
		{
			lastPixel = currentPixel;
			previousPixels[helpers::index(lastPixel)] = lastPixel;
			data.push_back(constants::tagRGBA);
			data.push_back(currentPixel.red);
			data.push_back(currentPixel.green);
			data.push_back(currentPixel.blue);
			data.push_back(currentPixel.alpha);
			continue;
		}

		const uint8_t diffGreen = currentPixel.green - lastPixel.green;
		const uint8_t diffRed = currentPixel.red - lastPixel.red;
		const uint8_t diffBlue = currentPixel.blue - lastPixel.blue;
		const uint8_t diffRedLuma = diffRed - diffGreen;
		const uint8_t diffBlueLuma = diffBlue - diffGreen;

		lastPixel = currentPixel;
		previousPixels[helpers::index(lastPixel)] = lastPixel;

		if(static_cast<uint8_t>(diffGreen + 32U) > 63 || static_cast<uint8_t>(diffRedLuma + 8U) > 15 ||
		    static_cast<uint8_t>(diffBlueLuma + 8U) > 15)
		{
			data.push_back(constants::tagRGB);
			data.push_back(currentPixel.red);
			data.push_back(currentPixel.green);
			data.push_back(currentPixel.blue);
			continue;
		}

		if(static_cast<uint8_t>(diffGreen + 2U) < 4 && static_cast<uint8_t>(diffRed + 2U) < 4 &&
		    static_cast<uint8_t>(diffBlue + 2U) < 4)
		{
			data.push_back(constants::tagDiff |
			               static_cast<uint8_t>((diffRed + 2U) << 4U | (diffGreen + 2U) << 2U | (diffBlue + 2U)));
			continue;
		}

		data.push_back(constants::tagLuma | static_cast<uint8_t>(diffGreen + 32U));
		data.push_back((diffRedLuma + 8U) << 4U | static_cast<uint8_t>(diffBlueLuma + 8U));
	}

	data.insert(end(data), 7, 0);
	data.insert(end(data), 1);

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
