#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <string>
#include <map>
#include <cassert>

template<typename TBase, typename TKey = std::string>
class Factory
{
public:
	static Factory* instance() {
		static Factory singleton;
		return &singleton;
	}

	struct BuilderBase
	{
		virtual TBase* create() = 0;
	};

	template<typename TDerived>
	struct Builder : public BuilderBase
	{
		Builder(const TKey& id) {
			Factory<TBase, TKey>::instance()->register_builder(id, this);
		}
		TBase* create() { return new TDerived; }
	};

	TBase* create(const TKey& id) const {
		std::map<TKey, BuilderBase*>::const_iterator it = m_builders.find(id);
		return (it == m_builders.end()) ? NULL : it->second->create();
	}

	void register_builder(const TKey& id, BuilderBase* bb) {
		std::pair<std::map<TKey, BuilderBase*>::iterator, bool> p = 
			m_builders.insert(std::make_pair(id, bb));
		assert(p.second); // builder 'id' already registered
	}

private:
	Factory() {}
	~Factory() {}
	Factory(const Factory&);
	Factory& operator=(const Factory&);

	std::map<TKey, BuilderBase*> m_builders;
};

#endif
