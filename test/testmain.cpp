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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("helper")
{
	using sgs::qoi::helpers::pushByte;

	uint32_t value = 0;
	value = pushByte(value, 0x01);
	REQUIRE(value == 0x01);

	value = pushByte(value, 0x10);
	REQUIRE(value == 0x0110);

	value = pushByte(value, 0xff);
	REQUIRE(value == 0x0110ff);

	using sgs::qoi::helpers::isLittleEndian;
	using sgs::qoi::helpers::read32BE;
	std::array<uint8_t, 4> bytes{0xAA, 0xBB, 0xCC, 0xDD};

	REQUIRE(read32BE(bytes.begin()) == 0xAABBCCDD);
}

TEST_CASE("readHeader")
{
	using sgs::qoi::Header;
	using sgs::qoi::readHeader;
	TestData testData;

	CHECK(readHeader(testData.qoi.wikipedia) == testData.desc.wikipedia);
	CHECK(readHeader(testData.qoi.testcardalpha) == testData.desc.testcardalpha);
	CHECK(readHeader(testData.qoi.testcard) == testData.desc.testcard);
	CHECK(readHeader(testData.qoi.qoilogo) == testData.desc.qoilogo);
	CHECK(readHeader(testData.qoi.kodim23) == testData.desc.kodim23);
	CHECK(readHeader(testData.qoi.kodim10) == testData.desc.kodim10);
	CHECK(readHeader(testData.qoi.dice) == testData.desc.dice);
}
