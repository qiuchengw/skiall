#ifndef _NIMA_BINARYREADER_HPP_
#define _NIMA_BINARYREADER_HPP_

#include <string>

namespace nima
{
	class BinaryReader
	{
		public:
			enum Endian
			{
				BigEndian,
				LittleEndian
			};

		protected:
			unsigned char* m_Data;
			unsigned int m_DataLength;
			unsigned int m_DataPosition;
			bool m_SwapEndianness;

		public:
			BinaryReader(unsigned char* data, unsigned int length, Endian endianness = LittleEndian);
			
			unsigned char readByte();
			unsigned int read(unsigned char* bytes, unsigned int length);
			void readFloatArray(float* numbers, unsigned int length);
			void readUnsignedShortArray(unsigned short* numbers, unsigned int length);
			int readInt();
			unsigned int readUnsignedInt();
			float readFloat();
			double readDouble();
			short readShort();
			unsigned short readUnsignedShort();
			std::string readString();
			unsigned long readUnsignedLong();
	};
}
#endif