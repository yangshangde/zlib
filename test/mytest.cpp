#include <iostream>
#include <fstream>
#include "zlib.h"

using namespace std;

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        exit(1); \
    } \
}

string getDictFromFile()
{
	ifstream dictFile;

	dictFile.open("dict.data", ios_base::in);
	if (!dictFile.is_open())
	{
		cerr << "Failed to open dictionary file" << endl;
		return NULL;
	}

	string dictionary;
	string line;

	while (dictFile >> line)
	{
		dictionary += line;
	}

	return dictionary;
}

int testCompress()
{
	string dictionary = getDictFromFile();	

	char destBuf[2048] = {0};

	ifstream srcFile("src.json");
	string line;
	int err;
	int destLen = 0;

	string first;
	srcFile >> first;

	z_stream d_stream;
	//d_stream.zalloc = malloc;
	//d_stream.zfree = free;
	d_stream.opaque = (voidpf)0;
	err = deflateInit(&d_stream, Z_BEST_COMPRESSION);
	CHECK_ERR(err, "deflateInit");

	while (srcFile >> line)
	{
		err = deflateSetDictionary(&d_stream, (const Bytef*)(dictionary.c_str()), dictionary.size());
		//err = deflateSetDictionary(&d_stream, (const Bytef*)(first.c_str()), first.size());
		CHECK_ERR(err, "deflateSetDictionary");

		d_stream.next_in = (z_const unsigned char *)(line.c_str());
		d_stream.avail_in = line.size();		
		d_stream.next_out = (Bytef *)destBuf;
		d_stream.avail_out = 2048;

		err = deflate(&d_stream, Z_FINISH);
		if (Z_STREAM_END != err)
		{
			fprintf(stderr, "deflate should report Z_STERAM_END\n");
			exit(1);
		}

		destLen = d_stream.total_out;		


		cout << "src len=" << line.size() << ", dest len=" << destLen << ", compress ratio=" << (int)((float) destLen / (float)line.size() * 100) << "%" << endl;
		deflateResetKeep(&d_stream) ;
	}

	err = deflateEnd(&d_stream);
	CHECK_ERR(err, "deflateEnd");

	return 0;
}

int main()
{
	return testCompress();
}

