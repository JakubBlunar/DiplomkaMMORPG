#ifndef OBJECT_POOL_ITEM
#define OBJECT_POOL_ITEM

class ObjectPoolItem {
public:
	virtual ~ObjectPoolItem() = default;
	virtual ObjectPoolItem* clone() = 0;
	virtual void reset() = 0;
};

#endif
