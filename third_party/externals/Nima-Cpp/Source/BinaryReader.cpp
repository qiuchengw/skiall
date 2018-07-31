#include "BinaryReader.hpp"
#include "Exceptions/OverflowException.hpp"

using namespace nima;

static bool isBigEndian(void)
{
	union 
	{
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1; 
}

BinaryReader::BinaryReader(unsigned char* data, unsigned int length, Endian endianness) :
	m_Data(data),
	m_DataLength(length),
	m_DataPosition(0),
	m_SwapEndianness(isBigEndian() != (endianness == BigEndian))
{
}

unsigned char BinaryReader::readByte()
{
	if(m_DataPosition+1 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readByte.");
	}

	unsigned char byte = m_Data[m_DataPosition];
	m_DataPosition += 1;
	return byte;
}

unsigned int BinaryReader::read(unsigned char* bytes, unsigned int length)
{
	unsigned int readCount = 0;
	for(int i = 0; i < length && m_DataPosition < m_DataLength; i++)
	{
		bytes[i] = m_Data[m_DataPosition];
		m_DataPosition++;
		readCount++;
	}
	return readCount;
}

void BinaryReader::readFloatArray(float* numbers, unsigned int length)
{
	if(m_DataPosition+4*length > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readFloatArray.");
	}
	for(int i = 0; i < length; i++)
	{
		unsigned char* bytes = &m_Data[m_DataPosition];
		m_DataPosition += 4;

		if(m_SwapEndianness)
		{
			unsigned char sbytes[4] = { bytes[3], bytes[2], bytes[1], bytes[0] };
			*numbers = *reinterpret_cast<float*>(sbytes);
		}
		else
		{
			*numbers = *reinterpret_cast<float*>(bytes);
		}
		numbers++;
	}
}

void BinaryReader::readUnsignedShortArray(unsigned short* numbers, unsigned int length)
{
	if(m_DataPosition+2*length > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readUnsignedShortArray.");
	}
	for(int i = 0; i < length; i++)
	{
		unsigned char* bytes = &m_Data[m_DataPosition];
		m_DataPosition += 2;

		if(m_SwapEndianness)
		{
			unsigned char sbytes[2] = { bytes[1], bytes[0] };
			*numbers = *reinterpret_cast<unsigned short*>(sbytes);
		}
		else
		{
			*numbers = *reinterpret_cast<unsigned short*>(bytes);
		}
		numbers++;
	}
}

int BinaryReader::readInt()
{
	if(m_DataPosition+4 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readInt.");
	}

	unsigned char* bytes = &m_Data[m_DataPosition];
	m_DataPosition += 4;
	
	if(m_SwapEndianness)
	{
		unsigned char sbytes[4] = { bytes[3], bytes[2], bytes[1], bytes[0] };
		return *reinterpret_cast<int*>(sbytes);
	}
	return *reinterpret_cast<int*>(bytes);
}

unsigned int BinaryReader::readUnsignedInt()
{
	if(m_DataPosition+4 > m_DataLength)
    {
        throw OverflowException("Attempt to read past end of data in BinaryReader::readUnsignedInt.");
    }
    unsigned char* bytes = &m_Data[m_DataPosition];
    m_DataPosition += 4;
    
    if(m_SwapEndianness)
    {
        unsigned char sbytes[4] = { bytes[3], bytes[2], bytes[1], bytes[0] };
        return *reinterpret_cast<unsigned int*>(sbytes);
    }
    return *reinterpret_cast<unsigned int*>(bytes);
}

float BinaryReader::readFloat()
{
	if(m_DataPosition+4 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readFloat.");
	}
	unsigned char* bytes = &m_Data[m_DataPosition];
	m_DataPosition += 4;

	if(m_SwapEndianness)
	{
		unsigned char sbytes[4] = { bytes[3], bytes[2], bytes[1], bytes[0] };
		return *reinterpret_cast<float*>(sbytes);
	}
	return *reinterpret_cast<float*>(bytes);

	return 0.0f;

}

double BinaryReader::readDouble()
{
	if(m_DataPosition+8 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readDouble.");
	}
	unsigned char* bytes = &m_Data[m_DataPosition];
	m_DataPosition += 8;

	if(m_SwapEndianness)
	{
		unsigned char sbytes[8] = { bytes[7], bytes[6], bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0] };
		return *reinterpret_cast<double*>(sbytes);
	}
	return *reinterpret_cast<double*>(bytes);

	return 0.0;

}

short BinaryReader::readShort()
{
	if(m_DataPosition+2 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readShort.");
	}
	unsigned char* bytes = &m_Data[m_DataPosition];
	m_DataPosition += 2;

	if(m_SwapEndianness)
	{
		unsigned char sbytes[2] = { bytes[1], bytes[0] };
		return *reinterpret_cast<short*>(sbytes);
	}
	return *reinterpret_cast<short*>(bytes);
}

unsigned short BinaryReader::readUnsignedShort()
{
	if(m_DataPosition+2 > m_DataLength)
	{
		throw OverflowException("Attempt to read past end of data in BinaryReader::readUnsignedShort.");
	}
	unsigned char* bytes = &m_Data[m_DataPosition];
	m_DataPosition += 2;

	if(m_SwapEndianness)
	{
		unsigned char sbytes[2] = { bytes[1], bytes[0] };
		return *reinterpret_cast<unsigned short*>(sbytes);
	}
	return *reinterpret_cast<unsigned short*>(bytes);
}

std::string BinaryReader::readString()
{
	unsigned int length = readUnsignedInt();
	std::string str(length, 0);

	if(read((unsigned char*)str.c_str(), length) == length)
	{
		return str;
	}

	return std::string();
}

unsigned long BinaryReader::readUnsignedLong()
{
	if(m_DataPosition+4 > m_DataLength)
    {
        throw OverflowException("Attempt to read past end of data in BinaryReader::readUnsignedLong.");
    }
    unsigned char* bytes = &m_Data[m_DataPosition];
    m_DataPosition += 8;
    
    if(m_SwapEndianness)
    {
        unsigned char sbytes[8] = { bytes[7], bytes[6], bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0] };
        return *reinterpret_cast<unsigned long*>(sbytes);
    }
    return *reinterpret_cast<unsigned long*>(bytes);
}