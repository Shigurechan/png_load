#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include "opencv2/opencv.hpp"

// ###################### �t�@�C����S���\��  ###################### 
void AllBinaryPrint(const char* fileName)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "�t�@�C�����J���܂���B" << std::endl;
	}
	else
	{
		//std::cout << "�t�@�C�����J���܂����B" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//�t�@�C���T�C�Y
		
		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		char* fileData = new char[fileSize];	//�t�@�C���f�[�^

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

		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�

		delete fileData;

	}

	fs.close();
}



// ###################### �o�C�g���w�肵�ĕ\��  ###################### 
void BytePrint(const char* fileName,int start,int end)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "�t�@�C�����J���܂���B" << std::endl;
	}
	else
	{
		//std::cout << "�t�@�C�����J���܂����B" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//�t�@�C���T�C�Y

		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		char* fileData = new char[fileSize];	//�t�@�C���f�[�^

		if (start < fileSize && end < fileSize)
		{
			
			fs.read(fileData, fileSize);	//�o�C�i���ǂݍ���

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

			fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�

		}
		else 
		{
			std::cerr << "�w�肵���ʒu�̓t�@�C���̃T�C�Y�O�ł��B" << std::endl;
		}

		delete fileData;	//�������J��

	}

	fs.close();	//�t�@�C�������

}


// ###################### ���f�[�^�̈ʒu���擾  ###################### 
void ImageDataChunk(const char *fileName,int pos[],int arrySize)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "�t�@�C�����J���܂���B" << std::endl;
	}
	else
	{
		//std::cout << "�t�@�C�����J���܂����B" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//�t�@�C���T�C�Y

		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		char* fileData = new char[fileSize];	//�t�@�C���f�[�^	
		fs.read(fileData, fileSize);	//�o�C�i���ǂݍ���

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
							printf("������: %d\n",++i);
							pos[num] = i;
							++num;
						}
					}
				}
			}

			if (arrySize < num)
			{
				std::cerr << "arraySize�𒴂��܂����B" << std::endl;
			}
		}

		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		delete fileData;	//�������J��

	}

	fs.close();	//�t�@�C�������

}

// ###################### int�^�ϊ�  ###################### 
int convInt(unsigned char* header, int start)
{
	int ret = (header[start + 0] << 24) | (header[start + 1] << 16) | (header[start + 2] << 8) | (header[start + 3]);
//	int ret = (header[start + 3] << 24) | (header[start + 2] << 16) | (header[start + 1] << 8) | (header[start + 0]);
	return ret;
}



// ###################### IDAT �`�����N��\��  ###################### 
/*
* �f�[�^�̃o�C�i�����R�s�[���ĕ\��
*/
void ImagePrint(const char* fileName, int pos,char* copy,int copySize)
{
	std::fstream fs(fileName, std::ios_base::binary | std::ios_base::in);

	if (fs.is_open() == false)
	{
		std::cerr << "�t�@�C�����J���܂���B" << std::endl;
	}
	else
	{
		//std::cout << "�t�@�C�����J���܂����B" << std::endl;

		size_t fileSize = fs.seekg(0, fs.end).tellg();	//�t�@�C���T�C�Y
		printf("fileSize: %zd\n",fileSize);
		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		unsigned char* fileData = new unsigned char[fileSize];	//�t�@�C���f�[�^	
		fs.read((char*)fileData, fileSize);	//�o�C�i���ǂݍ���

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






		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		delete fileData;	//�������J��

	}

	fs.close();	//�t�@�C�������

}


int main()
{

	//AllBinaryPrint("texture.png");	//�S�\��
	std::cout<<"png �w�b�_�[�F ";
	BytePrint("texture.png",0,8);
	std::cout << std::endl;
	
	/* ############### IHDR ############### */
	std::cout<<"############### IHDR ############### "<<std::endl;

	std::cout << "�`�����N�f�[�^�T�C�Y�F�@";
	BytePrint("texture.png",8, 12);		//13byte �Œ�
	std::cout << std::endl;

	std::cout << "�`�����N�^�C�v�F�@";
	BytePrint("texture.png", 12, 16);
	std::cout << std::endl;

	std::cout << "�摜�̕��F�@";
	BytePrint("texture.png", 16, 20);
	std::cout <<std::endl;

	std::cout << "�摜�̏c�F�@";
	BytePrint("texture.png", 20, 24);
	std::cout << std::endl;

	std::cout << "�r�b�g�[�x�F�@";
	BytePrint("texture.png", 24, 25);
	std::cout << std::endl;

	std::cout << "�J���[�^�C�v�F�@";
	BytePrint("texture.png", 25, 26);
	std::cout << std::endl;

	std::cout << "���k�F�@";
	BytePrint("texture.png", 26, 27);
	std::cout << std::endl;

	std::cout << "�t�B���^�[�F�@";
	BytePrint("texture.png", 27, 28);
	std::cout << std::endl;

	std::cout << "�C���^�[���X�F�@";
	BytePrint("texture.png", 28, 29);
	std::cout << std::endl;

	std::cout << "CRC�F�@";
	BytePrint("texture.png", 29, 33);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	/* ############### IDATA ############### */
	std::cout<<"############### IDATA ###############"<<std::endl;

	int pos[1000];
	ImageDataChunk("texture.png",pos,100);

	std::cout << "�`�����N�f�[�^�T�C�Y:�@";
	BytePrint("texture.png", 54, 58);
	std::cout << std::endl;

	std::cout << "�`�����N�^�C�v:�@";
	BytePrint("texture.png", 58, 62);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	ImagePrint("texture.png", pos[0]);








	return 0;
}