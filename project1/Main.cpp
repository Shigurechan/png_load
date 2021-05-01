#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include "opencv2/opencv.hpp"



unsigned char* gPictureData = nullptr;	//画像データ
int gPictureDataSize = 0;				//画像データサイズ


// ###################### ファイルを全部表示  ###################### 
void AllBinaryPrint(const char* fileName)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "ファイルが開けません。" << std::endl;
	}
	else
	{
		//std::cout << "ファイルを開きました。" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//ファイルサイズ
		
		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		char* fileData = new char[fileSize];	//ファイルデータ

		fs.read(fileData, fileSize);

		int a = 0;
		for (int i = 0; i < fileSize; i++)
		{
			printf("%x ", (unsigned char)fileData[i]);

			a++;
			if (a == 20)
			{
				printf("\n");
				a = 0;
			}

		}

		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す

		delete fileData;

	}

	fs.close();
}



// ###################### バイトを指定して表示  ###################### 
void BytePrint(const char* fileName,int start,int end)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "ファイルが開けません。" << std::endl;
	}
	else
	{
		//std::cout << "ファイルを開きました。" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//ファイルサイズ

		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		char* fileData = new char[fileSize];	//ファイルデータ

		if (start < fileSize && end < fileSize)
		{
			
			fs.read(fileData, fileSize);	//バイナリ読み込み

			int a = 0;
			for (int i = start; i < end; i++)
			{
				//printf("%x ", (unsigned char)fileData[i]);
				std::cout<< std::hex << (unsigned int)(unsigned char)fileData[i] << " ";

				a++;
				if (a == 20)
				{
					printf("\n");
					a = 0;
				}

			}

			fs.seekg(0, fs.beg);	//シーク位置を初期に戻す

		}
		else 
		{
			std::cerr << "指定した位置はファイルのサイズ外です。" << std::endl;
		}

		delete fileData;	//メモリ開放

	}

	fs.close();	//ファイルを閉じる

}


// ###################### 実データの位置を取得  ###################### 
void ImageDataChunk(const char *fileName,int pos[],int arrySize)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "ファイルが開けません。" << std::endl;
	}
	else
	{
		//std::cout << "ファイルを開きました。" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//ファイルサイズ

		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		char* fileData = new char[fileSize];	//ファイルデータ	
		fs.read(fileData, fileSize);	//バイナリ読み込み

		int num = 0;
		for (int i = 0; i < fileSize; i++)
		{
			if (fileData[i] == 'I')
			{
				if (fileData[++i] == 'D')
				{
					if (fileData[++i] == 'A')
					{
						if (fileData[++i] == 'T')
						{
							printf("見つけた: %d\n",++i);
							pos[num] = i;
							++num;
						}
					}
				}
			}

			if (arrySize < num)
			{
				std::cerr << "arraySizeを超えました。" << std::endl;
			}
		}

		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		delete fileData;	//メモリ開放

	}

	fs.close();	//ファイルを閉じる

}

// ###################### int型変換  ###################### 
int convInt(unsigned char* header, int start)
{
	int ret = (header[start + 0] << 24) | (header[start + 1] << 16) | (header[start + 2] << 8) | (header[start + 3]);
//	int ret = (header[start + 3] << 24) | (header[start + 2] << 16) | (header[start + 1] << 8) | (header[start + 0]);
	return ret;
}

// ###################### バイナリコピー  ###################### 

//データ書き込み
void bincat(char* s1, size_t s1_size, size_t s1_max_size, const char* s2, size_t s2_size)
{
	for (int i = 0; i < s2_size; i++)
	{
		if ((s1_size + i) > s1_max_size)
		{
			break;
		}
		else
		{
			s1[s1_size + i] = s2[i];
		}
	}	
}

//バイナリ追加
void AddPictureData(const unsigned char* data, int data_size)
{
	int size = gPictureDataSize + data_size;    //最大サイズ
	unsigned char* str = new unsigned char[size] {0x00};

	bincat((char*)str, 0, size, (char*)gPictureData, gPictureDataSize);
	bincat((char*)str, gPictureDataSize, size, (char*)data, data_size);


	gPictureDataSize = size;					//実データサイズ数を更新
	delete gPictureData;
	gPictureData = str;
}




// ###################### IDAT チャンクを表示  ###################### 
/*
* データのバイナリをコピーして表示
*/
void ImagePrint(const char* fileName, int pos)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "ファイルが開けません。" << std::endl;
	}
	else
	{
		//std::cout << "ファイルを開きました。" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//ファイルサイズ
		//printf("fileSize: %zd\n",fileSize);
		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		unsigned char* fileData = new unsigned char[fileSize];	//ファイルデータ	
		fs.read((char*)fileData, fileSize);	//バイナリ読み込み

		
		int dataSize = convInt(fileData, pos - 8);//バイナリサイズを取得
		unsigned char* data = new unsigned char[dataSize];

		for (int i = 0; i < dataSize; i++)
		{
			data[i] = fileData[i];
		}


		AddPictureData(data, dataSize);	//バイナリデータを追加
		


		
		delete data;
		data = nullptr;

		//fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		delete fileData;	//メモリ開放

	}

	fs.close();	//ファイルを閉じる

}


int main()
{

	//AllBinaryPrint("texture.png");	//全表示
	std::cout<<"png ヘッダー： ";
	BytePrint("texture.png",0,8);
	std::cout << std::endl;
	
	/* ############### IHDR ############### */
	std::cout<<"############### IHDR ############### "<<std::endl;

	std::cout << "チャンクデータサイズ：　";
	BytePrint("texture.png",8, 12);		//13byte 固定
	std::cout << std::endl;

	std::cout << "チャンクタイプ：　";
	BytePrint("texture.png", 12, 16);
	std::cout << std::endl;

	std::cout << "画像の幅：　";
	BytePrint("texture.png", 16, 20);
	std::cout <<std::endl;

	std::cout << "画像の縦：　";
	BytePrint("texture.png", 20, 24);
	std::cout << std::endl;

	std::cout << "ビット深度：　";
	BytePrint("texture.png", 24, 25);
	std::cout << std::endl;

	std::cout << "カラータイプ：　";
	BytePrint("texture.png", 25, 26);
	std::cout << std::endl;

	std::cout << "圧縮：　";
	BytePrint("texture.png", 26, 27);
	std::cout << std::endl;

	std::cout << "フィルター：　";
	BytePrint("texture.png", 27, 28);
	std::cout << std::endl;

	std::cout << "インターレス：　";
	BytePrint("texture.png", 28, 29);
	std::cout << std::endl;

	std::cout << "CRC：　";
	BytePrint("texture.png", 29, 33);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	/* ############### IDATA ############### */
	std::cout<<"############### IDATA ###############"<<std::endl;


	int searchCount = 1000;
	int* pos = new int[searchCount];
	for (int i = 0; i < searchCount; i++)
	{
		pos[i] = 0;
	}


	ImageDataChunk("texture.png",pos, searchCount);

	std::cout << "チャンクデータサイズ:　";
	BytePrint("texture.png", 54, 58);
	std::cout << std::endl;

	std::cout << "チャンクタイプ:　";
	BytePrint("texture.png", 58, 62);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	
	for (int i = 0; i < searchCount; i++)
	{
		if (pos[i] != 0) 
		{
			ImagePrint("texture.png", pos[i]);
		}
		else
		{
			break;
		}

	}

	printf("実画像データサイズ: %d\n",gPictureDataSize);
	







	

	delete[] pos;
	pos = nullptr;

	return 0;
}