#include "sgs/qoi.hpp"
#include "testdata.hpp"

#include <array>
#include <ostream>

bool operator==(const sgs::qoi::Header& sgsHeader, const impl::ImageDescription& testHeader) noexcept
{
	return sgsHeader.width == testHeader.width && sgsHeader.height == testHeader.height &&
	       static_cast<uint8_t>(sgsHeader.channels) == testHeader.channels &&
	       static_cast<uint8_t>(sgsHeader.colorspace) == testHeader.colorspace;
}
std::ostream& operator<<(std::ostream& oStrm, sgs::qoi::Header const& value)
{
	oStrm << '{' << value.width << ", " << value.height << ", " << static_cast<uint32_t>(value.channels) << ", "
	      << static_cast<uint32_t>(value.colorspace) << '}';
	return oStrm;
}
std::ostream& operator<<(std::ostream& oStrm, impl::ImageDescription const& value)
{
	oStrm << '{' << value.width << ", " << value.height << ", " << static_cast<uint32_t>(value.channels) << ", "
	      << static_cast<uint32_t>(value.colorspace) << '}';
	return oStrm;
}

namespace std
{
template<>
struct hash<std::vector<uint8_t>>
{
	typedef std::vector<uint8_t> argument_type;
	typedef std::size_t result_type;
	result_type operator()(argument_type const& s) const noexcept
	{
		return std::hash<std::string_view>{}(std::string_view{reinterpret_cast<const char*>(s.data()), s.size()});
	}
};
} // namespace std

std::ostream& operator<<(std::ostream& oStrm, const std::vector<uint8_t>& value)
{
	oStrm << "Hash: " << std::hex << std::hash<std::vector<uint8_t>>{}(value);
	return oStrm;
}

sgs::qoi::Header toHeader(const impl::ImageDescription& desc)
{
	return {desc.width, desc.height, static_cast<sgs::qoi::Channels>(desc.channels),
	    static_cast<sgs::qoi::Colorspace>(desc.colorspace)};
}

#include <catch2/catch.hpp>

TEST_CASE("helpers")
{
	SECTION("pushByte")
	{
		using sgs::qoi::helpers::pushByte;

		uint32_t value = 0;
		value = pushByte(value, 0x01);
		REQUIRE(value == 0x01);

		value = pushByte(value, 0x10);
		REQUIRE(value == 0x0110);

		value = pushByte(value, 0xff);
		REQUIRE(value == 0x0110ff);
	}

	SECTION("read32BE")
	{
		using sgs::qoi::helpers::read32BE;
		std::array<uint8_t, 4> bytes{0xAA, 0xBB, 0xCC, 0xDD};

		REQUIRE(read32BE(bytes.begin()) == 0xAABBCCDD);
	}

	SECTION("writeBE")
	{
		using sgs::qoi::helpers::writeBE;
		const std::array<uint8_t, 4> target{0xAA, 0xBB, 0xCC, 0xDD};
		std::array<uint8_t, 4> bytes{0, 0, 0, 0};

		writeBE(bytes.begin(), 0xAABBCCDD);

		REQUIRE(bytes == target);
	}
}

TEST_CASE("readHeader")
{
	using sgs::qoi::Header;
	using sgs::qoi::readHeader;
	const TestData& testData = TestData::getInstance();

	CHECK(readHeader(testData.qoi.wikipedia) == testData.desc.wikipedia);
	CHECK(readHeader(testData.qoi.testcardalpha) == testData.desc.testcardalpha);
	CHECK(readHeader(testData.qoi.testcard) == testData.desc.testcard);
	CHECK(readHeader(testData.qoi.qoilogo) == testData.desc.qoilogo);
	CHECK(readHeader(testData.qoi.kodim23) == testData.desc.kodim23);
	CHECK(readHeader(testData.qoi.kodim10) == testData.desc.kodim10);
	CHECK(readHeader(testData.qoi.dice) == testData.desc.dice);
}

TEST_CASE("decode")
{
	using sgs::qoi::decode;
	const TestData& testData = TestData::getInstance();

	{
		auto dataPair = decode(testData.qoi.kodim10);
		CHECK(dataPair.header == testData.desc.kodim10);
		CHECK(dataPair.data == testData.raw.kodim10);
	}
	{
		auto dataPair = decode(testData.qoi.dice);
		CHECK(dataPair.header == testData.desc.dice);
		CHECK(dataPair.data == testData.raw.dice);
	}
	{
		auto dataPair = decode(testData.qoi.testcardalpha);
		CHECK(dataPair.header == testData.desc.testcardalpha);
		CHECK(dataPair.data == testData.raw.testcardalpha);
	}
	{
		auto dataPair = decode(testData.qoi.testcard);
		CHECK(dataPair.header == testData.desc.testcard);
		CHECK(dataPair.data == testData.raw.testcard);
	}
	{
		auto dataPair = decode(testData.qoi.qoilogo);
		CHECK(dataPair.header == testData.desc.qoilogo);
		CHECK(dataPair.data == testData.raw.qoilogo);
	}
	{
		auto dataPair = decode(testData.qoi.wikipedia);
		CHECK(dataPair.header == testData.desc.wikipedia);
		CHECK(dataPair.data == testData.raw.wikipedia);
	}
	{
		auto dataPair = decode(testData.qoi.kodim23);
		CHECK(dataPair.header == testData.desc.kodim23);
		CHECK(dataPair.data == testData.raw.kodim23);
	}
}

TEST_CASE("encode")
{
	using sgs::qoi::encode;
	const TestData& testData = TestData::getInstance();

	{
		auto data = encode(toHeader(testData.desc.kodim10), testData.raw.kodim10);
		CHECK(data == testData.qoi.kodim10);
	}
	{
		auto data = encode(toHeader(testData.desc.dice), testData.raw.dice);
		CHECK(data == testData.qoi.dice);
	}
	{
		auto data = encode(toHeader(testData.desc.testcardalpha), testData.raw.testcardalpha);
		CHECK(data == testData.qoi.testcardalpha);
	}
	{
		auto data = encode(toHeader(testData.desc.testcard), testData.raw.testcard);
		CHECK(data == testData.qoi.testcard);
	}
	{
		auto data = encode(toHeader(testData.desc.qoilogo), testData.raw.qoilogo);
		CHECK(data == testData.qoi.qoilogo);
	}
	{
		auto data = encode(toHeader(testData.desc.wikipedia), testData.raw.wikipedia);
		CHECK(data == testData.qoi.wikipedia);
	}
	{
		auto data = encode(toHeader(testData.desc.kodim23), testData.raw.kodim23);
		CHECK(data == testData.qoi.kodim23);
	}
}
