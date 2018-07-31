#ifndef _NIMA_NESTEDACTORASSET_HPP_
#define _NIMA_NESTEDACTORASSET_HPP_

#include <string>
#include <vector>
#include <functional>

namespace nima
{
	class BlockReader;
	class Actor;

	class NestedActorAsset
	{
		protected:
			std::string m_Name;
			std::string m_Id;

		public:

			NestedActorAsset();
			virtual ~NestedActorAsset();

			const std::string& name() const;
			const std::string& id() const;

			virtual Actor* actor();
			void read(BlockReader* reader);

	};
}
#endif