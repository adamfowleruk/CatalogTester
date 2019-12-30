
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include <Entry.h>
#include <File.h>
#include <String.h>

#include "ComputableCatalog.h"

#include "/boot/home/git/haiku/headers/private/locale/EditableCatalog.h"
#include "/boot/home/git/haiku/headers/private/locale/HashMapCatalog.h"
//#include "/boot/home/git/haiku/headers/private/shared/HashMap.h"

using std::vector;
using std::map;

int fingerprint(vector<const char*>& inputFiles,const char* catalogSig)
{
	uint32 count = inputFiles.size();

	std::map<uint32,uint32> sourceHashByLine;
	std::map<uint32,std::string> sourceKeyByLine;
	
	std::string currentItem;
	bool sourceFile = true;
	for( uint32 i=0; i<count; ++i) {
		printf("----\nProcessing file: %s\n",inputFiles[i]);
		std::fstream catalogFile;
		catalogFile.open(inputFiles[i], std::fstream::in);
		if (!catalogFile.is_open())
		{
			printf("Failed to open file!!! %s",inputFiles[i]);
		}
		
		if (std::getline(catalogFile,currentItem,'\n')) {
			printf("Read and ignoring signature line\n");
			printf(currentItem.c_str());
			printf("\n");
		}
		std::string originalString;
		std::string context;
		std::string comment;
		std::string translated;

		//HashMap<CatKey, BString> fCatMap();
		ComputableCatalog hmp(catalogSig,"native",0);
		uint32 lines = 1;
		bool failed = false;
		while (std::getline(catalogFile, originalString,'\t').good()) {
			//printf("Read line\n");
			// Each line is : "original string \t context \t comment \t translation"

			if (!std::getline(catalogFile, context,'\t').good())
				return B_ERROR;

			if (!std::getline(catalogFile, comment,'\t').good())
				return B_ERROR;

			if (!std::getline(catalogFile, translated).good())
				return B_ERROR;

			// We could do that :
			// SetString(key, translated.c_str());
			// but we want to keep the strings in the new catkey. Hash collisions
			// happen, you know. (and CatKey::== will fail)
			//SetString(originalString.c_str(), translated.c_str(), context.c_str(),
			//	comment.c_str()); 
		
			hmp.SetString(originalString.c_str(), translated.c_str(), context.c_str(),
				comment.c_str());
	
			lines++;
			
			if (sourceFile) {
				// store the signature
				sourceHashByLine.insert(std::pair<uint32,uint32>(lines,hmp.FP()));
				sourceKeyByLine.insert(std::pair<uint32,std::string>(lines,std::string(originalString)));
			} else {
				// check our signature matches
				if (!failed) {
					uint32 sourceFP = sourceHashByLine.find(lines)->second;
					std::string sourceKey = sourceKeyByLine.find(lines)->second;
					uint32 thisFP = hmp.FP();
					if (thisFP != sourceFP)
					{
						printf("HASH DIVERGED FROM SOURCE at line %i. Source fingerprint: %i, this fingerprint: %i\n",lines,sourceFP,thisFP);
						printf("    source file: key: '%s'\n",sourceKey.c_str());
						printf("    this file  : key: '%s', context: '%s', comment: '%s'\n",originalString.c_str(),context.c_str(),comment.c_str());
						failed = true;
					}
				}
			}
		}
		catalogFile.close();
		
		// Now compute and print FP
		//uint32 checkFP = ComputeFingerprint();
		printf("fingerprint for %s is %i over %i lines \n",inputFiles[i],hmp.FP(),lines);
		sourceFile = false;
		
	}
	return 0;
}

int doLinkCheck(vector<const char*>& inputFiles,const char* catalogSig)
{
	//vector<const char *> inputFiles;
	//inputFiles.push_back("/boot/home/git/Paladin/Paladin/locales/en.catkeys");
	//inputFiles.push_back("/boot/home/git/Paladin/Paladin/locales/de.catkeys");
	//const char* catalogSig = "application/x-vnd.dw-Paladin";
	
	uint32 res;
	uint32 count = inputFiles.size();
	for( uint32 i=0; i<count; ++i) {
		printf("----\nProcessing file: %s\n",inputFiles[i]);
		BPrivate::EditableCatalog inputCatalog("plaintext", catalogSig, "native");
		if ((res = inputCatalog.ReadFromFile(inputFiles[i])) != B_OK) {
			fprintf(stderr, "couldn't load source-catalog %s - error: %s\n",
				inputFiles[i], strerror(res));
			//exit(-1);
		} else {
			fprintf(stdout, "read catkeys file fine %s\n",inputFiles[i]);
		}
		
	}
	printf("All files checked\n");
	return 0;
}

void
usage()
{
	printf("Usage: CatalogTester <operation> <appSignature> <sourceCatkey> <translation1> <translation2> ...\n");
	printf("\t<operation>\tEither 'fingerprint' or 'validate'\n");
	printf("\t\tvalidate:\tWill check each file's fingerprint matches that stated in the file\n");
	printf("\t\tfingerprint:\tWill introspect each file, comparing each line with the source file\n");
	printf("\t<appSignature>\tE.g. application/x-vnd.dw-Paladin\n");
	printf("\t<sourceCatKey>\tThe file path of the source language catkey file\n");
	printf("\t<translationX>\tA translation catkey file path\n");
}

int
main(int argc, char **argv)
{
	if (argc < 4)
	{
		usage();
		return -1;
	}
	vector<const char *> inputFiles;
	const char* catalogSig = argv[2];
	for (int i = 3;i < argc;i++) {
		inputFiles.push_back(argv[i]);
	}
	//inputFiles.push_back("/boot/home/Projects/CatalogTester/en.txt");
	//inputFiles.push_back("/boot/home/Projects/CatalogTester/de.txt");
	//inputFiles.push_back("/boot/home/Projects/CatalogTester/it.txt");
	//const char* catalogSig = "application/x-vnd.dw-Paladin";
	
	if (0 == strcmp("fingerprint",argv[1]))
	{
		return fingerprint(inputFiles,catalogSig);
	} else if (0 == strcmp("validate",argv[1])) {
		return doLinkCheck(inputFiles,catalogSig);	
	} else {
		printf("Unknown command");
		return 0;
	}
}

