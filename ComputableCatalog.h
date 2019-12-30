#ifndef COMPUTABLECATALOG_H
#define COMPUTABLECATALOG_H

#include "/boot/home/git/haiku/headers/private/locale/HashMapCatalog.h"

class ComputableCatalog : public BPrivate::HashMapCatalog
{
public:
				ComputableCatalog(const char* signature, const char* language,
			uint32 fingerprint);
	virtual		~ComputableCatalog();
	uint32		FP() const;
};


#endif
