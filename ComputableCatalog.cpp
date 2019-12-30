#include "ComputableCatalog.h"

ComputableCatalog::ComputableCatalog(const char* signature, const char* language,
			uint32 fingerprint)
	: HashMapCatalog(signature,language,fingerprint)
{
}

ComputableCatalog::~ComputableCatalog()
{
}

uint32
ComputableCatalog::FP() const
{
	return ComputeFingerprint();
}
