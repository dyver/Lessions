
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <memory>


using namespace std;



int main()
{
	
	//������� 1. ����� ��������� �������� (Raw String Literals)
	//�������� ������, ��������: my name is "Marina"
	//�) ��� ������������� Raw String Literals
	//�) ����������� Raw String Literals
	//�) �������� ����������� (Delimeter)
	{
		__asm nop
	}



	//////////////////////////////////////////////////////////////////////////////////////////////
	//������� 2. ���������� ���������� ���������� � ������� constexpr-�������.
	//
	//���������/�����������: 
	//		- constexpr � ������� ������ �������� �� ������������ ���������� return <���������>; (����!)
	//		- �� ��� ��������� ����� �������� ����� ������ constexpr � �������, � ��������� �����������
	//			����� 
	//		- ���� �������� ����������� constexpr- ������� - ��� ���������, ���������� �������� ���������
	//			�� ����� ����������



	//��������� ��� ����, ��� ���������� ��������� �������� �� ����� ���������� (���� � ��������
	//				��������� ������������ ���������, ��������� ����������� �� ����� ����������).
	//				��� �������� ���������� ������� ���������� ������ � ������������, �����������
	//				����������� constexpr-�������:

	{//��������:
		//int ar[factorial(3)];

		//���
		//constexpr int n = factorial(5);
		//int ar1[n];

		//����������:
		//int m = 7;
		//constexpr int n1 = factorial(m);
		//int ar1[n1];
		
		//� ���?
		//int n2 = factorial(m);
		__asm nop
	}



	
	//////////////////////////////////////////////////////////////////////////////////////////////


	//������� 3a. ������� � ������� ����������������� �������� �� ��������� ������������� ����������
	//� ��������, ��������: ������ "100000000" -> � �������� 256
	//��������� ��������� ����������� �������� 0b
	//����� ����� ����������������� �������� �������� ������ � �������, ��������: 100000000_b
	//������� ������������ ���������������� ������� � ������������ ���������� - const char*



	//��� ��� ���� ���� � ���������, ������� ��������� �������� �� ����� ����������
	// => ���������� ���������������� ������� ����������� constexpr - �������
	//���������/�����������: 
	//		- constexpr � ������� ������ �������� �� ������������ ���������� return <���������>;
	//		- �� ��� ��������� ����� �������� ����� ������ constexpr � �������,
	//		- ������� ����� ���� ����������� (���� �������� ����� ������� - ��� ���������,
	//					���������� �������� ��������� ������ ����������� ������� �� ����� ����������)


	{


		__asm nop

	}

	//������� 3b. ������� � ��������� �������� �������������, ��������: 256 -> "0b100000000"
	//��� ��� ������ ����� ���� ����� �����, �������� � ����� ���������� ������ std::string
	//=> ������������ �������� �� ����� ���� constexpr!
	//���������: ������������ std::bin ���� ��� => �������������� � �������� ������
	//�������� ������ �������
	//���������: ���������� �������� � ����� ���������� ��������� CHAR_BIT - <cstdint>

	
	//std::string sBin= 256_toBinStr;
	__asm nop



	//////////////////////////////////////////////////////////////////////////////////////////////
	//������� 4�. constexpr - ������� 
	//������� ����� (������ ������?) ��� �������� � ��������������� ���������� ��������.
	//� ������ ������ ����:
	//	���������� ��� �������� ������������ � ������������� ��������,
	//	������ ��� ��������� ������� �� ��������
	//	����� ��� �������� - �������� �� ��������� �������� � ��������
	//	�����, ������� �������� ����� �������� ������� ���� � ��������� �������������� ��������:
	//							���� ����������� ���������, �� ��� � ����������
	//							���� ������ ������������ ��������, ���������� �����������
	//							���� ������ ������������� ��������, ���������� ������������



	//��������� ��� ����, ��� ���������� ��������� �������� �� ����� ����������. 
	//				��� �������� ���������� ������� ���������� ������ � ������������, �����������
	//				����������� constexpr-������:
	{
	__asm nop
	}


		//������� 4b. constexpr ����� � �������� ������
		//����� � ������� ��������� �������� ������ ������ �������� � ����������� ��
		//������ �������. ������ �������� �������� �������� ����� ���� ����������� ������
		//��������� ��������, ��������������� ����� char ��� unsigned char

		//����������� constexpr ������ � ������� ����� ���������� ��������� � 
		//������������� �������� �� ����� ����������

	{
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//������� 5. unique_ptr
	{

		//5.� - ���������� ���������� ���������� ���������
		{
			std::vector<std::string*> v = { new std::string("aa"), new std::string("bb"), new std::string("cc") };
			//������������ ��� ������
		
			__asm nop
			//???
		} //???

		//5.b - ������������� ������� 1.�:
		 //���������� ����������� std::unique_ptr: 
		 //����������� ���������� (��������� ��� ������ ������� ������������ std::unique_ptr), 
		 //���������� �������� ���������� �� ����������� ����������� ������� std::string, 
		 //���������������,
		 //� ������������ ��������
		 //
			
		{
		 //������������ ��� ������

			__asm nop
		 //??? ����������� ����������� ��������� ��������?
		} //???

		{//5.c - ��������� ������� 5.b �������� ����������� �������� ���������� ������
		 //��������� ������� (��������, �������� ��������� �������: "AAA" -> "AAA_1")  
		

	
			__asm nop
		}

		{//5.d - ������������ ������ ��������	

		 //�������� unique_ptr, ������� �������� �������� ��� ������������� �������
		 //� ���������� std::string
		 //� ������� unique_ptr::operator[] ��������� ��������� ������ ����������
		 //����� ���������� ������������ ������?

			__asm nop
		}

		{//5.e - ������� ������������ �������� � ���������������� delete-������� (�������)
		 //����� �������� ������ ���������� �� ����������� ��������� �������
		 //�������� unique_ptr ��� ������ �������
		 //���������� ���������������� delete-������� (�������) ��� ����������� 
		 //������������ ������

			std::string* arStrPtr[] = { new std::string("aa"), new std::string("bb"), new std::string("cc") };

			__asm nop
		}

		{//5.f �������� � ��������� ������, ���������� unique_ptr ��� ���������� �� std::string
			//����������� ��������� copy() ���������� �������� ������� � ������ ������ � ���������� 
			//���� �� ����
			//���������: ������������ ��������� � ������ std::make_move_iterator

			__asm nop

		}
		__asm nop

	}

	////////////////////////////////////////////////////////////////////////////////
	//������� 6.shared_ptr + ���������������� delete-������� 

	//����������� ����������� ������ � ���� ������ (�������) �� ������ ���������� 
	//(��� ��������� ����� ����������� �������� ��� �������)
	//��� ��� ��� "��������" ����� �� ������� ���������� ���� ������ � ���� � ��� �� ����,
	//������� ������������ �� ����������� ������������ ����� � ��� �� ���������� FILE* =>
	//���������� �������� ��� ������ ��������� �������� shared_ptr
	//�. ������ �������� ������ �������/������� ���� ��� ������
	//�. ��� ��������� �������� ������ �������������� � �������������
	//�. ��������� �������� ��������� ������ ������� ����

	//���������: ����������� ������� ������ ����� � ������� ������� rand()
	/*
	{

	//"��������":
	//������� writer1, writer2


	//��������, ��������� ������:
	char ar1[] = "Writer1";
	char ar2[] = "Writer2";

	//�������� ����� �������� ��������� ������� ��������� ������ �� "���������" �������� � ����
	//���� �������
	//���������: ������� ������ ���������� � ���� ����������� ������� fputs()


	__asm nop
	}//�������� �����???

	*/

	
}