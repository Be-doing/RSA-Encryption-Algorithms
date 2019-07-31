#include"RSA.h"




//�����㷨#######################################
//�����㷨#######################################
RSA::RSA()
{
	ProduceKeys();
}
//��������ܵ�����Ϣ
//����ֻ����Կ��ͬ������������ͬ��
long RSA::ecrept(long msg, long key, long pkey)
{
	//���ܹ�ʽΪ��ret = ��Ϣ^key%n
	//TODO�Ż���������ȡģ
	//���յĽ��Ϊ��ret = ��Ϣ^key%n == (A0 * A1 * A2 * ���� * An)
	long retmsg = 1;
	long a = msg;
	long c = pkey;
	while (key)
	{
		if (key & 1)//�ӵ�һ������0��λ��ʼ��
		{
			//A0 == a----> A0 == a^2^0 %n ----> a%n%n == a% n
			//A1 == A0 * A0 % n

			//��1����Ϣλa%n
			//�ڶ���Ϊ A0 * A1
			//��n������ (A0 * A1 * A2 * ���� *A(n - 1) *  An)
			//������ (A0 * A1 * A2 * ���� *A(n - 1) *   A(n - 1) * A(n - 1) )
			//9��ǰ���
			retmsg = (retmsg * a) % c;
		}
		//��һ��
		key >>= 1;
		a = (a * a) % c;
	}
	return retmsg;
	//long tmp = pow(msg, key) ;
	//return tmp % pkey;
}
long RSA::ProducePrime()
{
	//Ūһ���򵥵�������
	long primearr[] = { 2, 3,5 ,7,11, 13,17,19,23,29,31,37,41,43,47,53,59,61,67,71 };
	srand(time(nullptr));
	int t = rand() % (sizeof(primearr)/sizeof(long) - 1);

	return primearr[t];
}

void RSA::ProduceKeys()
{
	long prime1 = ProducePrime();
	long prime2 = ProducePrime();
	while (prime1 == prime2)
	{
		prime2 = ProducePrime();
	}
	key_.nkey = Producenkey(prime1, prime2);

	long orla = ProduceOrla(prime1, prime2);
	key_.ekey = ProduceEkey(orla);

	key_.dkey = producedkey(key_.ekey, orla);
}
long RSA::Producenkey(long prime1, long prime2)
{
	//ѡ����������ȵ�����p��q���� n = pq
	return prime1 * prime2;
}
long RSA::ProduceOrla(long prime1, long prime2)
{
	//ŷ����ʽ��f(n)
	//���a��������f(n) = n - 1
	//���a, b����������f(ab) = (a - 1)(b - 1)
	//���a, b�ǻ��ʣ�f(ab) = f(a)f(b)
	return (prime1 - 1)*(prime2 - 1);
}

long RSA::ProduceGcd(long ekey, long orla)
{
	if (orla == 0)
		return ekey;

	return RSA::ProduceGcd(orla, ekey%orla);
}
long RSA::ProduceEkey(long orla)
{
	//�õ�f(n)��ѡ��һ��1 < E < f(n)����E��n���ʣ��ù�Կ(E, n)
	//��Լ��Ϊ 1 ���������ǻ��ʵģ���˱���������һ��E�������ɡ�
	long i = 2;
	for ( i; i < orla; ++i)
	{
		if (ProduceGcd(i, orla) == 1)
			break;
	}
	return i;
}


long RSA::producedkey(long ekey, long orla)
{

	long D = orla / ekey;
	for (D; D < orla; ++D)
	{
		if ((D*ekey) % orla == 1)
			break;
	}
	return D;
}


//���ܹ���############################
//���ܹ���############################
Key RSA::GetKey()
{
		return key_;
}
std::vector<long> RSA::Ecrept(std::string& str_in, long ekey, long pkey)
{
	std::vector<long> retv;
	for (size_t i = 0; i < str_in.size(); ++i)
	{
		retv.push_back(ecrept(str_in[i], ekey, pkey));
	}
	return retv;
}
std::string RSA::Decrept(std::vector<long>& ecrept_str, long dkey, long pkey)
{
	std::string retstr;
	for (size_t i = 0; i < ecrept_str.size(); ++i)
	{
		retstr += (char)ecrept(ecrept_str[i], dkey, pkey);
	}
	return retstr;
}





const int NUM = 256;//ÿ�ζ�256�ֽ�

void RSA::FileEcrept(const char* plain_file_in, const char* ecrept_file_out,
	long ekey, long pkey)
{
	std::ifstream file_in(plain_file_in, std::ios::binary);
	std::ofstream file_out(ecrept_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open() && file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	char buff_in[NUM];
	long buff_out[NUM];
	int curnum;//��ǰ��ȡ���ֽ�
	while (!file_in.eof())
	{
		file_in.read(buff_in, NUM);
		curnum = file_in.gcount();
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = ecrept((long)buff_in[i], ekey, pkey);
		}
		file_out.write((char*)buff_out, curnum * sizeof(long));
	}

	file_in.close();
	file_out.close();
}
void RSA::FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
	long dkey, long pkey)
{
	std::ifstream file_in(ecrept_file_in , std::ios::binary);
	std::ofstream file_out(plain_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open()&&file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	long buff_in[NUM];
	char buff_out[NUM];
	int curnum;//��ǰ��ȡ���ֽ�
	while (!file_in.eof())
	{
		file_in.read((char*)buff_in, NUM * sizeof(long));
		curnum = file_in.gcount();
		curnum /= sizeof(long);
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = (char)ecrept(buff_in[i], dkey, pkey);
		}
		file_out.write(buff_out, curnum);
	}

	file_in.close();
	file_out.close();
}
void RSA::PrintInfo(std::vector<long>& ecrept_str)
{
	for (const auto e : ecrept_str)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
}