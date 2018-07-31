#include "BlockReader.hpp"
#include "Exceptions/OverflowException.hpp"

#include <nima/Vec2D.hpp>
#include <nima/Mat2D.hpp>

using namespace nima;


BlockReader::BlockReader(unsigned char* data, unsigned int length) : BinaryReader(data, length, BinaryReader::LittleEndian)
{

}

BlockReader::BlockReader(int type, unsigned char* data, unsigned int length) : BinaryReader(data, length, BinaryReader::LittleEndian), m_BlockType(type)
{
}

void BlockReader::read(Mat2D& mat)
{
	float arr[6];
	readFloatArray(arr, 6);
	mat[0] = arr[0];
	mat[1] = arr[1];
	mat[2] = arr[2];
	mat[3] = arr[3];
	mat[4] = arr[4];
	mat[5] = arr[5];
}

void BlockReader::read(Vec2D& vec)
{
	float arr[2];
	readFloatArray(arr, 2);
	vec[0] = arr[0];
	vec[1] = arr[1];
}

BlockReader* BlockReader::readNextBlock()
{
	if(m_DataPosition == m_DataLength)
	{
		return nullptr;
	}
	unsigned char blockType = readByte();
	unsigned int length = readUnsignedInt();

	if(m_DataPosition + length > m_DataLength)
	{
		throw OverflowException("Block length is invalid and would cause overflow in BlockReader::readNextBlock.");
	}
	unsigned char* blockData = m_Data + m_DataPosition;

	m_DataPosition += length;
	return new BlockReader((int)blockType, blockData, length);
}