#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include "opencv2/opencv.hpp"

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



// ###################### IDAT チャンクを表示  ###################### 
/*
* データのバイナリをコピーして表示
*/
void ImagePrint(const char* fileName, int pos,char* copy,int copySize)
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
		printf("fileSize: %zd\n",fileSize);
		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
		unsigned char* fileData = new unsigned char[fileSize];	//ファイルデータ	
		fs.read((char*)fileData, fileSize);	//バイナリ読み込み

		/////////////////////////////////////////////////////////////////////////////
		//BytePrint("texture.png",pos - 8, pos - 4);

		int dataSize = convInt(fileData, pos - 8);
	

		printf("dataSize: %d\n",dataSize);
		
		int a = 0;
		for (int i = pos; i < dataSize; i++)
		{
			std::cout<< (unsigned int)fileData[i]<<" ";

			a++;
			if (a == 20)
			{
				printf("\n");
				a = 0;
			}
		}
		

		////////////////////////////////////////////////////////////////////////////






		fs.seekg(0, fs.beg);	//シーク位置を初期に戻す
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

	int pos[1000];
	ImageDataChunk("texture.png",pos,100);

	std::cout << "チャンクデータサイズ:　";
	BytePrint("texture.png", 54, 58);
	std::cout << std::endl;

	std::cout << "チャンクタイプ:　";
	BytePrint("texture.png", 58, 62);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	ImagePrint("texture.png", pos[0]);








	return 0;
}