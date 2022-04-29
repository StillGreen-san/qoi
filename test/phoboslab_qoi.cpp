
#include "qoiimpls.hpp"

//#define QOI_IMPLEMENTATION
#include <qoi.h>

#include <memory>

void encode()
{
	::qoi_desc desc;
	int size;
	void* data = ::qoi_encode(nullptr, &desc, &size);
	free(data);
}
void decode()
{
	::qoi_desc desc;
	void* data = ::qoi_decode(nullptr, 0, &desc, 4);
	free(data);
}
