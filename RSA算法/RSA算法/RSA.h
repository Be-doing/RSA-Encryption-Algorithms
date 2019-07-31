#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<time.h>
#include<Windows.h>
#include<fstream>

struct Key
{
	//��Կ(ekey, pkey): (e,n)
	long nkey;
	long ekey;
	//˽Կ(dkey, pkey): (d, n)
	long dkey;
};

class RSA
{
public:
	RSA();
	Key GetKey();
	void FileEcrept(const char* plain_file_in, const char* ecrept_file_out,
		long ekey, long pkey);
	void FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
		long dkey, long pkey);

	std::vector<long> Ecrept(std::string& str_in, long ekey, long pkey);
	std::string Decrept(std::vector<long>& ecrept_str, long dkey, long pkey);

	void PrintInfo(std::vector<long>& ecrept_str);
private:
	//��������ܵ�����Ϣ
	long ecrept(long msg, long key, long pkey);
	//��������
	long ProducePrime();
	//��������Կ��
	void ProduceKeys();
	//����n
	long Producenkey(long prime1, long prime2);
	//ŷ����ʽ������
	long ProduceOrla(long prime1, long prime2);
	//�����Լ��
	long ProduceGcd(long ekey, long orla);
	//������Կ
	long ProduceEkey(long orla);

	//����˽Կ
	long producedkey(long ekey, long orla);

private:
	Key key_;
};