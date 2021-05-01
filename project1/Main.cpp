#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include "opencv2/opencv.hpp"



unsigned char* gPictureData = nullptr;	//�摜�f�[�^
int gPictureDataSize = 0;				//�摜�f�[�^�T�C�Y


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

// ###################### �o�C�i���R�s�[  ###################### 

//�f�[�^��������
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

//�o�C�i���ǉ�
void AddPictureData(const unsigned char* data, int data_size)
{
	int size = gPictureDataSize + data_size;    //�ő�T�C�Y
	unsigned char* str = new unsigned char[size] {0x00};

	bincat((char*)str, 0, size, (char*)gPictureData, gPictureDataSize);
	bincat((char*)str, gPictureDataSize, size, (char*)data, data_size);


	gPictureDataSize = size;					//���f�[�^�T�C�Y�����X�V
	delete gPictureData;
	gPictureData = str;
}




// ###################### IDAT �`�����N��\��  ###################### 
/*
* �f�[�^�̃o�C�i�����R�s�[���ĕ\��
*/
void ImagePrint(const char* fileName, int pos)
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
		//printf("fileSize: %zd\n",fileSize);
		fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
		unsigned char* fileData = new unsigned char[fileSize];	//�t�@�C���f�[�^	
		fs.read((char*)fileData, fileSize);	//�o�C�i���ǂݍ���

		
		int dataSize = convInt(fileData, pos - 8);//�o�C�i���T�C�Y���擾
		unsigned char* data = new unsigned char[dataSize];

		for (int i = 0; i < dataSize; i++)
		{
			data[i] = fileData[i];
		}


		AddPictureData(data, dataSize);	//�o�C�i���f�[�^��ǉ�
		


		
		delete data;
		data = nullptr;

		//fs.seekg(0, fs.beg);	//�V�[�N�ʒu�������ɖ߂�
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


	int searchCount = 1000;
	int* pos = new int[searchCount];
	for (int i = 0; i < searchCount; i++)
	{
		pos[i] = 0;
	}


	ImageDataChunk("texture.png",pos, searchCount);

	std::cout << "�`�����N�f�[�^�T�C�Y:�@";
	BytePrint("texture.png", 54, 58);
	std::cout << std::endl;

	std::cout << "�`�����N�^�C�v:�@";
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

	printf("���摜�f�[�^�T�C�Y: %d\n",gPictureDataSize);
	







	

	delete[] pos;
	pos = nullptr;

	return 0;
}