#ifndef _NIMA_BLOCKREADER_HPP_
#define _NIMA_BLOCKREADER_HPP_

#include <string>
#include "BinaryReader.hpp"

namespace nima
{
	class Mat2D;
	class Vec2D;

	class BlockReader;

	class BlockReader : public BinaryReader
	{
		private:
			int m_BlockType;

		public:
			BlockReader(unsigned char* data, unsigned int length);
			BlockReader(int type, unsigned char* data, unsigned int length);
			BlockReader* readNextBlock();

			template<typename T>
			T blockType() const { return static_cast<T>(m_BlockType); }
			int blockType() const { return m_BlockType; }
			void read(Mat2D& mat);
			void read(Vec2D& vec);
			void close() { delete this; }
	};

}
#endif