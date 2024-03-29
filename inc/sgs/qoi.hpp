#pragma once

#include <cstdint>

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <vector>

// TODO constrain templates
// TODO add noexcept where possible

namespace sgs::qoi
{
namespace detail
{
constexpr size_t headerSize = 14;
constexpr size_t endMarkerSize = 8;
constexpr size_t magicBytes = 4;
constexpr size_t previousPixelsSize = 64;
constexpr uint8_t tagRGB = 0b11111110;
constexpr uint8_t tagRGBA = 0b11111111;
constexpr uint8_t tagMask2 = 0b11000000;
constexpr uint8_t dataMask2 = 0b00111111;
constexpr uint8_t tagIndex = 0b00000000;
constexpr uint8_t tagDiff = 0b01000000;
constexpr uint8_t tagLuma = 0b10000000;
constexpr uint8_t tagRun = 0b11000000;
constexpr size_t tagRunMaxLength = 62;
} // namespace detail

/**
 * @brief the amount of channels used (can be casted to their numeric value)
 */
enum class Channels : uint8_t
{
	RGB = 3,
	RGBA = 4
};

/**
 * @brief the colorspace used (does not change de/encoding)
 */
enum class Colorspace : uint8_t
{
	sRGB = 0,
	Linear = 1
};

/**
 * @brief holds basic information about the picture
 */
struct Header
{
	static constexpr std::array<uint8_t, detail::magicBytes> magic{'q', 'o', 'i', 'f'};
	uint32_t width{0};
	uint32_t height{0};
	Channels channels{Channels::RGBA};
	Colorspace colorspace{Colorspace::Linear};
};

/**
 * @brief pair type holding Header & picture data
 * @tparam TData container type to store picture data
 */
template<typename TData>
struct DataPair
{
	Header header;
	TData data;
};

namespace detail
{
template<typename TResult, typename TInt>
inline TResult pushByte(TResult value, TInt byte)
{
	value <<= 8U;
	return value | static_cast<TResult>(byte);
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
	return detail::headerSize + detail::endMarkerSize +
	       ((static_cast<size_t>(header.width) * header.height * static_cast<size_t>(header.channels)) /
	           detail::tagRunMaxLength);
}

struct Pixel
{
	Pixel() = default;
	Pixel(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_) // NOLINT(bugprone-easily-swappable-parameters)
	    : red{red_}, green{green_}, blue{blue_}, alpha{alpha_}
	{
	}
	template<typename TIter>
	explicit Pixel(TIter iter) :
	    red{static_cast<uint8_t>(*iter++)}, green{static_cast<uint8_t>(*iter++)}, blue{static_cast<uint8_t>(*iter++)},
	    alpha{static_cast<uint8_t>(*iter)}
	{
	}
	template<typename TIter>
	Pixel(TIter iter, uint8_t alpha_) :
	    red{static_cast<uint8_t>(*iter++)}, green{static_cast<uint8_t>(*iter++)}, blue{static_cast<uint8_t>(*iter)},
	    alpha{alpha_}
	{
	}
	template<typename TIter>
	Pixel(TIter iter, uint8_t alpha_, const Header& header) :
	    red{static_cast<uint8_t>(*iter++)}, green{static_cast<uint8_t>(*iter++)}, blue{static_cast<uint8_t>(*iter++)},
	    alpha{header.channels == Channels::RGBA ? static_cast<uint8_t>(*iter) : alpha_}
	{
	}
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t alpha = 0;
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

template<typename TContainer>
inline auto reserve_impl(TContainer& container, size_t size) -> decltype(container.reserve(size), void())
{
	container.reserve(size);
}
template<typename TContainer>
inline void reserve_impl([[maybe_unused]] const TContainer& container, [[maybe_unused]] size_t size)
{
}
template<typename TContainer>
void reserve(TContainer& container, size_t size)
{
	reserve_impl(container, size);
}
} // namespace detail

/**
 * @brief tries to read header from QOI file data
 * @tparam TContainer container type
 * @param qoiData the QOI file data (passing only the header bytes is sufficient)
 * @return Header type
 * @throws std::runtime_error on insufficient data or magic bytes mismatch
 */
template<typename TContainer>
Header readHeader(const TContainer& qoiData)
{
	if(qoiData.size() < detail::headerSize)
	{
		throw std::runtime_error{"insufficient data"};
	}

	auto qoiIt = next(cbegin(qoiData), detail::magicBytes);

	if(!std::equal(cbegin(qoiData), qoiIt, std::cbegin(Header::magic)))
	{
		throw std::runtime_error{"magic bytes mismatch"};
	}

	Header header;

	header.width = detail::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.height = detail::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.channels = static_cast<Channels>(*qoiIt);
	std::advance(qoiIt, 1);

	header.colorspace = static_cast<Colorspace>(*qoiIt);

	return header;
}

/**
 * @brief tries to decode QOI file data
 * @tparam TOutContainer container type for output
 * @tparam TInContainer container type for input
 * @param qoiData container with QOI file data (header, packets & endmarker)
 * @return DataPair with Header & raw picture data
 * @throws std::runtime_error on insufficient data or header error
 */
template<typename TOutContainer, typename TInContainer>
DataPair<TOutContainer> decode(const TInContainer& qoiData)
{
	if(qoiData.size() < detail::headerSize + detail::endMarkerSize)
	{
		throw std::runtime_error{"insufficient data"};
	}

	DataPair<TOutContainer> dataPair{readHeader(qoiData), {}};
	const size_t reservedSize = detail::rawBufferSize(dataPair.header);
	detail::reserve(dataPair.data, reservedSize);

	const auto qoiEnd = std::prev(cend(qoiData), detail::endMarkerSize);
	auto qoiIt = std::next(cbegin(qoiData), detail::headerSize);

	std::array<detail::Pixel, detail::previousPixelsSize> previousPixels{};
	detail::Pixel lastPixel(0, 0, 0, 255);

	// TODO handle corrupted data (chunks of wrong size)
	for(const size_t target = reservedSize; qoiIt != qoiEnd && dataPair.data.size() < target;)
	{
		if(*qoiIt == detail::tagRGB)
		{
			lastPixel = detail::Pixel(std::next(qoiIt), lastPixel.alpha);
			detail::push_back(dataPair, lastPixel);
			previousPixels[detail::index(lastPixel)] = lastPixel;
			advance(qoiIt, 4);
			continue;
		}
		if(*qoiIt == detail::tagRGBA)
		{
			lastPixel = detail::Pixel(std::next(qoiIt));
			detail::push_back(dataPair, lastPixel);
			previousPixels[detail::index(lastPixel)] = lastPixel;
			advance(qoiIt, 5);
			continue;
		}
		switch(*qoiIt & detail::tagMask2)
		{
		case detail::tagIndex:
			lastPixel = previousPixels[*qoiIt];
			detail::push_back(dataPair, lastPixel);
			advance(qoiIt, 1);
			break;
		case detail::tagDiff:
			lastPixel = detail::Pixel(static_cast<uint8_t>(lastPixel.red + (((*qoiIt & 0b00110000U) >> 4U) - 2U)),
			    static_cast<uint8_t>(lastPixel.green + (((*qoiIt & 0b00001100U) >> 2U) - 2U)),
			    static_cast<uint8_t>(lastPixel.blue + ((*qoiIt & 0b00000011U) - 2U)), lastPixel.alpha);
			detail::push_back(dataPair, lastPixel);
			previousPixels[detail::index(lastPixel)] = lastPixel;
			advance(qoiIt, 1);
			break;
		case detail::tagLuma:
		{
			const auto greenDiff = static_cast<uint8_t>((*qoiIt & detail::dataMask2) - 32U);
			const auto qoiNext = static_cast<uint8_t>(*std::next(qoiIt));
			lastPixel =
			    detail::Pixel(static_cast<uint8_t>(lastPixel.red + (((qoiNext & 0b11110000U) >> 4U) - 8U) + greenDiff),
			        static_cast<uint8_t>(lastPixel.green + greenDiff),
			        static_cast<uint8_t>(lastPixel.blue + ((qoiNext & 0b00001111U) - 8U) + greenDiff), lastPixel.alpha);
			detail::push_back(dataPair, lastPixel);
			previousPixels[detail::index(lastPixel)] = lastPixel;
			advance(qoiIt, 2);
			break;
		}
		case detail::tagRun:
			detail::push_back(dataPair, lastPixel, (*qoiIt & detail::dataMask2) + 1);
			advance(qoiIt, 1);
			break;
		default:
			throw std::runtime_error{"unknown chunk"};
		}
	}

	return dataPair;
}

/**
 * @brief tries to decode QOI file data
 * @tparam TContainer container type for in&output
 * @param qoiData container with QOI file data (header, packets & endmarker)
 * @return DataPair with Header & raw picture data
 * @throws std::runtime_error on insufficient data or header error
 */
template<typename TContainer>
inline DataPair<TContainer> decode(const TContainer& qoiData)
{
	return decode<TContainer, TContainer>(qoiData);
}

/**
 * @brief tries to encode raw picture data
 * @tparam TOutContainer container type for output
 * @tparam TInContainer container type for input
 * @param header basic information about the picture
 * @param rawData raw picture data
 * @return TOutContainer encoded QOI file data
 */
template<typename TOutContainer, typename TInContainer>
TOutContainer encode(const Header& header, const TInContainer& rawData)
{
	if(rawData.size() < detail::rawBufferSize(header))
	{
		throw std::runtime_error{"insufficient data"};
	}

	TOutContainer data;
	detail::reserve(data, detail::qoiBufferSizeMin(header));

	data.insert(end(data), std::cbegin(Header::magic), std::cend(Header::magic));

	data.resize(data.size() + 4);
	auto dataIt = prev(end(data), 4);
	detail::writeBE(dataIt, header.width);

	data.resize(data.size() + 4);
	dataIt = prev(end(data), 4);
	detail::writeBE(dataIt, header.height);

	data.push_back(static_cast<uint8_t>(header.channels));
	data.push_back(static_cast<uint8_t>(header.colorspace));

	const auto rawEnd = cend(rawData);
	auto rawIt = cbegin(rawData);

	std::array<detail::Pixel, detail::previousPixelsSize> previousPixels{};
	detail::Pixel lastPixel(0, 0, 0, 255);

	for(; rawIt != rawEnd; advance(rawIt, static_cast<ptrdiff_t>(header.channels)))
	{
		const detail::Pixel currentPixel(rawIt, lastPixel.alpha, header);

		if(currentPixel == lastPixel)
		{
			uint8_t runCount = 1;
			for(; next(rawIt, static_cast<ptrdiff_t>(header.channels)) != rawEnd && runCount < detail::tagRunMaxLength;
			    ++runCount, advance(rawIt, static_cast<ptrdiff_t>(header.channels)))
			{
				auto rawNext = next(rawIt, static_cast<ptrdiff_t>(header.channels));
				const detail::Pixel nextPixel(rawNext, lastPixel.alpha, header);
				if(nextPixel != currentPixel)
				{
					break;
				}
			}
			data.push_back(detail::tagRun | static_cast<uint8_t>(detail::dataMask2 & (runCount - 1U)));
			continue;
		}

		const size_t idx = detail::index(currentPixel);
		if(previousPixels[idx] == currentPixel)
		{
			lastPixel = currentPixel;
			data.push_back(detail::tagIndex | static_cast<uint8_t>(detail::dataMask2 & idx));
			continue;
		}

		if(lastPixel.alpha != currentPixel.alpha)
		{
			lastPixel = currentPixel;
			previousPixels[detail::index(lastPixel)] = lastPixel;
			data.push_back(detail::tagRGBA);
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
		previousPixels[detail::index(lastPixel)] = lastPixel;

		if(static_cast<uint8_t>(diffGreen + 32U) > 63 || static_cast<uint8_t>(diffRedLuma + 8U) > 15 ||
		    static_cast<uint8_t>(diffBlueLuma + 8U) > 15)
		{
			data.push_back(detail::tagRGB);
			data.push_back(currentPixel.red);
			data.push_back(currentPixel.green);
			data.push_back(currentPixel.blue);
			continue;
		}

		if(static_cast<uint8_t>(diffGreen + 2U) < 4 && static_cast<uint8_t>(diffRed + 2U) < 4 &&
		    static_cast<uint8_t>(diffBlue + 2U) < 4)
		{
			data.push_back(detail::tagDiff |
			               static_cast<uint8_t>((diffRed + 2U) << 4U | (diffGreen + 2U) << 2U | (diffBlue + 2U)));
			continue;
		}

		data.push_back(detail::tagLuma | static_cast<uint8_t>(diffGreen + 32U));
		data.push_back(static_cast<uint8_t>((diffRedLuma + 8U) << 4U) | static_cast<uint8_t>(diffBlueLuma + 8U));
	}

	data.insert(end(data), 7, 0);
	data.insert(end(data), 1);

	return data;
}

/**
 * @brief tries to encode raw picture data
 * @tparam TContainer container type for in&output
 * @param header basic information about the picture
 * @param rawData raw picture data
 * @return TOutContainer encoded QOI file data
 */
template<typename TContainer> // TODO resolve ambiguity for de/encode<vector<T>>() called with vector<T>
inline TContainer encode(const Header& header, const TContainer& rawData)
{
	return encode<TContainer, TContainer>(header, rawData);
}
} // namespace sgs::qoi
